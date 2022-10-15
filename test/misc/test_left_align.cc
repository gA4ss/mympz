#include <iostream>
#include <ctype.h>

#define CALC_MASK        (0xffffffffffffffffL)
#define UNIT_BITS        64
#define UNIT_ULONG       uint64_t

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

int main(int argc, char* argv[]) {
  uint64_t num[2], onum[2];
  num[0] = 0xFFFFFFFFFFFFFFFF; num[1] = 0xFFFFFFF;

  size_t l = 2;
  size_t rshift = num_bits_word(num[l-1]);   // 统计最高位的数值位数

  size_t lshift = UNIT_BITS - rshift;        // 最高位左边还空余的位数
  rshift %= UNIT_BITS;
  uint64_t rmask = 0 - rshift;  /* rmask = 0 - (rshift != 0) */
  rmask |= rmask >> 8;

  uint64_t n = 0, m = 0;
  for (size_t i = 0; i < l; i++) {
    n = num[i];
    onum[i] = ((n << lshift) | m) & CALC_MASK;
    m = (n >> rshift) & rmask;
  }

  for (size_t i = 0; i < l; i++) {
    std::cout << "num[" << i << "] = " << std::hex << num[i] << std::endl;
  }
  std::cout << std::endl;
  for (size_t i = 0; i < l; i++) {
    std::cout << "onum[" << i << "] = " << std::hex << onum[i] << std::endl;
  }

  return 0;
}