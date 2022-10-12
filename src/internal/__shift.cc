number_t __lshift1(const number_ptr_t &x, size_t xl)
{
  number_t y;
  register unit_t t = 0, c = 0;

  num_resize(y, xl + 1);

  number_ptr_t p = num_ptr(y);
  for (size_t i = 0; i < xl; i++)
  {
    t = x[i];
    *(p++) = ((t << 1) | c) & CALC_MASK;
    c = t >> (UNIT_BITS - 1);
  }
  *p = c;

  clear_number_head_zero(y);
  return y;
}

number_t __rshift1(const number_ptr_t &x, size_t xl)
{
  number_t y;
  register unit_t t = 0, c = 0;

  if (__is_zero(x, xl))
  {
    __zero(y);
    return y;
  }

  num_resize(y, xl);
  int i = static_cast<int>(xl);

  t = x[--i];
  y[i] = t >> 1;
  c = t << (UNIT_BITS - 1);

  while (i > 0)
  {
    t = x[--i];
    y[i] = ((t >> 1) & CALC_MASK) | c;
    c = t << (UNIT_BITS - 1);
  }

  clear_number_head_zero(y);
  return y;
}

number_t __lshift(const number_ptr_t &x, size_t xl, size_t n)
{
  number_t y;

  if (n == 0)
  {
    for (size_t i = 0; i < xl; i++)
      y[i] = x[i];
  }

  size_t nw = n / UNIT_BITS;  // 有多少个64位
  num_resize(y, xl + nw + 1); // 扩展到移动后的位数

  unit_t lb = static_cast<unit_t>(n) % UNIT_BITS;
  unit_t rb = UNIT_BITS - lb;
  rb %= UNIT_BITS;
  unit_t rmask = 0 - rb; // rmask = 0 - (rb != 0)
  rmask |= rmask >> 8;

  number_ptr_t f = x;
  number_ptr_t t = y.begin() + nw;

  unit_t l = f[xl - 1];
  t[xl] = (l >> rb) & rmask;
  unit_t m = 0;
  for (int i = static_cast<int>(xl) - 1; i > 0; i--)
  {
    m = l << lb;
    l = f[i - 1];
    t[i] = (m | ((l >> rb) & rmask)) & CALC_MASK;
  }
  t[0] = (l << lb) & CALC_MASK;

  clear_number_head_zero(y);
  return y;
}

number_t __rshift(const number_ptr_t &x, size_t xl, size_t n)
{
  number_t y;
  if (n == 0)
  {
    for (size_t i = 0; i < xl; i++)
      y[i] = x[i];
  }

  size_t nw = n / UNIT_BITS;
  if (nw >= xl)
  {
    __zero(y);
    return y;
  }

  unit_t rb = n % UNIT_BITS;
  unit_t lb = UNIT_BITS - rb;
  lb %= UNIT_BITS;
  unit_t mask = 0 - lb; /* mask = 0 - (lb != 0) */
  mask |= mask >> 8;
  size_t top = xl - nw;
  num_resize(y, top);

  number_ptr_t t = y.begin();
  number_ptr_t f = x + nw;
  unit_t l = f[0], m = 0;
  size_t i = 0;
  for (i = 0; i < top - 1; i++)
  {
    m = f[i + 1];
    t[i] = (l >> rb) | ((m << lb) & mask);
    l = m;
  }
  t[i] = l >> rb;

  clear_number_head_zero(y);
  return y;
}