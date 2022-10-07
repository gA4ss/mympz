#if defined(DISABLE_OPTIMIZE)
/* 用h,l除以d并返回结果 */
unit_t __div_unit(unit_t h, unit_t l, unit_t d)
{
  unit_t dh, dl, q, ret = 0, th, tl, t;
  int count = 2;

  if (d == 0)
    return CALC_MASK;

  // 统计除数的位数
  size_t i = __count_bits(d);
  my_assert((i == UNIT_BITS) || (h <= (unit_t)(1 << i)), "invalid word bits, %lu.", i);

  // 计算得到比除数多出来的位
  i = UNIT_BITS - i;

  //
  // 高位大于或者等于除数则减去一个除数
  //
  if (h >= d)
    h -= d;

  //
  // 如果除数不足一个字，则进行对齐
  // 除数左移到最高位
  // 被除数的高位也移动到本身字的最高位，低位随着移动对齐。
  //
  if (i)
  {
    d <<= i;
    h = (h << i) | (l >> (UNIT_BITS - i));
    l <<= i;
  }

  // 取出除数的高位与低位
  dh = (d & CALC_MASKh) >> UNIT_HALF_BITS;
  dl = (d & CALC_MASKl);

  // 进行实际除法操作
  for (;;)
  {
    // 计算试商
    // 如果被除数的高位的半字与除数的高位相等，则直接得出一个商0xffffffff，
    // 当乘以除数时直接给给除数左移动了32位。
    // 如果不相等，则计算高位与除数高位半字的商q。
    //
    if ((h >> UNIT_HALF_BITS) == dh)
      q = CALC_MASKl;
    else
      q = h / dh;

    // 试商乘以除数，用来后判断商的正确性。
    th = q * dh;
    tl = dl * q;

    //
    // 使用尝试来得到正确的商，这里使用了余数小于除数的判断法则来进行判定。
    //  
    for (;;)
    {
      //
      // 使用余数小于除数的法则判断商是否正确
      // h - th 计算余数t
      //
      t = h - th;

      // t是64位除以32位的结果，肯定在32位以内，所以判断t的高位是否为0，如果不为零则说明
      // h - th溢出了，是负数。
      if ((t & CALC_MASKh) || ((tl) <= ((t << UNIT_HALF_BITS) |
                                        ((l & CALC_MASKh) >> UNIT_HALF_BITS))))
        break;
      
      // 商减1
      q--;
      th -= dh;
      tl -= dl;
    }

    //
    // 这里是使用之前的商与除数的积构造下一轮的被除数
    //
    t = (tl >> UNIT_HALF_BITS);
    tl = (tl << UNIT_HALF_BITS) & CALC_MASKh;
    th += t;

    if (l < tl)
      th++;
    l -= tl;
    if (h < th)
    {
      h += d;
      q--;
    }
    h -= th;

    // 两个字除以一个字，最多计算两次。
    if (--count == 0)
      break;

    //
    // 位移商到最终的位置上
    // 重新移动下一轮的被除数的高位与低位到最高位上
    //
    ret = q << UNIT_HALF_BITS;
    h = ((h << UNIT_HALF_BITS) | (l >> UNIT_HALF_BITS)) & CALC_MASK;
    l = (l & CALC_MASKl) << UNIT_HALF_BITS;
  }
  ret |= q;
  return ret;
}
#undef REMAINDER_IS_ALREADY_CALCULATED
#endif

number_t __div_units_unit(const number_ptr_t &x, size_t xl, unit_t w, unit_t *r)
{
  number_t y;
  num_resize(y, xl);
  unit_t l, d, rem = 0;

  for (int i = static_cast<int>(xl) - 1; i >= 0; i--)
  {
    l = x[i];
    d = __div_unit(rem, l, w);
#ifndef REMAINDER_IS_ALREADY_CALCULATED
    rem = (l - ((d * w) & CALC_MASK)) & CALC_MASK;
#endif
    y[i] = d;
  }

  if (y[xl - 1] == 0)
    num_resize(y, xl - 1);

  if (r)
    *r = rem;

  return y;
}

//
// 交叉相乘
// 结果的低位保存在"l"，高位保存在"h"。
//
#define __mul64_in_div(l, h, bl, bh) \
  {                                  \
    UNIT_ULONG m, m1, lt, ht;        \
    lt = l;                          \
    ht = h;                          \
    m = (bh) * (lt);                 \
    lt = (bl) * (lt);                \
    m1 = (bl) * (ht);                \
    ht = (bh) * (ht);                \
    m = (m + m1) & CALC_MASK;        \
    if (m < m1)                      \
      ht += L2HBITS((UNIT_ULONG)1);  \
    ht += HBITS(m);                  \
    m1 = L2HBITS(m);                 \
    lt = (lt + m1) & CALC_MASK;      \
    if (lt < m1)                     \
      ht++;                          \
    (l) = lt;                        \
    (h) = ht;                        \
  }

/* 保证 xl > yl
 * 余数信息保存在x中
 */
number_t __div_units(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl)
{
  number_t z;
  my_assert(xl > yl, "xl less than yl. xl = %lu, yl = %lu", xl, yl);

  //
  // 保证了xl比yl大1位，同时也保证了后面
  // 的窗口变量wnumtop在某个时刻n0 == d0
  // 时保持为0
  //
  size_t loop = xl - yl;

  //
  // 初始化
  //

  number_ptr_t wnum = x + loop;        // 被除数比除数多余的部分
  number_ptr_t wnumtop = x + (xl - 1); // 被除数

  // 获取除数的头两位
  unit_t d0 = y[yl - 1];
  unit_t d1 = (yl == 1) ? 0 : y[yl - 2];

  // 设置商
  num_resize(z, loop);
  number_ptr_t zp = z.begin() + loop;

  // 临时变量申请
  number_t tmp;
  num_resize(tmp, yl + 1);

  // 进入循环
  for (size_t i = 0; i < loop; i++, wnumtop--)
  {
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
    if (n0 == d0)
    {
      //
      // 除数与被除数的第一个字如果相同，
      //
      q = CALC_MASK;
    }
    else
    { /* n0 < d0 */
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
        __mul64_in_div(t2l, t2h, ql, qh); /* t2=d1*q; */
      }

      //
      // 第五个阶段 检查余数
      //
      for (;;)
      {
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
      } /* end for */
    }   /* end else */

    //
    // (u_{j+n}u_{j+n-1}...u_j) - q'(0v_{n-1}...v_1v_0)
    //
    l0 = __mul_units_unit(tmp.begin(), y, yl, q);
    tmp[yl] = l0;
    wnum--;

    l0 = __sub_units(wnum, wnum, tmp.begin(), yl + 1);
    q -= l0;

    //
    // 用前两个字计算的试商，除数乘以试商也许会大于被除数的部分，但是试商减一乘以
    // 除数必然小于等于被除数的部分。
    //
    l0 = 0 - l0; // 生成借位掩码
    for (size_t j = 0; j < yl; j++)
      tmp[j] = y[j] & l0;
    l0 = __add_units(wnum, wnum, tmp.begin(), yl);
    (*wnumtop) += l0;
    my_assert((*wnumtop) == 0, "top number is not zero, top = %lu.", (*wnumtop));

    /* 保存部分结果 */
    *--zp = q;
  }
  return z;
}
