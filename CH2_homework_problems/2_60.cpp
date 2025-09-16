#include <iostream> 

unsigned replace_byte(unsigned x, int i, unsigned char b) {
  unsigned new_b = b << (i << 3);
  unsigned bit_mask = 0xFF << (i << 3);
  unsigned res = (x & ~bit_mask) | new_b;
  return res; 
}

int main() {
  unsigned x = 0x12345678;
  unsigned char b = 0xAB;
  int i;

  i = 0;
  unsigned res1 = replace_byte(x, i, b);
  i = 2;
  unsigned res2 = replace_byte(x, i, b);

  std::cout << std::hex << res1 << std::endl;
  std::cout << std::hex << res2 << std::endl;

}



