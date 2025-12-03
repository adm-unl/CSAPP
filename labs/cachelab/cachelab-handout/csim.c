#include "cachelab.h"

#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_SIZE 256

void printSummary(int hit_count, int miss_count, int eviction_count);

void partition_trace(char* line);

void partition_address(char* addr);

int main(int argc, char* argv[])
{
    char usage = 0, verb = 0; 
    int n_setb, n_lines, n_blockb;
    char trace_file[MAX_SIZE];

    // read command line args
    int opt; 
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
            case 'h':   usage = 1;              break;
            case 'v':   verb = 1;               break;
            case 's':   n_setb = atoi(optarg);  break;
            case 'E':   n_lines = atoi(optarg); break;
            case 'b':   n_blockb = atoi(optarg);    break;
            case 't':   strcpy(trace_file, optarg); break;
            default:    return -1;
        }
    }
    
    FILE* fp = fopen(trace_file, "r");

    char line[MAX_SIZE]; 
    while (fgets(line, sizeof(line), fp))
    {
       printf("%s\n", line);  
    }  
    // read trace line by line
    partition_trace();
    // split address (tag and set bits)
    partition_address();
    // check for hit or miss (update cache accordingly)
    // note: will use random cache replacement policy
    // print result
    printSummary(0, 0, 0);
    return 0;
}

void partition_trace(char* line)
{

}

void partition_address(char* addr)
{
    
}

void set_selection(int set_idx)
{
}

void line_matching()
{
}

void calc_tag_bits()
{
}


void printSummary(int hit_count, int miss_count, int eviction_count)
{

}
