number_t __mod(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl)
{
  if (__is_zero(y, yl))
  {
    mympz_divide_by_zero(y);
  }

  //
  // 对被除数与除数进行标准化，使之达到$y_{n-1} >= \floor{b/2}$的要求
  // 这样保证了 q'-2 <= q <= q'
  //

  //
  // 左对其除数
  // 左对齐被除数
  //
  number_t _y;
  num_resize(_y, yl);
  for (size_t i = 0; i < yl; i++)
    _y[i] = y[i];

  size_t norm_shift = __left_align(num_ptr(_y), yl);

  //
  // 当norm_shift为0时，会多分配一个空字节，后面的__div_units
  // 函数实现有这样做的需求。
  //
  number_t _x = __lshift(x, xl, norm_shift);
  xl = num_size(_x);

  //
  // 被除数的长度小于等于除数则扩展被除数
  // 保证被除数为n+1位，除数为n位。
  //
  if (xl <= yl)
  {
    xl = yl + 1;
    num_resize(_x, xl);
  }

  // 进行除法
  __div_units(num_ptr(_x), xl, num_ptr(_y), yl);

  //
  // 计算余数
  //
  number_t r = __rshift(num_ptr(_x), xl, norm_shift);
  return r;
}

/**
 * @brief         有符号的两个大数的模数相加取模后取绝对值
 * @param[in]     x x >=0 && x < m
 * @param[in]     y y >=0 && y < m
 * @return        r = (x + y) % |m|
 */
number_t __mod_add(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl,
                   const number_ptr_t &m, size_t ml)
{
  number_t storage;
  num_resize(storage, 1024 / UNIT_BITS);
  if (ml > num_size(storage))
  {
    num_resize(storage, ml);
  }
  number_ptr_t tp = num_ptr(storage);
  number_t r;
  num_resize(r, ml);

  unit_t carry = 0, temp = 0, mask = 0;
  for (size_t i = 0, xi = 0, yi = 0; i < ml;)
  {
    mask = (unit_t)0 - ((i - xl) >> (8 * sizeof(i) - 1));
    temp = ((x[xi] & mask) + carry) & CALC_MASK;
    carry = (temp < carry);

    mask = (unit_t)0 - ((i - yl) >> (8 * sizeof(i) - 1));
    tp[i] = ((y[yi] & mask) + temp) & CALC_MASK;
    carry += (tp[i] < temp);

    i++;
    xi += (i - xl) >> (8 * sizeof(i) - 1);
    yi += (i - yl) >> (8 * sizeof(i) - 1);
  }

  number_ptr_t rp = num_ptr(r);
  carry -= __sub_units(rp, tp, m, ml);
  for (size_t i = 0; i < ml; i++)
  {
    rp[i] = (carry & tp[i]) | (~carry & rp[i]);
    tp[i] = 0;
  }

  return r;
}

/**
 * @brief         有符号的两个大数的模数相减取模后取绝对值
 * @param[in]     x x >=0 && x < m
 * @param[in]     y y >=0 && y < m
 * @return        r = (x - y) % |m|
 */
number_t __mod_sub(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl,
                   const number_ptr_t &m, size_t ml)
{
  number_t r;
  num_resize(r, ml);

  unit_t tx, ty;
  unit_t borrow = 0, carry = 0, mask = 0;
  for (size_t i = 0, xi = 0, yi = 0; i < ml;)
  {
    mask = (unit_t)0 - ((i - xl) >> (8 * sizeof(i) - 1));
    tx = x[xi] & mask;

    mask = (unit_t)0 - ((i - yl) >> (8 * sizeof(i) - 1));
    ty = y[yi] & mask;
    r[i] = tx - ty - borrow;
    if (tx != ty)
      borrow = (tx < ty);

    i++;
    xi += (i - xl) >> (8 * sizeof(i) - 1);
    yi += (i - yl) >> (8 * sizeof(i) - 1);
  }

  mask = 0 - borrow;
  carry = 0;
  for (size_t i = 0; i < ml; i++)
  {
    tx = ((m[i] & mask) + carry) & CALC_MASK;
    carry = (tx < carry);
    r[i] = (r[i] + tx) & CALC_MASK;
    carry += (r[i] < tx);
  }

  borrow -= carry;
  mask = 0 - borrow;
  carry = 0;
  for (size_t i = 0; i < ml; i++)
  {
    tx = ((m[i] & mask) + carry) & CALC_MASK;
    carry = (tx < carry);
    r[i] = (r[i] + tx) & CALC_MASK;
    carry += (r[i] < tx);
  }

  return r;
}

// //
// // 蒙哥马利模算法
// //
// #include "__montgomery_ctx.cc"
// #include "__montgomery_reduction.cc"
// #include "__montgomery_mod.cc"