#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

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

  //
  // 如果任意一个参数为0，则返回空值。
  //
  int xl = static_cast<int>(bn_size(x)), yl = static_cast<int>(bn_size(y));
  if ((xl == 0) || (yl == 0)) {
    return z;
  }
  
  int i = static_cast<int>(bn_size(x)) - static_cast<int>(bn_size(y));
  if (i == 0) {
    //
    // x与y的长度相同
    //
    if (xl == 4) {
      bn_resize(z, 8);
      __mul_4_units(z.number, x.number, y.number);
    } else if (xl == 8) {
      bn_resize(z, 16);
      __mul_8_units(z.number, x.number, y.number);
    }
  } else if ((xl >= CALC_MULL_SIZE_NORMAL) && (yl >= CALC_MULL_SIZE_NORMAL)) {
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
      my_assert(j <= xl || j <= yl);
      k = j + j;

      if (xl > j || yl > j) {
        bn_resize(t, k*4);
        bn_resize(z, k*4);
        __mul_part_recursive(z.number, x.number, y.number,
                             j, xl - j, yl - j, t.number);
      } else {  // xl <= j || yl <= j
        bn_resize(t, k*2);
        bn_resize(z, k*2);
        __mul_recursive(z.number, x.number, y.number,
                        j, xl - j, yl - j, t.number);
      }
    }
  } else {
    //
    // 小于CALC_MULL_SIZE_NORMAL位数则直接使用循环来处理
    //
    bn_resize(z, xl + yl);
    __mul_units_loop(z.number, x.number, y.number);
  }

  z.neg = x.neg ^ y.neg;    // 设定符号
  clear_head_zero(z);
  return z;
}


} // namespace mympz