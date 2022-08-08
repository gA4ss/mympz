#include <iostream>
#include <ctype.h>

#define CALC_MASK        (0xffffffffffffffffL)
#define UNIT_BITS        64
#define UNIT_ULONG       unsigned long

size_t num_bits_word(UNIT_ULONG l) {
  UNIT_ULONG x, mask;
  size_t bits = (l != 0);

  //
  // 
  //
#if UNIT_BITS > 32
  x = l >> 32;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 32 & mask;
  l ^= (x ^ l) & mask;
#endif

  x = l >> 16;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 16 & mask;
  l ^= (x ^ l) & mask;

  x = l >> 8;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 8 & mask;
  l ^= (x ^ l) & mask;

  x = l >> 4;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 4 & mask;
  l ^= (x ^ l) & mask;

  x = l >> 2;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 2 & mask;
  l ^= (x ^ l) & mask;

  x = l >> 1;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 1 & mask;

  return bits;
}

#include <cmath>
int main(int argc, char* argv[]) {
  size_t b = 0;
  for (int i = 0; i < 64; i+=2) {
    UNIT_ULONG l = std::pow(2, i);
    b = num_bits_word(l);
    std::cout << l << " " << "b = " << b << std::endl;
  }
  std::cout << std::endl;

  for (int i = 0; i < 32; i+=2) {
    UNIT_ULONG l = std::pow(2, i) + 3;
    b = num_bits_word(l);
    std::cout << l << " " << "b = " << b << std::endl;
  }
  return 0;
}