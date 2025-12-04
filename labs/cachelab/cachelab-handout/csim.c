/* csim.c
 *
 * Cache simulator that, 
 *      reads a valgrind memory trace as input,
 *      simulates hit/miss behaviour of a cache memory,
 *      outputs the total number of hits, misses and evictions. 
 *
 * Takes several command line arguments,
 *      -v:     Optional verbose flag that displays trace info
 *      -s <s>: Number of set index bits (S = 2^s -> number of sets)
 *      -E <E>: Associativity (number of lines per set)
 *      -b <b>: Number of block bits (B = 2^b -> block size) 
 *      -t <trace_file>: Name of the valgrind trace to replay.
 */

#include "cachelab.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_SIZE 256


typedef struct 
{
    char op;
    unsigned long full_addr;
    int sz;
} TraceEntry;

typedef struct
{
    char usage;
    char verb;
    int s;
    int E;
    int b;
    char* trace_file; 
} CacheConfig;

typedef struct
{
    unsigned long tag;
    unsigned long set_idx;
    unsigned long block_offset;
} AddressComponents;


typedef struct
{
    char valid_bit;
    unsigned long tag;
    unsigned long time;
} CacheLine; 

typedef struct
{
    CacheLine* lines;
} CacheSet;

typedef struct
{
    int E; 
    int S;
    CacheSet* sets;
} Cache;


typedef struct
{
    int hit_count;
    int miss_count;
    int eviction_count;
} Count;

typedef struct
{
    char hit;
    char missed;
    char evicted;
} ResultFlags;


void printSummary(int hit_count, int miss_count, int eviction_count);

void init_config(int argc, char* argv[], CacheConfig* conf);

Cache* init_cache(int s, int E);

AddressComponents parse_address(unsigned long addr, int s, int b);

TraceEntry parse_trace(char* line);

char is_hit(CacheSet* cache, int E, unsigned long tag);

CacheLine* find_empty_line(CacheSet* set, int E);

int generate_uniform_dist_num(int L, int H);

ResultFlags access_cache(CacheSet* set, char op, unsigned long tag, int E, Count* cnt);

void printSummary(int hit_count, int miss_count, int eviction_count);

void print_verbose(TraceEntry t, ResultFlags flags);


unsigned long global_cnt = 0;


int main(int argc, char* argv[])
{
    CacheConfig config = {0};
    init_config(argc, argv, &config);
    Cache* cache = init_cache(config.s, config.E);
    Count cnt = {0};

    // read trace file
    FILE* fp = fopen(config.trace_file, "r");
    char line[MAX_SIZE]; 
    while (fgets(line, sizeof(line), fp))
    {
        TraceEntry t = parse_trace(line); 
        if (t.op == 'I') 
            continue;
        AddressComponents addr = parse_address(t.full_addr, config.s, config.b);
        CacheSet* set = &cache->sets[addr.set_idx];
        ResultFlags r = access_cache(set, t.op, addr.tag, config.E, &cnt);
        if (config.verb)
            print_verbose(t, r);
    }  
    printSummary(cnt.hit_count, cnt.miss_count, cnt.eviction_count);
    return 0;
}


void init_config(int argc, char* argv[], CacheConfig* conf)
{
    int opt; 
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
            case 'h':   conf->usage = 1;            break;
            case 'v':   conf->verb = 1;             break;
            case 's':   conf->s = atoi(optarg);     break;
            case 'E':   conf->E = atoi(optarg);     break;
            case 'b':   conf->b = atoi(optarg);     break;
            case 't':   conf->trace_file = optarg; break;
            default:    exit(EXIT_FAILURE);
        }
    }
}


Cache* init_cache(int s, int E)
{
    Cache* cache = malloc(sizeof(Cache)); 
    cache->S = (1 << s);
    cache->E = E;
    cache->sets = malloc(sizeof(CacheSet) * cache->S);  // S = 2^s sets
    for (int i = 0; i < cache->S; i++)
    {
        cache->sets[i].lines = malloc(sizeof(CacheLine) * E);
        for (int j = 0; j < E; j++) 
        {
            cache->sets[i].lines[j].valid_bit = 0;
            cache->sets[i].lines[j].tag = 0;
        }
    }  
    return cache;
}


TraceEntry parse_trace(char* line)
{
    TraceEntry entry;  

    // get operator
    int i = 0;
    if (line[i] == 'I')
        entry.op = line[i];
    else
        entry.op = line[++i];

    // skip whitespace
    i++;

    // get address
    int start = i;
    while (line[i] != ',')
        i++;
    entry.full_addr = strtol(&line[start], NULL, 16); 

    // skip comma
    i++;

    // get size
    entry.sz = atoi(&line[i]);
    
    return entry;
}


AddressComponents parse_address(unsigned long addr, int s, int b)
{
    AddressComponents comp;
    comp.block_offset = addr & ((1UL << b) - 1);
    comp.set_idx = (addr >> b) & ((1UL << s) -1);
    comp.tag = addr >> (s + b); 
    return comp; 
}


char is_hit(CacheSet* set, int E, unsigned long tag)
{
    for (int i = 0; i < E; i++)
    {
        CacheLine* line = &set->lines[i];
        if (line->tag == tag && line->valid_bit == 1)
        {
            line->time = global_cnt++;
            return 1;
        }
    } 
    return 0;
}


CacheLine* find_empty_line(CacheSet* set, int E)
{
    CacheLine* line;     
    for (int i = 0; i < E; i++)
    {
        line = &set->lines[i];
        if (line->valid_bit == 0)
            return line;
    }
    return NULL;
}


CacheLine* find_lru_line(CacheSet* set, int E)
{
    CacheLine* lru_line = &set->lines[0];
    for (int i = 0; i < E; i++)
    {
        if (set->lines[i].time < lru_line->time)
            lru_line = &set->lines[i];
    }
    return lru_line;
}


ResultFlags access_cache(CacheSet* set, char op, unsigned long tag, int E, Count* cnt)
{
    ResultFlags flags = {0};
    if (is_hit(set, E, tag))
    {
        cnt->hit_count++;
        flags.hit = 1;
        if (op == 'M')
            cnt->hit_count++;
    }
    else
    {
        cnt->miss_count++;
        flags.missed = 1;
        if (op == 'M')
        {
            cnt->hit_count++; 
            flags.hit = 1;
        }
        CacheLine* empty_line = find_empty_line(set, E);
        if (empty_line == NULL)
        {
            cnt->eviction_count++;
            flags.evicted = 1;
            empty_line = find_lru_line(set, E); 
        }
        empty_line->valid_bit = 1;  
        empty_line->tag = tag;
        empty_line->time = global_cnt++;
    }    
    return flags;
}


void print_verbose(TraceEntry t, ResultFlags flags)
{
    printf("%c 0x%lx,%d ", t.op, t.full_addr, t.sz);
    if (flags.missed)  printf("miss ");
    if (flags.evicted) printf("eviction ");
    if (flags.hit)     printf("hit");
    printf("\n");
}


void printSummary(int hit_count, int miss_count, int eviction_count)
{
    printf("hits:%d misses:%d evictions:%d\n", hit_count, miss_count, eviction_count);
}

