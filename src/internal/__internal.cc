bool __is_zero(const number_t &x)
{
  if (x.empty())
    return false; // null不是0
  if (x.size() >= 1)
  {
    ///< 确保数值队列都是0
    for (size_t i = 0; i < x.size(); i++)
    {
      if (x[i] != 0)
        return false;
    }
  }
  return true;
}

bool __is_zero(const number_ptr_t &x, size_t xl)
{
  if (xl >= 1)
  {
    ///< 确保数值队列都是0
    for (size_t i = 0; i < xl; i++)
    {
      if (x[i] != 0)
        return false;
    }
  }
  return true;
}

void __zero(number_t &x)
{
  x.clear();
  x.push_back(0);
}

void __zero(const number_ptr_t &x, size_t xl)
{
  for (size_t i = 0; i < xl; i++)
  {
    x[i] = 0;
  }
}

void __one(number_t &x)
{
  x.clear();
  x.push_back(0);
}

void __one(const number_ptr_t &x, size_t xl)
{
  if (!xl)
    return;
  while (--xl)
  {
    x[xl] = 0;
  }
  x[0] = 1;
}

bool __is_one(const number_t &x)
{
  return __is_one(num_ptr(x), num_size(x));
}

bool __is_one(const number_ptr_t &x, size_t xl)
{
  if (x == 0)
    return false; // null不是0
  if (xl > 1)
  {
    ///< 确保数值队列都是0
    for (size_t i = 1; i < xl; i++)
    {
      if (x[i] != 0)
        return false;
    }
  }
  return x[0] == 1 ? true : false;
}

bool __is_odd(const number_t &x)
{
  return __is_odd(num_ptr(num_const_cast(x)));
}

bool __is_odd(const number_ptr_t &x)
{
  return (x[0] & 1);
}

/*
 * 这里此函数借鉴了Openssl的代码，在VS的编译器会之前会引发一个编译器优化
 * 错误，这里如果遇到微软的编译器则将其优化关闭。
 *
 * https://mta.openssl.org/pipermail/openssl-users/2018-August/008465.html
 */
#if defined(_MSC_VER) && defined(_ARM_) && defined(_WIN32_WCE) && _MSC_VER >= 1400 && _MSC_VER < 1501
#define MS_BROKEN_count_bits
#pragma optimize("", off)
#endif

/**
 * @brief         统计输入的数存在多少位
 * @param[in]     x
 * @return        x的二次幂
 */
size_t __count_bits(unit_t l)
{
  unit_t x, mask;
  size_t bits = (l != 0);

#if UNIT_BITS > 32
  x = l >> 32;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 32 & mask;
  l ^= (x ^ l) & mask;
#endif

  x = l >> 16;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 16 & mask;
  l ^= (x ^ l) & mask;

  x = l >> 8;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 8 & mask;
  l ^= (x ^ l) & mask;

  x = l >> 4;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 4 & mask;
  l ^= (x ^ l) & mask;

  x = l >> 2;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 2 & mask;
  l ^= (x ^ l) & mask;

  x = l >> 1;
  mask = (0 - x) & CALC_MASK;
  mask = (0 - (mask >> (UNIT_BITS - 1)));
  bits += 1 & mask;

  return bits;
}
#ifdef MS_BROKEN_count_bits
#pragma optimize("", on)
#endif

/**
 * @brief         统计输入的大数一同存在多少位
 * @param[in]     x
 * @return        一个大数总共的有效位数
 */
size_t __number_bits(const number_t &x)
{
  return __number_bits(num_ptr(num_const_cast(x)), num_size(x));
}

size_t __number_bits(const number_ptr_t &x, size_t xl)
{
  if (__is_zero(x, xl))
    return 0;

  size_t i = xl - 1;
  return ((i * UNIT_BITS) + __count_bits(x[i]));
}

// 第n位置位
void __set_bit(number_t &x, size_t n)
{
  size_t i = n / UNIT_BITS;
  size_t j = n % UNIT_BITS;

  size_t xl = num_size(x);
  if (xl <= i)
  {
    num_resize(x, i + 1);
  }

  x[i] |= (((unit_t)1) << j);
  return;
}

void __set_bit(number_ptr_t &x, size_t xl, size_t n)
{
  size_t i = n / UNIT_BITS;
  size_t j = n % UNIT_BITS;

  if (xl <= i)
  {
    mympz_out_of_range(i);
  }

  x[i] |= (((unit_t)1) << j);
  return;
}

// 第n位清位
void __clear_bit(number_t &x, size_t n)
{
  size_t i = n / UNIT_BITS;
  size_t j = n % UNIT_BITS;

  size_t xl = num_size(x);
  if (xl <= i) // 超出了x的队列
    return;

  x[i] &= (~(((unit_t)1) << j));
  return;
}

void __clear_bit(number_ptr_t &x, size_t xl, size_t n)
{
  size_t i = n / UNIT_BITS;
  size_t j = n % UNIT_BITS;

  if (xl <= i)
  {
    mympz_out_of_range(i);
  }

  x[i] &= (~(((unit_t)1) << j));
  return;
}

// 检查某位是否被设置
int __is_bit_set(const number_t &x, size_t n)
{
  size_t i = n / UNIT_BITS;
  size_t j = n % UNIT_BITS;

  size_t xl = num_size(x);
  if (xl <= i)
    return 0;
  return (int)(((x[i]) >> j) & ((unit_t)1));
}

int __is_bit_set(const number_ptr_t &x, size_t xl, size_t n)
{
  size_t i = n / UNIT_BITS;
  size_t j = n % UNIT_BITS;

  if (xl <= i)
    return 0;
  return (int)(((x[i]) >> j) & ((unit_t)1));
}

void __mask_bits(number_t &x, size_t n)
{
  size_t w = n / UNIT_BITS;
  size_t b = n % UNIT_BITS;

  size_t xl = num_size(x);
  if (w >= xl)
  {
    return;
  }

  if (b == 0)
  { // 如果是整个的，则只取到这里
    num_resize(x, w);
  }
  else
  {
    num_resize(x, w + 1);
    x[w] &= ~(CALC_MASK << b);
  }
  return;
}

void __mask_bits(number_ptr_t &x, size_t xl, size_t n)
{
  size_t w = n / UNIT_BITS;
  size_t b = n % UNIT_BITS;

  if (w >= xl)
  {
    mympz_out_of_range(w);
  }

  if (b == 0)
  {
    for (size_t i = w; i < xl; i++)
    {
      x[i] = 0;
    }
  }
  else
  {
    x[w] &= ~(CALC_MASK << b);
    // 避免w等于x-1的情况，造成溢出。
    if (w < xl - 1)
    {
      for (size_t i = w + 1; i < xl; i++)
      {
        x[i] = 0;
      }
    }
  }
  return;
}

size_t __left_align(const number_ptr_t &x, size_t xl)
{
  size_t rshift = __count_bits(x[xl - 1]); // 统计最高位的数值位数

  size_t lshift = UNIT_BITS - rshift; // 最高位左边还空余的位数
  rshift %= UNIT_BITS;
  unit_t rmask = (unit_t)0 - rshift; // rmask = 0 - (rshift != 0)
  rmask |= rmask >> 8;               // rmask为CALC_MASK

  unit_t n = 0, m = 0;
  for (size_t i = 0; i < xl; i++)
  {
    //
    // 先左移到需要得位然后或上上一个字节
    // 的右移位的部分。
    //
    n = x[i];
    x[i] = ((n << lshift) | m) & CALC_MASK;
    m = (n >> rshift) & rmask;
  }

  return lshift;
}

/* x和y的常数时间条件交换。如果condition不为0，则交换x和y。
 * nwords是要交换的字数。
 * 假设x和y中至少分配了n个字。
 * 假设x或y使用的字不超过n个。
 */
void __consttime_swap(unit_t condition, const number_ptr_t &x, size_t *xl, int *xneg,
                      const number_ptr_t &y, size_t *yl, int *yneg,
                      size_t nwords)
{
  my_assert(xneg && yneg, "%s", "pointer \'xneg\' | \'yneg\' is nullptr.");

  //
  // 这里必须保证x与y的长度等于nwords
  //
  // 设置条件数
  condition = ((~condition & ((condition - 1))) >> (UNIT_BITS2 - 1)) - 1;

  unit_t t = 0;

  //
  // 交换两个数的长度与符号
  //
  t = (*xl ^ *yl) & condition;
  *xl ^= t;
  *yl ^= t;

  t = (*xneg ^ *yneg) & condition;
  *xneg ^= t;
  *yneg ^= t;

  for (size_t i = 0; i < nwords; i++)
  {
    t = (x[i] ^ y[i]) & condition;
    x[i] ^= t;
    y[i] ^= t;
  }
}