#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         一个大数与一个字相加，将y加到x上。
  * @param[in]     x 大数
  * @param[in]     y 一个字
  * @return        x + y
  */
bignum_t add(const bignum_t& x, unit_t w, int wneg) {
  bignum_t y;
  w &= CALC_MASK;

  if (!w)
    return x;
  if (is_zero(x)) {
    set_word(y, w);
    return y;
  }

  // x为负数
  if (x.neg) {
    y = x;
    set_positive(y);
    y = sub(y, w, wneg);
    if (!is_zero(y))
      y.neg = !(y.neg);
    return y;
  }

  size_t xl = bn_size(x);
  unit_t l = 0;
  size_t i = 0;
  y = x;
  for (i = 0; w != 0 && i < xl; i++) {
    y.number[i] = l = (x.number[i] + w) & CALC_MASK;
    w = (w > l) ? 1 : 0;
  }

  // 最后存在进位
  if (w && i == xl) {
    bn_resize(y, xl + 1);
    y.number[i] = w;
  }

  return y;
}

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
  * @brief         一个有符号大数与一个字相减法。
  * @param[in]     x 大数
  * @param[in]     y 一个字
  * @return        x - y
  */
bignum_t sub(const bignum_t& x, unit_t w, int wneg) {
  bignum_t y;
  w &= CALC_MASK;

  if (!w)
    return x;
  if (is_zero(x)) {
    set_word(y, w);
    set_negative(y);
    return y;
  }
  
  if (x.neg) {
    y = x;
    set_positive(y);
    y = add(y, w, wneg);
    set_negative(y);
    return y;
  }

  size_t xl = bn_size(x);
  y = x;

  if ((xl == 1) && (x.number[0] < w)) {
    y.number[0] = w - x.number[0];
    set_negative(y);
    return y;
  }

  size_t i = 0;
  for (;;) {
    if (x.number[i] >= w) {
      y.number[i] = x.number[i] - w;
      break;
    } else {
      y.number[i] = (x.number[i] - w) & CALC_MASK;
      i++;
      w = 1;
    }
  }

  // 去掉末尾的0
  if ((y.number[i] == 0) && (i == (xl - 1)))
    bn_resize(y, i + 1);
  return y;
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

  // 进行运算
  z.number = __add_units(bn_ptr(_x), bn_size(_x), bn_ptr(_y), bn_size(_y));
  z.neg = 0;
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

  z.number = __sub_units(bn_ptr(_x), bn_size(_x), bn_ptr(_y), bn_size(_y));
  z.neg = 0;
  return z;
}

} // namespace mympz