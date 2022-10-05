#include <iostream>
#include <ctype.h>

typedef uint64_t unit_t;

#define CALC_MASK (0xffffffffffffffffL)
#define CALC_MASKl (0x00000000ffffffffL)
#define CALC_MASKh (0xffffffff00000000L)
#define CALC_MASKh1 (0xffffffff80000000L)

#define UNIT_BITS 64
#define UNIT_ULONG uint64_t
#define UNIT_BYTES 8
#define UNIT_BITS 64
#define UNIT_HALF_BITS 32
#define UNIT_BITS2 (UNIT_BYTES * 8)
// #define UNIT_BITS        (UNIT_BITS2 * 2)
#define UNIT_TBIT ((UNIT_ULONG)1 << (UNIT_BITS2 - 1))

size_t num_bits_word(UNIT_ULONG l)
{
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

unit_t __div_unit(unit_t h, unit_t l, unit_t d)
{
  unit_t dh, dl, q, ret = 0, th, tl, t;
  int count = 2;

  if (d == 0)
    return CALC_MASK;

  // 统计除数的位数
  size_t i = num_bits_word(d);
  // my_assert((i == UNIT_BITS) || (h <= (unit_t)(1 << i)), "invalid word bits, %lu.", i);

  // 计算得到比除数多出来的位
  i = UNIT_BITS - i;

  //
  // 高位大于或者等于除数则减去一个除数
  // 原因就是
  //
  if (h >= d)
    h -= d;

  //
  // 如果除数不足一个字，则进行对齐
  // 除数左移到最高位
  // 被除数的高位也移动到本身字的最高位，低位随着移动对齐。
  //
  if (i)
  {
    d <<= i;
    h = (h << i) | (l >> (UNIT_BITS - i));
    l <<= i;
  }

  // 取出除数的高位与低位
  dh = (d & CALC_MASKh) >> UNIT_HALF_BITS;
  dl = (d & CALC_MASKl);

  // 进行实际除法操作
  for (;;)
  {
    // 计算试商
    // 如果被除数的高位的半字与除数的高位相等，则直接得出一个商0xffffffff，
    // 当乘以除数时直接给给除数左移动了32位。
    // 如果不相等，则计算高位与除数高位半字的商q。
    //
    if ((h >> UNIT_HALF_BITS) == dh)
      q = CALC_MASKl;
    else
      q = h / dh;

    // 试商乘以除数，用来后判断商的正确性。
    th = q * dh;
    tl = dl * q;

    //
    // 使用尝试来得到正确的商，这里使用了余数小于除数的判断法则来进行判定。
    //
    for (;;)
    {
      // 使用余数小于除数的法则判断商是否正确
      t = h - th;
      if ((t & CALC_MASKh) || ((tl) <= ((t << UNIT_HALF_BITS) |
                                        ((l & CALC_MASKh) >> UNIT_HALF_BITS))))
        break;

      // 商减1
      q--;
      th -= dh;
      tl -= dl;
    }

    //
    // 这里是使用之前的商与除数的积构造下一轮的被除数
    //
    t = (tl >> UNIT_HALF_BITS);
    tl = (tl << UNIT_HALF_BITS) & CALC_MASKh;
    th += t;

    // 被除数减去上一轮的商乘以除数得到新一轮的被除数
    if (l < tl)
      th++;
    l -= tl;
    if (h < th)
    {
      h += d;
      q--;
    }
    h -= th;

    // 两个字除以一个字，最多计算两次。
    if (--count == 0)
      break;

    //
    // 位移商到最终的位置上
    // 重新移动下一轮的被除数的高位与低位到最高位上
    //
    ret = q << UNIT_HALF_BITS;
    h = ((h << UNIT_HALF_BITS) | (l >> UNIT_HALF_BITS)) & CALC_MASK;
    l = (l & CALC_MASKl) << UNIT_HALF_BITS;
  }
  ret |= q;
  return ret;
}

int main(int argc, char *argv[])
{
  // 0xFFFFBBBBCCCCDDDDAABB112233445566
  unit_t xl = 0x5566778811223344, xh = 0xFFEEDDCCAABBCCDD;
  unit_t y = 0xFFEEDDCCBBAA1122;
  // unit_t xl = 125, xh = 4456;
  // unit_t y = 8;
  unit_t q = __div_unit(xh, xl, y);
  std::cout << "q = " << std::hex << q << std::endl;
  return 0;
}