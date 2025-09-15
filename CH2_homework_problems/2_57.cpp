#include <iostream>
#include <iomanip>
#include <climits>


typedef unsigned char* byte_pointer;


void show_bytes(byte_pointer start, size_t len) {
  int i;
  for (i = 0; i < len; i++) 
    std::cout << " "
	      << std::setw(2) << std::setfill('0') 
	      << std::hex << static_cast<unsigned int>(start[i]); 
  std::cout << std::endl;
}
 
void show_short(short x) {
  show_bytes((byte_pointer) &x, sizeof(short));
}

void show_long(long x) {
  show_bytes((byte_pointer) &x, sizeof(long));
}

void show_double(double x) {
  show_bytes((byte_pointer) &x, sizeof(double));
}


void test_show_bytes(int val) {
  short sval = (short) val;
  long lval = (long) val;
  double dval = (double) val;
  show_short(sval);
  show_long(lval);
  show_double(dval);
}


int main() {
  int min = INT_MIN;
  int max = INT_MAX;
  int zero = 0;
  int neg = -100;
  int pos = 100;
  int vals[] = {min, max, zero, neg, pos};
  for (int val : vals) {
    std::cout << "Val: " << std::dec << val << std::endl;
    test_show_bytes(val);
    std::cout << std::endl;
  }
}

