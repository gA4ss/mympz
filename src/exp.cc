#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         有符号的两个大数的模数
  * @param[in]     x
  * @param[in]     y
  * @return        r = x^p
  */
bignum_t exp(const bignum_t& x, const bignum_t& p) {
  size_t bits = __number_bits(p.number);

  bignum_t r;
  if (is_odd(p)) {
    r = x;
  } else {
    one(r);
  }

  bignum_t v = x;
  for (size_t i = 1; i < bits; i++) {
    v = sqr(v);

    // 检查指数第i位是否被置位
    if (is_bit_set(p, i)) {
      r = mul(r, v);
    }
  }
  return r;
}

} // namespace mympz