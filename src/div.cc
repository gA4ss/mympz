#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         一个大数与一个字相除。
  * @param[in]     x 大数
  * @param[in]     w 一个字
  * @param[out]    r 输出余数
  * @return        余数与商
  */
bignum_t div(const bignum_t& x, unit_t w, unit_t* r) {
  bignum_t y; y.neg = 0;
  size_t xl = bn_size(x);

  w &= CALC_MASK;
  if (!w)
    mympz_exception("%s", "division is zero.")
  if (xl == 0)
    return y;

  // 规范化数据
  size_t j = UNIT_BITS - __count_bits(w);
  w <<= j;
  bignum_t _x = lshift(x, j);

  bn_resize(y, xl);
  unit_t rm = 0;
  for (int i = static_cast<int>(xl) - 1; i >= 0; i--) {
    unit_t l, d;

    l = _x.number[i];
    d = __div_unit(rm, l, w);
    rm = (l - ((d * w) & CALC_MASK)) & CALC_MASK;
    y.number[i] = d;
  }

  if (y.number[xl - 1] == 0)
    bn_resize(y, xl - 1);

  // 求余数
  rm >>= j;
  if (r) *r = rm;
  y.neg = x.neg;
  return y;
}

static size_t __left_align(number_t& num) {
  size_t l = num.size();
  size_t rshift = __count_bits(num[l - 1]);   // 统计最高位的数值位数

  size_t lshift = UNIT_BITS - rshift;         // 最高位左边还空余的位数
  rshift %= UNIT_BITS;
  unit_t rmask = (unit_t)0 - rshift;          // rmask = 0 - (rshift != 0)
  rmask |= rmask >> 8;                        // rmask为CALC_MASK

  unit_t n = 0, m = 0;
  for (size_t i = 0; i < l; i++) {
    //
    // 先左移到需要得位然后或上上一个字节
    // 的右移位的部分。
    //
    n = num[i];
    num[i] = ((n << lshift) | m) & CALC_MASK;
    m = (n >> rshift) & rmask;
  }

  return lshift;
}

/**
  * @brief         有符号的两个大数想除。
  * @param[in]     x
  * @param[in]     y
  * @return        余数与商

  * @par Sample
  * @code
  * bignum_t x = create("112233445566");
  * bignum_t y = create("11223377");
  * division_result_t z = div(x, y);
  * std::cout << print_string(z.first) << " "
  *           << print_string(z.second) << std::endl;
  * @endcode
  */
division_result_t div(const bignum_t& x, const bignum_t& y) {
  if (is_zero(y)) {
    mympz_exception("%s.", "divisor is zero");
  }

  //
  // 阶段1
  // 对被除数与除数进行标准化
  //

  // 左对其除数
  bignum_t _y = bn_const_cast(y);
  size_t norm_shift = __left_align(_y.number);
  _y.neg = 0;

  // 左对齐被除数
  bignum_t _x = lshift(x, norm_shift);

  size_t yl = bn_size(_y), xl = bn_size(_x);

  // 被除数的长度小于等于除数则扩展被除数
  if (xl <= yl) {
    xl = yl + 1;
    bn_resize(_x, xl);
  }
  size_t loop = xl - yl;

  // 设置滑动变量
  number_ptr_t wnum = bn_ptr2(_x, loop);        // 被除数比除数多余的部分
  number_ptr_t wnumtop = bn_ptr2(_x, xl - 1);   // 被除数的最高位

  // 获取除数的头两位
  unit_t d0 = _y.number[yl - 1];
  unit_t d1 = (yl == 1) ? 0 : _y.number[yl - 2];

  // 设置商
  bignum_t _z; init_bignum(_z); bn_resize(_z, loop);
  _z.neg = x.neg ^ y.neg;
  number_ptr_t zp = bn_ptr2(_z, loop);

  // 临时变量申请
  bignum_t tmp; init_bignum(tmp);
  bn_resize(tmp, yl + 1);

  for (size_t i = 0; i < loop; i++, wnumtop--) {
    unit_t q, l0;
    unit_t n0, n1, rem = 0;

    n0 = wnumtop[0];
    n1 = wnumtop[-1];
    if (n0 == d0) {
      // 除数与被除数的第一个字如果相同。
      q = CALC_MASK;
    } else {                  /* n0 < d0 */
      // 需要产生借位
      unit_t n2 = (wnumtop == wnum) ? 0 : wnumtop[-2];
      unit_t t2l, t2h;

      // 这里n1其实就表示了借位
      q = __div_unit(n0, n1, d0);
#ifndef REMAINDER_IS_ALREADY_CALCULATED
      // 余数计算
      rem = (n1 - q * d0) & CALC_MASK;
#endif

      {
        unit_t ql, qh;
        t2l = LBITS(d1);
        t2h = HBITS(d1);
        ql = LBITS(q);
        qh = HBITS(q);
        mul64(t2l, t2h, ql, qh); /* t2=d1*q; */
      }

      for (;;) {
        if ((t2h < rem) || ((t2h == rem) && (t2l <= n2)))
          break;
        q--;
        rem += d0;
        if (rem < d0)
          break;      /* 防止余数溢出 */
        if (t2l < d1)
          t2h--;
        t2l -= d1;
      }/* end for */
    }/* end else */

    l0 = __mul_units_unit(bn_ptr(tmp), bn_ptr(_y), yl, q);
    tmp.number[yl] = l0;
    wnum--;

    /*
     * ignore top values of the bignums just sub the two BN_ULONG arrays
     * with bn_sub_words
     */
    l0 = __sub_units(wnum, wnum, bn_ptr(tmp), yl + 1);
    q -= l0;
    /*
     * Note: As we have considered only the leading two BN_ULONGs in
     * the calculation of q, sdiv * q might be greater than wnum (but
     * then (q-1) * sdiv is less or equal than wnum)
     */
    l0 = 0 - l0;      // 生成借位掩码
    for (size_t j = 0; j < yl; j++)
      tmp.number[j] = _y.number[j] & l0;
    l0 = __add_units(wnum, wnum, bn_ptr(tmp), yl);
    (*wnumtop) += l0;
    my_assert((*wnumtop) == 0, "top number is not zero, top = %lu.", (*wnumtop));

    /* 保存部分结果 */
    *--zp = q;
  }

  /* _x中保存了余数，与除数长度一致。 */
  _x.neg = x.neg;
  bignum_t rm = rshift(_x, norm_shift);

  clear_head_zero(_z);
  clear_head_zero(rm);
  return division_result_t(_z, rm);
}

bignum_t idiv(const bignum_t& x, const bignum_t& y) {
  return div(x,y).first;
}

} // namespace mympz