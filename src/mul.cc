#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

bignum_t mul(const bignum_t& x, unit_t w) {
  bignum_t y; init_bignum(y);
  w &= CALC_MASK;
  size_t xl = bn_size(x);
  if (xl) {
    if (w == 0)
      zero(y);
    else {
      bignum_t _x = bn_const_cast(x);
      bn_resize(y, xl);
      unit_t ll = __mul_units_unit(bn_ptr(y), bn_ptr(_x), xl, w);
      if (ll) {
        bn_resize(y, xl + 1);
        y.number[xl] = ll;
      }
      y.neg = x.neg;
    }
  }

  return y;
}

/**
  * @brief         有符号的两个大数相乘法
  * @param[in]     x
  * @param[in]     y
  * @return        x * y

  * @par Sample
  * @code
  * bignum_t x = create("-19831210");
  * bignum_t y = create("19930613");
  * bignum_t z = mul(x, y);
  * std::cout << print_string(z) << std::endl;
  * @endcode
  */
bignum_t mul(const bignum_t& x, const bignum_t& y) {
  bignum_t z, t;
  bignum_t& _x = bn_const_cast(x);
  bignum_t& _y = bn_const_cast(y);

  init_bignum(z);init_bignum(t);

  //
  // 如果任意一个参数为0，则返回空值。
  //
  int xl = static_cast<int>(bn_size(_x)), yl = static_cast<int>(bn_size(_y));
  if ((xl == 0) || (yl == 0)) {
    set_error(z);
    return z;
  }
  
  int i = static_cast<int>(bn_size(_x)) - static_cast<int>(bn_size(_y));

#if 0
  if (i == 0) {
    //
    // x与y的长度相同
    //
    if (xl == 4) {
      bn_resize(z, 8);
      __mul_4_units(bn_ptr(z), bn_ptr(_x), bn_ptr(_y));
    } else if (xl == 8) {
      bn_resize(z, 16);
      __mul_8_units(bn_ptr(z), bn_ptr(_x), bn_ptr(_y));
    } else {
      bn_resize(z, xl + yl + 1);
      __mul_units_loop(bn_ptr(z), bn_ptr(_x), xl, bn_ptr(_y), yl);
    }
  } else 
#endif

  if ((xl >= CALC_MULL_SIZE_NORMAL) && (yl >= CALC_MULL_SIZE_NORMAL)) {
    //
    // 1.乘数与被乘数大于某个长度
    // 2.乘数与被乘数的长度仅差1位
    // 才使用分治算法
    //
    if (i >= -1 && i <= 1) {  // -1 <= i <= 1
      //
      // 求最小二的幂，或等于其中最长的两个数字。
      //
      int j = 0;   ///< 保存最大长度的最小二次幂
      if (i >= 0) {   // xl >= yl
        j = static_cast<int>(__count_bits(static_cast<unit_t>(xl)));
      } else if (i == -1) {  // xl < yl
        j = static_cast<int>(__count_bits(static_cast<unit_t>(yl)));
      }

      //
      // j,k这个组合相当于取了2的取整数，
      // 随后使用k的长度来保存乘法结果。
      //
      j = 1 << (j - 1);
      my_assert(j <= xl || j <= yl, "j > xl || j > yl,j = %l.", j);
      int k = j + j;

      if (xl > j || yl > j) {
        bn_resize(t, k*4);
        bn_resize(z, k*4);
        __mul_part_units_recursive(bn_ptr(z), bn_ptr(_x), bn_ptr(_y),
                                   j, xl - j, yl - j, bn_ptr(t));
      } else {  // xl <= j || yl <= j
        bn_resize(t, k*2);
        bn_resize(z, k*2);
        __mul_units_recursive(bn_ptr(z), bn_ptr(_x), bn_ptr(_y),
                              j, xl - j, yl - j, bn_ptr(t));
      }
    } else {
      bn_resize(z, xl + yl + 1);
      __mul_units_loop(bn_ptr(z), bn_ptr(_x), xl, bn_ptr(_y), yl);
    }
  } else {
    //
    // 小于CALC_MULL_SIZE_NORMAL位数则直接使用循环来处理
    //
    bn_resize(z, xl + yl + 1);
    __mul_units_loop(bn_ptr(z), bn_ptr(_x), xl, bn_ptr(_y), yl);
  }

  z.neg = x.neg ^ y.neg;    // 设定符号
  clear_head_zero(z);
  return z;
}

} // namespace mympz