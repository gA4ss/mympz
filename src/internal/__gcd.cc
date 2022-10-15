/* 此函数基于Bernstein和Yang的恒定时间GCD工作:
 * https://eprint.iacr.org/2019/266
 * 广义快速GCD函数，允许均匀输入。
 * 算法首先找到2的共有幂
 * 输入，并删除它们，减少至少一个输入奇数值。然后继续计算GCD。
 * 在返回结果GCD之前，我们会注意添加把一开始的2的幂去掉。
 * 注1:我们假设两个输入的比特长度都是公共信息，因为访问top可能会泄露这些信息。
 */
number_t __gcd(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl)
{
  mympz_dbgprint_fmt_gcd("x = %s.\n", __print_string_hex(x, xl).c_str());
  mympz_dbgprint_fmt_gcd("xl = %lu.\n", xl);
  mympz_dbgprint_fmt_gcd("y = %s.\n", __print_string_hex(y, yl).c_str());
  mympz_dbgprint_fmt_gcd("yl = %lu.\n", yl);

  int xneg = 0, yneg = 0;
  unit_t mask = 0, bit = 1, delta = 1, cond = 0, shifts = 0;
  number_t z;

  if (__is_zero(y, yl))
  {
    num_resize(z, xl);
    for (size_t i = 0; i < xl; i++)
      z[i] = x[i];
    return z;
  }

  if (__is_zero(x, xl))
  {
    num_resize(z, yl);
    for (size_t i = 0; i < yl; i++)
      z[i] = y[i];
    return z;
  }

  //
  // 使得 r != 0, g != 0，保证左移的有效性。
  //
  mympz_dbgprint_gcd("lshift 1 bits.\n");
  number_t r = __lshift1(x, xl);
  xl = num_size(r);

  number_t g = __lshift1(y, yl);
  yl = num_size(g);

  mympz_dbgprint_fmt_gcd("r = %s.\n", __print_string_hex(num_ptr(r), num_size(r)).c_str());
  mympz_dbgprint_fmt_gcd("g = %s.\n", __print_string_hex(num_ptr(g), num_size(g)).c_str());

  //
  // 寻找共同的2次幂
  //
  mympz_dbgprint_gcd("find common power of 2.\n");
  for (size_t i = 0; i < num_size(r) && i < num_size(g); i++)
  {
    mask = ~(r[i] | g[i]);
    for (size_t j = 0; j < UNIT_BITS2; j++)
    {
      bit &= mask;
      shifts += bit;
      mask >>= 1;
    }
  }
  mympz_dbgprint_fmt_gcd("mask = %lx.\n", mask);
  mympz_dbgprint_fmt_gcd("bit = %lu.\n", bit);

  // 减去共同的2次幂
  mympz_dbgprint_fmt_gcd("rshift %lu bits.\n", shifts);
  r = __rshift(num_ptr(r), num_size(r), shifts);
  xl = num_size(r);
  g = __rshift(num_ptr(g), num_size(g), shifts);
  yl = num_size(g);
  mympz_dbgprint_fmt_gcd("r = %s.\n", __print_string_hex(num_ptr(r), num_size(r)).c_str());
  mympz_dbgprint_fmt_gcd("g = %s.\n", __print_string_hex(num_ptr(g), num_size(g)).c_str());

  /* 计算迭代数 */
  mympz_dbgprint_gcd("compute iteration count.\n");
  size_t rlen = __number_bits(r);
  size_t glen = __number_bits(g);
  size_t m = 4 + 3 * ((rlen >= glen) ? rlen : glen);
  mympz_dbgprint_fmt_gcd("r bits = %lu.\n", rlen);
  mympz_dbgprint_fmt_gcd("g bits = %lu.\n", glen);
  mympz_dbgprint_fmt_gcd("m = %lu.\n", m);

  // 扩展到最大的n个字，为可能的额外单字留有空间
  size_t top = 1 + ((num_size(r) >= num_size(g)) ? num_size(r) : num_size(g));
  mympz_dbgprint_fmt_gcd("expand %lu words.\n", top);
  num_resize(r, top);
  num_resize(g, top);

  // 重新排列输入，r是计数
  mympz_dbgprint_fmt_gcd("rearrange words on condition %lu.\n", (~r[0]) & 1);
  __consttime_swap((~r[0]) & 1,
                   num_ptr(r), &xl, &xneg,
                   num_ptr(g), &yl, &yneg,
                   top);

  number_t t;
  size_t tl = 0;
  int tneg = 0;
  for (size_t i = 0; i < m; i++)
  {
    // 有条件地翻转符号，如果delta是正的并且g是奇数。
    cond = (-delta >> (8 * UNIT_HALF_BYTES - 1)) & g[0] & 1
           /* 确保 g.size() > 0 (例如：如果 top == 0 那么 g == 0) */
           & (~((yl - 1) >> (UNIT_HALF_BYTES * 8 - 1)));
    delta = (-cond & -delta) | ((cond - 1) & delta);
    xneg ^= cond; // 反转符号

    /* 交换 */
    __consttime_swap(cond,
                     num_ptr(r), &xl, &xneg,
                     num_ptr(g), &yl, &yneg,
                     top);

    mympz_dbgprint_fmt_gcd("[%lu] cond = %lu, delta = %lx.\n",
                           i, cond, delta);

    /* 消除步骤 */
    delta++;
    // number_t t; num_resize(t, top);
    {
      mympz_dbgprint_fmt_gcd("[%lu] t = g + r.\n", i);
      mympz_dbgprint_fmt_gcd("[%lu] rneg = %d, gneg = %d\n", i, xneg, yneg);
      if (xneg ^ yneg)
      {
        if (__cmp(num_ptr(g), num_ptr(r), top) >= 0)
        {
          t = __sub_units(num_ptr(g), top, num_ptr(r), top);
          tneg = yneg;
        }
        else
        {
          t = __sub_units(num_ptr(r), top, num_ptr(g), top);
          tneg = xneg;
        }
      }
      else
      {
        t = __add_units(num_ptr(g), top, num_ptr(r), top);
        tneg = xneg;
      }
    }

    // 如果是0则设置为正号
    if (__is_zero(num_ptr(t), top))
      tneg = 0;

    // 计算t的最高位数
    {
      size_t l = top;
      while (l && t[l - 1] == 0)
        --l;
      tl = l == 0 ? 1 : l;
    }
    num_resize(t, top);
    mympz_dbgprint_fmt_gcd("[%lu] sign = \'%c\', t = %s.\n", i, tneg ? '-' : '+',
                            __print_string_hex(num_ptr(t), num_size(t)).c_str());
    mympz_dbgprint_fmt_gcd("[%lu] sign = \'%c\', g = %s.\n", i, yneg ? '-' : '+',
                            __print_string_hex(num_ptr(g), num_size(g)).c_str());
    mympz_dbgprint_fmt_gcd("[%lu] sign = \'%c\', r = %s.\n", i, xneg ? '-' : '+',
                            __print_string_hex(num_ptr(r), num_size(r)).c_str());

    // g是奇数
    // 确保 g.size() > 0 (例如：如果 top == 0 那么 g == 0)
    __consttime_swap(g[0] & 1 & (~((yl - 1) >> (UNIT_HALF_BYTES * 8 - 1))),
                     num_ptr(g), &yl, &yneg,
                     num_ptr(t), &tl, &tneg,
                     top);

    g = __rshift1(num_ptr(g), yl);
    num_resize(g, top);
    mympz_dbgprint_fmt_gcd("[%lu] g rshift 1 bits.\n",
                           i, __print_string_hex(num_ptr(g), num_size(g)).c_str());
  }

  // 加上去除2的幂，然后修正人工移位
  r = __lshift(num_ptr(r), num_size(r), shifts);
  mympz_dbgprint_fmt_gcd("r lshift %lu bits = %s.\n",
                         shifts, __print_string_hex(num_ptr(r), num_size(r)).c_str());
  r = __rshift1(num_ptr(r), num_size(r));
  mympz_dbgprint_fmt_gcd("r rshift 1 bits = %s.\n",
                         __print_string_hex(num_ptr(r), num_size(r)).c_str());
  return r;
}
