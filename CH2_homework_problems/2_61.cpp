#include <cassert>
#include <iostream>

bool A(int x) {
  return !!x;
}

bool B(int x) {
  return !!~x;
}

bool C(int x) {
  int shift_val = (sizeof(int) - 1) << 3;
  return !!(x << shift_val);
}

bool D(int x) {
  int shift_val = (sizeof(int) -1) << 3;
  return !!(~x << shift_val);
}

int main() {
  int all_ones = -1;
  int all_zeros = 0;

  assert(A(all_ones));
  assert(!A(all_zeros));

  assert(!B(all_ones));
  assert(B(all_zeros));
  
  assert(C(all_ones));
  assert(!C(all_zeros));

  assert(!D(all_ones));
  assert(D(all_zeros));
}
