#include <iostream>

bool is_little_endian() {
    short x = 1;
    unsigned char* p = (unsigned char*) &x;
    return *p == 1; 
}

int main() {
    bool little_endian = is_little_endian();
    std::cout << little_endian << std::endl;
    return 0;
}
