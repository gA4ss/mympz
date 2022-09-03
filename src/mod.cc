#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         有符号的两个大数的模数
  * @param[in]     x
  * @param[in]     y
  * @return        r = x % y
  */
bignum_t mod(const bignum_t& x, const bignum_t& y) {
  division_result_t z = div(x, y);
  return z.second;
}

/**
  * @brief         有符号的两个大数的模数取绝对值
  * @param[in]     x
  * @param[in]     y
  * @return        r = |x % y|
  * @note 与mod运算一致，但是返回非负的余数。
  *       $0 <= r < |y|$
  */
bignum_t nnmod(const bignum_t& x, const bignum_t& y) {
  bignum_t r = mod(x, y);
  if (!r.neg)   // 如果为正数
    return r;
  /* 现在 -|y| < r < 0, 因此我们需要设置 r = r + |y| */
  return (y.neg ? sub : add) (r, y);
}

/**
  * @brief         有符号的两个大数的模数相加取模后取绝对值
  * @param[in]     x
  * @param[in]     y
  * @return        r = |(x + y) % m|
  */
bignum_t mod_add(const bignum_t& x, const bignum_t& y, const bignum_t& m) {
  bignum_t z;
  // x,y > 0 && x,y <= m
  if ((cmp(x, m) <= 0) && (cmp(y, m) <= 0) &&
      (x.neg == 0) && (y.neg == 0))
  {
    bignum_t _x = bn_const_cast(x);
    bignum_t _y = bn_const_cast(y);
    bignum_t _m = bn_const_cast(m);
    z.number = __mod_add(bn_ptr(_x), bn_size(_x), bn_ptr(_y), bn_size(_y),
                         bn_ptr(_m), bn_size(_m));
    z.neg = 0;
    return z;
  }
  z = add(x, y);
  return nnmod(z, m);
}

/**
  * @brief         有符号的两个大数的模数相减取模后取绝对值
  * @param[in]     x
  * @param[in]     y
  * @return        r = |(x - y) % m|
  */
bignum_t mod_sub(const bignum_t& x, const bignum_t& y, const bignum_t& m) {
  bignum_t z;
  // x,y > 0 && x,y <= m
  if ((cmp(x, m) <= 0) && (cmp(y, m) <= 0) &&
      (x.neg == 0) && (y.neg == 0))
  {
    bignum_t _x = bn_const_cast(x);
    bignum_t _y = bn_const_cast(y);
    bignum_t _m = bn_const_cast(m);
    z.number = __mod_sub(bn_ptr(_x), bn_size(_x), bn_ptr(_y), bn_size(_y),
                         bn_ptr(_m), bn_size(_m));
    z.neg = 0;
    return z;
  }
  z = sub(x, y);
  return nnmod(z, m);
}

/**
  * @brief         有符号的两个大数的模数相乘取模后取绝对值
  * @param[in]     x
  * @param[in]     y
  * @return        r = |(x * y) % m|
  */
bignum_t mod_mul(const bignum_t& x, const bignum_t& y, const bignum_t& m) {
  bignum_t z;

  if (cmp(x, y) == 0) {
    z = sqr(x);
    return z;
  }

  z = mul(x, y);
  return nnmod(z, m);
}

/**
  * @brief         大数的平方取模后取绝对值
  * @param[in]     x
  * @return        r = |(x * x) % m|
  */
bignum_t mod_sqr(const bignum_t& x, const bignum_t& m) {
  bignum_t y = sqr(x);
  return mod(y, m);
}

/**
  * @brief         大数的左移1位取模后取绝对值
  * @param[in]     x
  * @return        r = |(x << 1) % m|
  */
bignum_t mod_lshift1(const bignum_t& x, const bignum_t& m) {
  bignum_t r = lshift1(x);
  if (cmp(r, m) >= 0) {
    return sub(r, m);
  }
  return nnmod(r, m);
}

/**
  * @brief         大数的左移n位取模后取绝对值
  * @param[in]     x x < m && x >= 0
  * @param[in]     n 要移动的位数
  * @return        r = |(x << n) % m|
  */
bignum_t __mod_lshift(const bignum_t& x, int n, const bignum_t& m) {
  bignum_t r = x;

  while (n > 0) {
    int max_shift;

    /* 0 < r < m */
    max_shift = static_cast<int>(__number_bits(m.number) - __number_bits(r.number));
    /* max_shift >= 0 */
    if (max_shift < 0) {
      mympz_exception("%s", "invalid argument : r > m");
    }

    if (max_shift > n)
      max_shift = n;

    if (max_shift) {
      r = lshift(r, max_shift);
      n -= max_shift;
    } else {
      r = lshift1(r);
      --n;
    }

    /* __bignum_bits(r) <= __bignum_bits(m) */
    if (cmp(r, m) >= 0) {
      r = sub(r, m);
    }
  }

  return r;
}

/**
  * @brief         大数的左移n位取模后取绝对值
  * @param[in]     x
  * @param[in]     n 要移动的位数
  * @return        r = |(x << n) % m|
  */
bignum_t mod_lshift(const bignum_t& x, size_t n, const bignum_t& m) {
  bignum_t r, abs_m;

  r = nnmod(x, m);

  // 如果模数为负数
  if (m.neg) {
    abs_m = m;
    abs_m.neg = 0;
  }

  return __mod_lshift(r, static_cast<int>(n), (m.neg ? abs_m : m));
}

} // namespace mympz