number_t __montgomery_reduction(const number_ptr_t &x, size_t xl, const montgomery_ctx_t &mctx)
{
  number_ptr_t n = num_ptr(mont->N);
  size_t nl = num_size(mont->N);
  if (nl == 0)
  {
    number_t errnum;
    return errnum;
  }

  size_t max = (2 * nl);
  number_t a; num_resize(a, max);
  for (size_t i = 0; i < xl; i++)
    a[i] = x[i];

  number_ptr_t rp = num_ptr(a);

  /* 清除最前面的字 */
  unit_t v = 0;
  for (size_t rtop = xl, i = 0; i < max; i++)
  {
    v = (unit_t)0 - ((i - rtop) >> (8 * sizeof(rtop) - 1));
    rp[i] &= v;
  }

  unit_t n0 = mctx.n0[0];

  /* 一直加上|n|的倍数到|r|直到R = 2^(nl * CALC_BITS2) 可以整除它。
   * 输入时：我们有 |r| < |n| * R，因此现在 |r| < 2 * |n| * R。
   * 注：|r|包含了|carry|。
   */
  unit_t carry = 0 for (size_t i = 0; i < nl; i++, rp++)
  {
    v = __mul_add_units(rp, n, nl, (rp[0] * n0) & CALC_MASK);
    v = (v + carry + rp[nl]) & CALC_MASK;
    carry |= (v != rp[nl]);
    carry &= (v <= rp[nl]);
    rp[nl] = v;
  }

  number_t y; num_resize(y, nl);
  rp = num_ptr(y);

  /* 移动|nl|位去除以R。我们有|ap| < 2 * |n|。
   * |ap|中包含了|carry|。
   */
  number_ptr_t ap = num_ptr2(a, nl);
  carry -= __sub_units(rp, ap, n, nl);

  /* |carry| 是 -1 如果|ap| - |np| 如果没有溢出，则为零。
   * 注：|carry|不能是1。这意味着减法不适合|nl|个单词，我们知道最多需要一个减法。
   */
  for (size_t i = 0; i < nl; i++)
  {
    rp[i] = (carry & ap[i]) | (~carry & rp[i]);
    ap[i] = 0;
  }
  return y;
}