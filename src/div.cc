#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{

  /**
   * @brief         一个大数与一个字相除。
   * @param[in]     x 大数
   * @param[in]     w 一个字
   * @param[in]     wneg w的符号
   * @param[out]    r 输出余数，这里余数并非真正的余数。全部是按照正数计算的余数。
   * @return        余数与商
   */
  bignum_t div(const bignum_t &x, unit_t w, int wneg, unit_t *r)
  {
    bignum_t y;
    unit_t rm = 0;
    size_t xl = bn_size(x);

    w &= CALC_MASK;
    if (!w)
    {
      mympz_divide_by_zero(w);
    }

    //
    // 处理一个特殊情况
    //
    if (xl == 1)
    {
      bn_resize(y, 1);
      y.number[0] = x.number[0] / w;
      rm = x.number[0] % w;
    }
    else
    {
      // 规范化数据
      // size_t j = UNIT_BITS - __count_bits(w);
      // w <<= j;
      // bignum_t _x = lshift(x, j);
      bignum_t _x = bn_const_cast(x);
      y.number = __div_units_unit(bn_ptr(_x), xl, w, &rm);
    }

    //
    // 余数与符号
    //
    // rm >>= j;
    if (x.neg ^ wneg)
    {
      bignum_t o;
      one(o);
      y = add(y, o);
      y.neg = 1;

      bignum_t rr = create(rm);
      rr = sub(y, rr);
      rm = rr.number[0];
    }
    else
    {
      y.neg = 0;
    }
    if (r)
    {
      *r = rm;
    }

    clear_head_zero(y);
    return y;
  }

  /**
   * @brief         有符号的两个大数相除，这里不处理符号问题。
   * @param[in]     x 大数
   * @param[in]     y 大数
   * @return        余数与商
   */
  division_result_t __div(const bignum_t &x, const bignum_t &y)
  {
    mympz_dbgprint_fmt_div("x = %s.\n", __print_string(x.number, true).c_str());
    mympz_dbgprint_fmt_div("y = %s.\n", __print_string(y.number, true).c_str());

    if (is_zero(y))
    {
      mympz_exception("%s.", "divisor is zero");
    }

    // 初始化商
    bignum_t z;
    init_bignum(z);
    z.neg = x.neg ^ y.neg;

    //
    // 对被除数与除数进行标准化，使之达到$y_{n-1} >= \floor{b/2}$的要求
    // 这样保证了 q'-2 <= q <= q'
    //

    //
    // 左对其除数
    // 左对齐被除数
    //
    bignum_t _y = bn_const_cast(y);
    size_t norm_shift = __left_align(bn_ptr(_y), bn_size(_y));
    _y.neg = 0;
    mympz_dbgprint_fmt_div("normalize shift = %lu.\n", norm_shift);

    bignum_t _x = lshift(x, norm_shift);
    //
    // 当norm_shift为0时，需要多分配一个空字节，后面的__div_units
    // 函数实现有这样做的需求。
    //
    if (!norm_shift)
    {
      bn_resize(_x, bn_size(_x) + 1);
    }
    size_t yl = bn_size(_y), xl = bn_size(_x);
    mympz_dbgprint_fmt_div("normalized x = %s.\n", __print_string(_x.number, true).c_str());
    mympz_dbgprint_fmt_div("normalized y = %s.\n", __print_string(_y.number, true).c_str());

    //
    // 被除数的长度小于等于除数则扩展被除数
    // 保证被除数为n+1位，除数为n位。
    //
    if (xl <= yl)
    {
      xl = yl + 1;
      bn_resize(_x, xl);
    }

    // 进行除法
    z.number = __div_units(bn_ptr(_x), xl, bn_ptr(_y), yl);

    //
    // 计算余数
    //
    bignum_t r = rshift(_x, norm_shift);
    mympz_dbgprint_fmt_div("remove normalized r = %s.\n", __print_string(r.number, true).c_str());

    clear_head_zero(z);
    clear_head_zero(r);
    return division_result_t(z, r);
  }

  /**
   * @brief         有符号的两个大数相除
   * @param[in]     x 大数
   * @param[in]     y 大数
   * @return        余数与商
   */
  division_result_t div(const bignum_t &x, const bignum_t &y)
  {
    division_result_t res;
    //
    // 这里如果判断y如果只有1个字，则转向使用大数与字的除法。
    //
    if (bn_size(y) == 1)
    {
      unit_t r = 0;
      bignum_t q = div(x, y.number[0], y.neg, &r);
      bignum_t rm = create(r);
      res = {q, rm};
    }
    else
    {
      res = __div(x, y);

      //  异号
      if (x.neg ^ y.neg)
      {
        bignum_t o;
        one(o);
        res.first = add(res.first, o);
        res.first.neg = 1;

        res.second = sub(y, res.second);
      }
      else
      {
        res.first.neg = 0;
        res.second.neg = y.neg;
      }
    }

    return res;
  }

  bignum_t idiv(const bignum_t &x, const bignum_t &y)
  {
    return div(x, y).first;
  }

} // namespace mympz