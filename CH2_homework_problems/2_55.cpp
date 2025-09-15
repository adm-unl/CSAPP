#include <iostream>
#include <iomanip>

typedef unsigned char* byte_pointer;


void show_bytes(byte_pointer start, size_t len) {
  int i;
  for (i = 0; i < len; i++) 
    std::cout << " "
	      << std::setw(2) << std::setfill('0') 
	      << std::hex << static_cast<int>(start[i]); 
  std::cout << std::endl;
}
 

void show_int(int x) {
  show_bytes((byte_pointer) &x, sizeof(int));
}


void show_float(float x) {
  show_bytes((byte_pointer) &x, sizeof(float));
}


void show_pointer(void* x) {
  show_bytes((byte_pointer) &x, sizeof(void*));
}


void test_show_bytes(int val) {
  int ival = val;
  float fval = (float) ival;
  int* pval = &ival;
  show_int(ival);
  show_float(fval);
  show_pointer(pval);
}


int main() {
  int x = 10;
  test_show_bytes(x);
}

