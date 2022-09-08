#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         一个大数与一个字相除。
  * @param[in]     x 大数
  * @param[in]     w 一个字
  * @param[in]     wneg w的符号
  * @param[out]    r 输出余数，这里余数并非真正的余数。全部是按照正数计算的余数。
  * @return        余数与商
  */
bignum_t div(const bignum_t& x, unit_t w, int wneg, unit_t* r) {
  bignum_t y; unit_t rm = 0;
  size_t xl = bn_size(x);

  w &= CALC_MASK;
  if (!w)
    mympz_exception("%s", "division is zero.")

  //
  // 处理一个特殊情况
  //
  if (xl == 1) {
    bn_resize(y, 1);
    y.number[0] = x.number[0] / w;
    rm = x.number[0] % w;
  } else {
    // 规范化数据
    // size_t j = UNIT_BITS - __count_bits(w);
    // w <<= j;
    // bignum_t _x = lshift(x, j);
    bignum_t _x = x;
    bn_resize(y, xl);
    for (int i = static_cast<int>(xl) - 1; i >= 0; i--) {
      unit_t l, d;

      l = _x.number[i];
      d = __div_unit(rm, l, w);
      rm = (l - ((d * w) & CALC_MASK)) & CALC_MASK;
      y.number[i] = d;
    }

    if (y.number[xl - 1] == 0)
      bn_resize(y, xl - 1);
  }

  //
  // 余数与符号
  //
  // rm >>= j;
  if (x.neg ^ wneg) {
    bignum_t o; one(o);
    y = add(y, o);
    y.neg = 1;

    bignum_t rr = create(rm);
    rr = sub(y, rr);
    rm = rr.number[0];
    if (r) *r = rm;
  } else {
    y.neg = 0;
  }

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
  * @brief         有符号的两个大数相除，这里不处理符号问题。
  * @param[in]     x 大数
  * @param[in]     y 大数
  * @return        余数与商
  */
division_result_t __div(const bignum_t& x, const bignum_t& y) {
  if (is_zero(y)) {
    mympz_exception("%s.", "divisor is zero");
  }

  //
  // 阶段1
  // 对被除数与除数进行标准化，使之达到$y_{n-1} >= \floor{b/2}$的要求
  // 这样保证了 q'-2 <= q <= q'
  //

  // 左对其除数
  bignum_t _y = bn_const_cast(y);
  size_t norm_shift = __left_align(_y.number);
  _y.neg = 0;

  // 左对齐被除数
  bignum_t _x = lshift(x, norm_shift);

  size_t yl = bn_size(_y), xl = bn_size(_x);

  //
  // 被除数的长度小于等于除数则扩展被除数
  // 保证被除数为n+1位，除数为n位。
  // 同时这里也保证了后面的窗口变量
  // wnumtop在某个时刻n0 == d0时保持为0
  //
  if (xl <= yl) {
    xl = yl + 1;
    bn_resize(_x, xl);
  }
  size_t loop = xl - yl;

  //
  // 阶段2
  // 初始化
  //

  number_ptr_t wnum = bn_ptr2(_x, loop);        // 被除数比除数多余的部分
  number_ptr_t wnumtop = bn_ptr2(_x, xl - 1);   // 被除数

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

  // 进入循环
  for (size_t i = 0; i < loop; i++, wnumtop--) {
    unit_t q, l0;
    unit_t n0, n1, rem = 0;

    n0 = wnumtop[0];
    n1 = wnumtop[-1];
    //
    // 这里计算的q' = floor(u_{j+n}b+u_{j+n-1} / v_{n-1})
    // 所以如果u_{j+n} == v_{n-1},必然意味着q' > b
    // 所以这里直接取b。
    //
    // 依照
    // q' = min(floor(u_{j+n}b+u_{j+n-1} / v_{n-1}), b)
    //
    if (n0 == d0) {
      //
      // 除数与被除数的第一个字如果相同，
      //
      q = CALC_MASK;
    } else {                  /* n0 < d0 */
      // 需要产生借位
      unit_t n2 = (wnumtop == wnum) ? 0 : wnumtop[-2];
      unit_t t2l, t2h;

      // 这里产生尝商
      q = __div_unit(n0, n1, d0);
#ifndef REMAINDER_IS_ALREADY_CALCULATED
      // 余数计算
      rem = (n1 - q * d0) & CALC_MASK;
#endif

      {
        //
        // 用试商乘以被除数的前两位的后一位
        // 这里用来判断是否结束循环
        //
        unit_t ql, qh;
        t2l = LBITS(d1);
        t2h = HBITS(d1);
        ql = LBITS(q);
        qh = HBITS(q);
        mul64(t2l, t2h, ql, qh); /* t2=d1*q; */
      }

      //
      // 第五个阶段 检查余数
      //
      for (;;) {
        //
        // q'v_{n-2} > br'+u_{j+n-2}
        //
        if ((t2h < rem) || ((t2h == rem) && (t2l <= n2)))
          break;

        //
        // 试商减1，试余加上被除数前两位的第一位
        // 商减少1，那么就意味着余数多了一个被除数
        //
        q--;
        rem += d0;

        //
        // 防止余数溢出
        //
        if (rem < d0)
          break;

        //
        // t2 = t2 - d1
        //
        if (t2l < d1)
          t2h--;
        t2l -= d1;
      }/* end for */
    }/* end else */

    //
    // (u_{j+n}u_{j+n-1}...u_j) - q'(0v_{n-1}...v_1v_0)
    //
    l0 = __mul_units_unit(bn_ptr(tmp), bn_ptr(_y), yl, q);
    tmp.number[yl] = l0;
    wnum--;

    l0 = __sub_units(wnum, wnum, bn_ptr(tmp), yl + 1);
    q -= l0;

    //
    // 用前两个字计算的试商，除数乘以试商也许会大于被除数的部分，但是试商减一乘以
    // 除数必然小于等于被除数的部分。
    //
    l0 = 0 - l0;      // 生成借位掩码
    for (size_t j = 0; j < yl; j++)
      tmp.number[j] = _y.number[j] & l0;
    l0 = __add_units(wnum, wnum, bn_ptr(tmp), yl);
    (*wnumtop) += l0;
    my_assert((*wnumtop) == 0, "top number is not zero, top = %lu.", (*wnumtop));

    /* 保存部分结果 */
    *--zp = q;
  }

  //
  // 计算余数
  //
  bignum_t rm = rshift(_x, norm_shift);

  clear_head_zero(_z);
  clear_head_zero(rm);
  return division_result_t(_z, rm);
}

/**
  * @brief         有符号的两个大数相除
  * @param[in]     x 大数
  * @param[in]     y 大数
  * @return        余数与商
  */
division_result_t div(const bignum_t& x, const bignum_t& y) {
  division_result_t res;
  //
  // 这里如果判断y如果只有1个字，则转向使用大数与字的除法。
  //
  if (bn_size(y) == 1) {
    unit_t r = 0;
    bignum_t q = div(x, y.number[0], y.neg, &r);
    bignum_t rm = create(r);
    res = {q, rm};
  } else {
    res = __div(x, y);

    //  异号
    if (x.neg ^ y.neg) {
      bignum_t o; one(o);
      res.first = add(res.first, o);
      res.first.neg = 1;

      res.second = sub(y, res.second);
    } else {
      res.first.neg = 0;
      res.second.neg = y.neg;
    }
  }

  return res;
}

bignum_t idiv(const bignum_t& x, const bignum_t& y) {
  return div(x,y).first;
}

} // namespace mympz