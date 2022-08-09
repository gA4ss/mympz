#include <mympz/mympz.h>
#include "__internal.h"

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
  int r_neg, cmp_res;

  mympz_check_size(x);
  mympz_check_size(y);

  if (x.neg == y.neg) {
    r_neg = x.neg;
    z = uadd(x, y);
  } else {
    cmp_res = ucmp(x, y);
    if (cmp_res > 0) {
      r_neg = x.neg;
      z = usub(x, y);
    } else if (cmp_res < 0) {
      r_neg = y.neg;
      z = usub(y, x);
    } else {
      r_neg = 0;
    }
  }

  z.neg = r_neg;
  return z;
}

/**
  * @brief         有符号的两个大数相减法。
  * @param[in]     x
  * @param[in]     y
  * @return        x - y

  * @par Sample
  * @code
  * bignum_t x = create("19831210");
  * bignum_t y = create("19930613");
  * bignum_t z = sub(x, y);
  * std::cout << print_string(z) << std::endl;
  * @endcode
  */
bignum_t sub(const bignum_t& x, const bignum_t& y) {
  bignum_t z;
  int r_neg, cmp_res;

  mympz_check_size(x);
  mympz_check_size(y);

  if (x.neg != y.neg) {
    r_neg = x.neg;
    z = uadd(x, y);
  } else {
    cmp_res = ucmp(x, y);
    if (cmp_res > 0) {
      r_neg = x.neg;
      z = usub(x, y);
    } else if (cmp_res < 0) {
      r_neg = !y.neg;
      z = usub(y, x);
    } else {
      r_neg = 0;
    }
  }

  z.neg = r_neg;
  return z;
}

/**
  * @brief         无符号的两个大数相加。
  * @param[in]     x
  * @param[in]     y
  * @return        x + y

  * @par Sample
  * @code
  * bignum_t x = create("-19831210");
  * bignum_t y = create("19930613");
  * bignum_t z = uadd(x, y);
  * std::cout << print_string(z) << std::endl;
  * @endcode
  */
bignum_t uadd(const bignum_t& x, const bignum_t& y) {
  bignum_t z;

  mympz_check_size(x);
  mympz_check_size(y);

  //
  // 交换两个数
  //
  bignum_t& _x = bn_const_cast(x);
  bignum_t& _y = bn_const_cast(y);
  if (bn_size(_x) < bn_size(_y)) {
    bignum_t& tmp = _x;
    _x = _y;
    _y = tmp;
  }
  size_t max = bn_size(_x), min = bn_size(_y);
  size_t dif = max - min;

  bn_resize(z, max + 1);

  //
  // 先将min位相加
  //
  unit_t carry = __add_units(bn_ptr(z), bn_ptr(_x), bn_ptr(_y), min);

  size_t i = min;
  unit_t t1, t2;
  while (dif) {
    dif--;
    t1 = _x.number[i];
    t2 = (t1 + carry) & CALC_MASK;
    z.number[i] = t2;
    carry &= (t2 == 0);
    i++;
  }

  if (carry)
    z.number[i] = carry;

  z.neg = 0;
  clear_head_zero(z);
  return z;
}

/**
  * @brief         无符号的两个大数相加。
  * @param[in]     x
  * @param[in]     y
  * @note          x 必须大于 y 否则 产生异常。
  * @return        x - y

  * @par Sample
  * @code
  * bignum_t x = create("-19831210");
  * bignum_t y = create("19930613");
  * bignum_t z = usub(x, y);
  * std::cout << print_string(z) << std::endl;
  * @endcode
  */
bignum_t usub(const bignum_t& x, const bignum_t& y) {
  bignum_t z;
  bignum_t& _x = bn_const_cast(x);
  bignum_t& _y = bn_const_cast(y);

  mympz_check_size(_x);
  mympz_check_size(_y);

  size_t max = bn_size(_x);
  size_t min = bn_size(_y);
  size_t dif = max - min;

  if (static_cast<int>(dif) < 0) {
    mympz_exception("x(%s) < y(%s)", 
                    print_string(_x).c_str(), 
                    print_string(_y).c_str()
    );
  }

  bn_resize(z, max);

  unit_t borrow = __sub_units(bn_ptr(z), bn_ptr(_x), bn_ptr(_y), min);
  size_t i = min;

  unit_t t1, t2;
  while (dif) {
    dif--;
    t1 = _x.number[i];
    t2 = (t1 - borrow) & CALC_MASK;
    z.number[i] = t2;
    borrow &= (t1 == 0);
    i++;
  }

  i = max-1;
  while (max && z.number[i--] == 0)
    max--;
  bn_resize(z, max);
  
  if (!max) z.number.push_back(0);

  z.neg = 0;
  return z;
}

} // namespace mympz