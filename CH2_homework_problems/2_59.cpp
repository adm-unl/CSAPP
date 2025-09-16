#include <iostream>

int main() {
  int x = 0x89abcdef;
  int y = 0x76543210;

  int bit_mask = 0xFF;
  int res = (y & ~bit_mask) | (x & bit_mask);
  std::cout << std::hex << res << std::endl;
}
