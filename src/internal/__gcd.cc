/* 此函数基于Bernstein和Yang的恒定时间GCD工作:
 * https://eprint.iacr.org/2019/266
 * 广义快速GCD函数，允许均匀输入。
 * 算法首先找到2的共有幂
 * 输入，并删除它们，减少至少一个输入奇数值。然后继续计算GCD。
 *在返回结果GCD之前，我们会注意添加把一开始的2的幂去掉。
 *注1:我们假设两个输入的比特长度都是公共信息，因为访问top可能会泄露这些信息。
 */
number_t __gcd(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl)
{
  unit_t mask = 0;
  size_t bit = 1, delta = 1, cond = 0, shifts = 0;
  number_t z;

  if (__is_zero(y, yl)) {
    num_resize(z, xl);
    for (size_t i = 0; i < xl; i++)
      z[i] = x[i];
    return z;
  }

  if (__is_zero(x, xl)) {
    num_resize(z, yl);
    for (size_t i = 0; i < yl; i++)
      z[i] = y[i];
    return z;
  }

  //
  // 使得 r != 0, g != 0，保证左移的有效性。
  //
  number_t g = __lshift1(y, yl);
  number_t r = __lshift1(x, xl);

  //
  // 寻找共同的2次幂
  //
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

  // 减去共同的2次幂
  r = __rshift(num_ptr(r), num_size(r), shifts);
  g = __rshift(num_ptr(g), num_size(g), shifts);

  // 扩展到最大的n个字，为可能的额外单字留有空间
  size_t top = 1 + ((num_size(r) >= num_size(g)) ? num_size(r) : num_size(g));
  num_resize(r, top);
  num_resize(g, top);

  // 重新排列输入，r是计数
  __consttime_swap((~r[0]) & 1, num_ptr(r), num_ptr(g), top);

  /* 计算迭代数 */
  size_t rlen = __number_bits(r);
  size_t glen = __number_bits(g);
  size_t m = 4 + 3 * ((rlen >= glen) ? rlen : glen);

  for (size_t i = 0; i < m; i++)
  {
    // 有条件地翻转符号，如果delta是正的并且g是奇数。
    cond = (-delta >> (8 * UNIT_HALF_BYTES - 1)) & g[0] & 1
           /* 确保 g.size() > 0 (例如：如果 top == 0 那么 g == 0) */
           & (~((num_size(g) - 1) >> (UNIT_HALF_BYTES * 8 - 1)));
    delta = (-cond & -delta) | ((cond - 1) & delta);
    // r_neg ^= cond;   // 反转符号

    /* 交换 */
    __consttime_swap(cond, num_ptr(r), num_ptr(g), top);

    /* 消除步骤 */
    delta++;
    // number_t t; num_resize(t, top);
    number_t t;
    {
      if (num_size(g) >= num_size(r))
        t = __add_units(num_ptr(g), num_size(g), num_ptr(r), num_size(r));
      else
        t = __add_units(num_ptr(r), num_size(r), num_ptr(g), num_size(g));
    }

    // g是奇数
    // 确保 g.size() > 0 (例如：如果 top == 0 那么 g == 0)
    __consttime_swap(g[0] & 1 & (~((num_size(g) - 1) >> (UNIT_HALF_BYTES * 8 - 1))),
                     num_ptr(g), num_ptr(t), top);
    g = __rshift1(num_ptr(g), num_size(g));
  }

  // 移除可能的负号
  // r_neg = 0;

  // 加上去除2的幂，然后修正人工移位
  r = __lshift(num_ptr(r), num_size(r), shifts);
  r = __lshift1(num_ptr(r), num_size(r));

  clear_number_head_zero(r);
  return r;
}
