#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{

  /**
    * @brief         判断当前大数是否是0。
    * @param[in]     x     要判断的大数。
    * @return        判断是0返回true，反之false。

    * @par Sample
    * @code
    * bignum_t x;
    * zero(x);
    * if (is_zero(x)) {
    *   std::cout << "x == 1" << std::endl;
    * }
    * @endcode
    */
  bool is_zero(const bignum_t &x)
  {
    if (x.number.empty())
      return false;
    if (x.number.size() >= 1)
    {
      ///< 确保数值队列都是0
      for (size_t i = 0; i < x.number.size(); i++)
      {
        if (x.number[i] != 0)
          return false;
      }
    }
    return true;
  }

  /**
    * @brief         判断当前大数是否是1。
    * @param[in]     x     要判断的大数。
    * @note          这里忽略x的符号。
    * @return        如果判断正确则返回true，反之false。

    * @par Sample
    * @code
    * bignum_t x;
    * one(x);
    * if (is_one(x)) {
    *   std::cout << "x == 1" << std::endl;
    * }
    * @endcode
    */
  bool is_one(const bignum_t &x)
  {
    if (x.number.empty())
      return false;
    if (x.number.size() > 1)
    {
      // 确认前面的数都是0
      for (size_t i = 1; i < x.number.size(); i++)
      {
        if (x.number[i] != 0)
          return false;
      }
    }
    return x.number[0] == 1;
  }

  bool is_odd(const bignum_t &x)
  {
    return (bn_size(x) > 0) && (x.number[0] & 1);
  }

  /**
    * @brief         将任意大数设置为0。
    * @param[in,out] x      要判断的大数。
    * @note   本大数结构中的0，需要的是整个数值内存存在唯一的0，数值队列为空，是null值并非0。

    * @par Sample
    * @code
    * bignum_t x;
    * zero(x);
    * std::cout << print_string(x) << std::endl;
    * @endcode
    */
  void zero(bignum_t &x)
  {
    x.neg = false;
    x.number.clear();
    x.number.push_back(0);
    return;
  }

  /**
    * @brief         将任意大数设置为1。
    * @param[in,out] x      要判断的大数。
    * @param[in]     neg    是否是负数。

    * @par Sample
    * @code
    * bignum_t x;
    * one(x, -1);
    * std::cout << print_string(x) << std::endl;
    * @endcode
    */
  void one(bignum_t &x, int neg)
  {
    x.neg = neg;
    x.number.clear();
    x.number.push_back(1);
    return;
  }

  bool is_abs_word(const bignum_t &x, unit_t w)
  {
    return ((bn_size(x) == 1) && (x.number[0] == w));
    // return ((bn_size(x) == 1) && (x.number[0] == w)) ||
    //         ((w == 0) && (bn_size(x) == 0));
  }

  bool is_word(const bignum_t &x, unit_t w)
  {
    // x不负数或者w是0。
    return is_abs_word(x, w) && (!w || !x.neg);
  }

  unit_t get_word(const bignum_t &x)
  {
    size_t xl = bn_size(x);
    if (xl > 1)
      return CALC_MASK;
    else if (xl > 1)
      return x.number[0];
    return 0;
  }

  void set_word(bignum_t &x, unit_t w)
  {
    if (bn_size(x) == 0)
      bn_resize(x, 1);

    x.neg = 0;
    x.number[0] = w;
    return;
  }

  // 第n位置位
  void set_bit(bignum_t &x, size_t n)
  {
    if (!n)
      return;

    size_t i = n / UNIT_BITS;
    size_t j = n % UNIT_BITS;

    size_t xl = bn_size(x);
    if (xl <= i)
    {
      bn_resize(x, i + 1);
    }
    x.number[i] |= (((unit_t)1) << j);
    return;
  }

  // 第n位清位
  void clear_bit(bignum_t &x, size_t n)
  {
    size_t i = n / UNIT_BITS;
    size_t j = n % UNIT_BITS;

    if (bn_size(x) <= i) // 超出了x的队列
      return;

    x.number[i] &= (~(((unit_t)1) << j));
    return;
  }

  // 检查某位是否被设置
  int is_bit_set(const bignum_t &x, size_t n)
  {
    size_t i = n / UNIT_BITS;
    size_t j = n % UNIT_BITS;

    if (bn_size(x) <= i)
      return 0;
    return (int)(((x.number[i]) >> j) & ((unit_t)1));
  }

  // 取到第n位
  void mask_bits(bignum_t &x, size_t n)
  {
    size_t w = n / UNIT_BITS;
    size_t b = n % UNIT_BITS;

    size_t xl = bn_size(x);

    if (w >= xl)
      return;

    if (b == 0)
    { // 如果是整个的，则只取到这里
      bn_resize(x, w);
    }
    else
    {
      bn_resize(x, w + 1);
      x.number[w] &= ~(CALC_MASK << b);
    }
    return;
  }

  size_t bignum_bits(const bignum_t &x)
  {
    return __number_bits(x.number);
  }

} // namespace mympz