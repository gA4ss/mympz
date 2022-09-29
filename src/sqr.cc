
#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         计算x的平方
  * @param[in]     x
  * @return        x * x
  */
bignum_t sqr(const bignum_t& x) {
  bignum_t y;

  bignum_t& _x = bn_const_cast(x);
  size_t xl = bn_size(_x);
  if (xl == 0) {
    return y;
  }

  size_t max = 2 * xl;
  bn_resize(y, max);

#if defined(CALC_SQR_COMBA)
  if (xl == 4) {
    __sqr_4_units(bn_ptr(y), bn_ptr(_x));
    goto _end;
  } else if (xl == 8) {
    __sqr_8_units(bn_ptr(y), bn_ptr(_x));
    goto _end;
  }
#endif

#if defined(CALC_RECURSION)
  if (xl < CALC_SQR_RECURSIVE_SIZE_NORMAL) {
    number_t t;
    num_resize(t, CALC_SQR_RECURSIVE_SIZE_NORMAL * 2);
    __sqr_units_loop(bn_ptr(y), bn_ptr(_x), xl, num_ptr(t));
  } else {
    size_t j = __count_bits(xl);
    j = 1 << (j - 1);
    size_t k = j + j;
    number_t t;
    if (xl == j) {
      num_resize(t, k * 2);
      __sqr_units_recursive(bn_ptr(y), bn_ptr(_x), xl, num_ptr(t));
    } else {
      num_resize(t, max);
      __sqr_units_loop(bn_ptr(y), bn_ptr(_x), xl, num_ptr(t));
    }
  }/* end else */
#else
  number_t t; num_resize(t, max);
  __sqr_units_loop(bn_ptr(y), bn_ptr(_x), xl, num_ptr(t));
#endif

_end:
  y.neg = 0;
  clear_head_zero(y);
  return y;
}

} // namespace mympz