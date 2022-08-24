/*
 * 这里此函数借鉴了Openssl的代码，在VS的编译器会之前会引发一个编译器优化
 * 错误，这里如果遇到微软的编译器则将其优化关闭。
 * 
 * https://mta.openssl.org/pipermail/openssl-users/2018-August/008465.html
 */
#if defined(_MSC_VER) && defined(_ARM_) && defined(_WIN32_WCE) \
    && _MSC_VER>=1400 && _MSC_VER<1501
# define MS_BROKEN_num_bits_word
# pragma optimize("", off)
#endif

/**
  * @brief         统计输入的数存在多少位
  * @param[in]     x
  * @return        x的二次幂
  */
size_t __count_bits(unit_t l) {
  unit_t x, mask;
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
#ifdef MS_BROKEN_BN_num_bits_word
# pragma optimize("", on)
#endif

