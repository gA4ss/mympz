#include <iostream>
#include <ctype.h>

typedef uint64_t         unit_t;

#define CALC_MASK        (0xffffffffffffffffL)
#define CALC_MASKl       (0x00000000ffffffffL)
#define CALC_MASKh       (0xffffffff00000000L)
#define CALC_MASKh1      (0xffffffff80000000L)

#define UNIT_BITS        64
#define UNIT_ULONG       uint64_t
#define UNIT_BYTES       8
#define UNIT_BITS        64
#define UNIT_HALF_BITS   32
#define UNIT_BITS2       (UNIT_BYTES * 8)
// #define UNIT_BITS        (UNIT_BITS2 * 2)
#define UNIT_TBIT        ((UNIT_ULONG)1 << (UNIT_BITS2 - 1))

size_t num_bits_word(UNIT_ULONG l) {
  UNIT_ULONG x, mask;
  size_t bits = (l != 0);

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

unit_t __div_unit(unit_t h, unit_t l, unit_t d) {
  unit_t dh, dl, q, ret = 0, th, tl, t;
  int count = 2;

  if (d == 0)
    return CALC_MASK;

  size_t i = num_bits_word(d);
  // my_assert((i == UNIT_BITS) || (h <= (unit_t)(1 << i)), "invalid word bits, %lu.", i);

  i = UNIT_BITS - i;
  if (h >= d)
    h -= d;

  if (i) {
    d <<= i;
    h = (h << i) | (l >> (UNIT_BITS - i));
    l <<= i;
  }
  dh = (d & CALC_MASKh) >> UNIT_HALF_BITS;
  dl = (d & CALC_MASKh);
  for (;;) {
    if ((h >> UNIT_HALF_BITS) == dh)
      q = CALC_MASKl;
    else
      q = h / dh;

    th = q * dh;
    tl = dl * q;
    for (;;) {
      t = h - th;
      if ((t & CALC_MASKh) || ((tl) <= ((t << UNIT_HALF_BITS) | 
          ((l & CALC_MASKh) >> UNIT_HALF_BITS))))
        break;
      q--;
      th -= dh;
      tl -= dl;
    }
    t = (tl >> UNIT_HALF_BITS);
    tl = (tl << UNIT_HALF_BITS) & CALC_MASKh;
    th += t;

    if (l < tl)
      th++;
    l -= tl;
    if (h < th) {
      h += d;
      q--;
    }
    h -= th;

    if (--count == 0)
      break;

    ret = q << UNIT_HALF_BITS;
    h = ((h << UNIT_HALF_BITS) | (l >> UNIT_HALF_BITS)) & CALC_MASK;
    l = (l & CALC_MASKl) << UNIT_HALF_BITS;
  }
  ret |= q;
  return ret;
}

int main(int argc, char* argv[]) {
  // 0xFFFFBBBBCCCCDDDDAABB112233445566
  unit_t xl = 0xFFEEDDCC11223344, xh = 0x55667788AABBCCDD;
  unit_t y = 0xEEFFCCDDBBAA1122;
  unit_t q = __div_unit(xh, xl, y);
  std::cout << "q = " << std::hex << q << std::endl;
  return 0;
}