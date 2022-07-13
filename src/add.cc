#include <mympz/mympz.h>

namespace mympz {

/**
  * @brief         有符号的两个大数相加，将y加到x上。
  * @param[in]     x
  * @param[in]     y
  * @return        x + y

  * @par Sample
  * @code
  * bignum_t x = create("-19831210");
  * bignum_t y = create("19930613");
  * bignum_t z = add(x, y);
  * std::cout << print_string(z) << std::endl;
  * @endcode
  */
bignum_t add(const bignum_t& x, const bignum_t& y) {
  bignum_t z;

  // int ret, r_neg, cmp_res;

  // if (x.sign == y.sign) {
  //     r_neg = x.sign;
  //     z = uadd(r, a, b);
  // } else {
  //     cmp_res = BN_ucmp(a, b);
  //     if (cmp_res > 0) {
  //         r_neg = a->neg;
  //         ret = BN_usub(r, a, b);
  //     } else if (cmp_res < 0) {
  //         r_neg = b->neg;
  //         ret = BN_usub(r, b, a);
  //     } else {
  //         r_neg = 0;
  //         BN_zero(r);
  //         ret = 1;
  //     }
  // }

  // r->neg = r_neg;
  // bn_check_top(r);
  // return ret;

  return z;
}

bignum_t sub(const bignum_t& x, const bignum_t& y) {
  bignum_t z;
  return z;
}

bignum_t uadd(const bignum_t& x, const bignum_t& y) {
  bignum_t _x = x, _y = y, z;
  if (size(_x) < size(_y)) {
    bignum_t tmp = _x;
    _x = _y;
    _y = tmp;
  }

  size_t max = size(_x);
  size_t min = size(_y);
  size_t dif = max - min;

  z.resize(max + 1);

  const number_t& xn = _x.number;
  const number_t& yn = _y.number;
  number_t& zn = z.number;

  unit_t carry = bn_add_words(rp, ap, bp, min);
  rp += min;
  ap += min;

  while (dif) {
      dif--;
      t1 = *(ap++);
      t2 = (t1 + carry) & BN_MASK2;
      *(rp++) = t2;
      carry &= (t2 == 0);
  }
  *rp = carry;
  r->top += carry;

  r->neg = 0;
  bn_check_top(r);
  return 1;

  return z;
}

bignum_t usub(const bignum_t& x, const bignum_t& y) {
  bignum_t z;
  return z;
}

} // namespace mympz