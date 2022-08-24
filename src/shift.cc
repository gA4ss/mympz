#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         左移一位
  * @param[in]     x
  * @return        左移一位后的结果
  */
bignum_t lshift1(const bignum_t& x) {
  bignum_t y; init_bignum(y);
  register unit_t t = 0, c = 0;

  size_t l = bn_size(x);
  bn_resize(y, l + 1);

  number_ptr_t p = y.number.begin();
  for (size_t i = 0; i < l; i++) {
    t = x.number[i];
    *(p++) = ((t << 1) | c) & CALC_MASK;
    c = t >> (UNIT_BITS - 1);
  }
  *p = c;

  clear_head_zero(y);
  return y;
}

/**
  * @brief         右移一位
  * @param[in]     x
  * @return        右移一位后的结果
  */
bignum_t rshift1(const bignum_t& x) {
  bignum_t y; init_bignum(y);
  register unit_t t = 0, c = 0;

  if (is_zero(x)) {
    zero(y);
    return y;
  }

  size_t l = bn_size(x);
  bn_resize(y, l);

  // number_ptr_t p = y.number.begin();
  int i = static_cast<int>(l);

  t = x.number[--i];
  y.number[i] = t >> 1;
  c = t << (UNIT_BITS - 1);

  while (i > 0) {
    t = x.number[--i];
    y.number[i] = ((t >> 1) & CALC_MASK) | c;
    c = t << (UNIT_BITS - 1);
  }

  clear_head_zero(y);
  return y;
}

/**
  * @brief         左移n位
  * @param[in]     x 要移动的数
  * @param[in]     n 要移动的位数
  * @return        左移n位后的结果
  */
bignum_t lshift(const bignum_t& x, size_t n) {
  bignum_t y; init_bignum(y);

  if (n == 0) {
    return x;
  }

  size_t nw = n / UNIT_BITS;        // 有多少个64位
  size_t xl = bn_size(x);
  bn_resize(y, xl + nw + 1);        // 扩展到移动后的位数

  unit_t lb = static_cast<unit_t>(n) % UNIT_BITS;
  unit_t rb = UNIT_BITS - lb;
  rb %= UNIT_BITS;
  unit_t rmask = 0 - rb;                   // rmask = 0 - (rb != 0)
  rmask |= rmask >> 8;


  bignum_t& _x = bn_const_cast(x);
  number_ptr_t f = bn_ptr(_x);
  number_ptr_t t = bn_ptr2(y, nw);

  unit_t l = f[xl - 1];
  t[xl] = (l >> rb) & rmask;
  unit_t m = 0;
  for (int i = static_cast<int>(xl)- 1; i > 0; i--) {
    m = l << lb;
    l = f[i - 1];
    t[i] = (m | ((l >> rb) & rmask)) & CALC_MASK;
  }
  t[0] = (l << lb) & CALC_MASK;

  clear_head_zero(y);
  return y;
}

/**
  * @brief         右移n位
  * @param[in]     x 要移动的数
  * @param[in]     n 要移动的位数
  * @return        右移n位后的结果
  */
bignum_t rshift(const bignum_t& x, size_t n) {
  bignum_t y; init_bignum(y);
  if (n == 0) {
    return x;
  }

  size_t xl = bn_size(x);
  size_t nw = n / UNIT_BITS;
  if (nw >= xl) {
    zero(y);
    return y;
  }

  unit_t rb = n % UNIT_BITS;
  unit_t lb = UNIT_BITS - rb;
  lb %= UNIT_BITS;
  unit_t mask = 0 - lb;   /* mask = 0 - (lb != 0) */
  mask |= mask >> 8;
  size_t top = xl - nw;
  bn_resize(y, top);

  bignum_t& _y = bn_const_cast(y);
  number_ptr_t t = bn_ptr(_y);

  bignum_t& _x = bn_const_cast(x);
  number_ptr_t f = bn_ptr2(_x, nw);
  unit_t l = f[0], m = 0;
  size_t i = 0;
  for (i = 0; i < top - 1; i++) {
    m = f[i + 1];
    t[i] = (l >> rb) | ((m << lb) & mask);
    l = m;
  }
  t[i] = l >> rb;

  clear_head_zero(y);
  return y;
}

} // namespace mympz