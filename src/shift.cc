#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         左移一位
  * @param[in]     x
  * @return        左移一位后的结果
  */
bignum_t lshift1(const bignum_t& x) {
  bignum_t y;
  y.neg = x.neg;
  y.number = __lshift1(bn_ptr(bn_const_cast(x)), bn_size(x));
  return y;
}

/**
  * @brief         右移一位
  * @param[in]     x
  * @return        右移一位后的结果
  */
bignum_t rshift1(const bignum_t& x) {
  bignum_t y;
  y.neg = x.neg;
  y.number = __rshift1(bn_ptr(bn_const_cast(x)), bn_size(x));
  return y;
}

/**
  * @brief         左移n位
  * @param[in]     x 要移动的数
  * @param[in]     n 要移动的位数
  * @return        左移n位后的结果
  */
bignum_t lshift(const bignum_t& x, size_t n) {
  bignum_t y;
  y.neg = x.neg;
  y.number = __lshift(bn_ptr(bn_const_cast(x)), bn_size(x), n);
  return y;
}

/**
  * @brief         右移n位
  * @param[in]     x 要移动的数
  * @param[in]     n 要移动的位数
  * @return        右移n位后的结果
  */
bignum_t rshift(const bignum_t& x, size_t n) {
  bignum_t y;
  y.neg = x.neg;
  y.number = __rshift(bn_ptr(bn_const_cast(x)), bn_size(x), n);
  return y;
}

} // namespace mympz