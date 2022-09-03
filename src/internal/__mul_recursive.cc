/* 面是__sub_units()的特殊变体。*/
unit_t __sub_part(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y,
                  int cl, int dl)
{
  my_assert(cl >= 0, "common length is less than 0, cl = %d.", cl);
  unit_t c = __sub_units(z, x, y, cl);

  if (dl == 0)
    return c;

  number_ptr_t _x = num_ptr_const_cast(x);
  number_ptr_t _y = num_ptr_const_cast(y);
  number_ptr_t _z = num_ptr_const_cast(z);

  _z += cl;
  _x += cl;
  _y += cl;

  unit_t t = 0;
  if (dl < 0)
  {
    for (;;)
    {
      t = _y[0];
      _z[0] = (0 - t - c) & CALC_MASK;
      if (t != 0)
        c = 1;
      if (++dl >= 0)
        break;

      t = _y[1];
      _z[1] = (0 - t - c) & CALC_MASK;
      if (t != 0)
        c = 1;
      if (++dl >= 0)
        break;

      t = _y[2];
      _z[2] = (0 - t - c) & CALC_MASK;
      if (t != 0)
        c = 1;
      if (++dl >= 0)
        break;

      t = _y[3];
      _z[3] = (0 - t - c) & CALC_MASK;
      if (t != 0)
        c = 1;
      if (++dl >= 0)
        break;

      _y += 4;
      _z += 4;
    }
  }
  else
  {
    int save_dl = dl;
    while (c)
    {
      t = _x[0];
      _z[0] = (t - c) & CALC_MASK;
      if (t != 0)
        c = 0;
      if (--dl <= 0)
        break;

      t = _x[1];
      _z[1] = (t - c) & CALC_MASK;
      if (t != 0)
        c = 0;
      if (--dl <= 0)
        break;

      t = _x[2];
      _z[2] = (t - c) & CALC_MASK;
      if (t != 0)
        c = 0;
      if (--dl <= 0)
        break;

      t = _x[3];
      _z[3] = (t - c) & CALC_MASK;
      if (t != 0)
        c = 0;
      if (--dl <= 0)
        break;

      save_dl = dl;
      _x += 4;
      _z += 4;
    }
    if (dl > 0)
    {
      if (save_dl > dl)
      {
        switch (save_dl - dl)
        {
        case 1:
          _z[1] = _x[1];
          if (--dl <= 0)
            break;
          /* fall thru */
        case 2:
          _z[2] = _x[2];
          if (--dl <= 0)
            break;
          /* fall thru */
        case 3:
          _z[3] = _x[3];
          if (--dl <= 0)
            break;
        }
        _x += 4;
        _z += 4;
      }
    }
    if (dl > 0)
    {
      for (;;)
      {
        _z[0] = _x[0];
        if (--dl <= 0)
          break;
        _z[1] = _x[1];
        if (--dl <= 0)
          break;
        _z[2] = _x[2];
        if (--dl <= 0)
          break;
        _z[3] = _x[3];
        if (--dl <= 0)
          break;

        _x += 4;
        _z += 4;
      }
    }
  }
  return c;
}

/**
  * @brief         Karatsuba算法
  * @param[out]    z是2乘以n2个元素的长度的队列
  * @param[in]     x是n2个长度的队列
  * @param[in]     y是n2个长度的队列
  * @param[in]     n2是2的次幂
  * @param[in]     dnx是x队列要分治的长度，可能不是整数但是n2/2+dna必须是
  * @param[in]     dny是y队列要分治的长度,可能不是整数但是n2/2+dna必须是
  * @param[out]    t是2乘以n2个元素的长度的队列
  * @note
  * x[0]*y[0]
  * x[0]*y[0]+x[1]*y[1]+(x[0]-x[1])*(y[1]-y[0])
  * x[1]*y[1]
  * @return        x * y

  * @par Sample
  * @code
  * @endcode
  */
void __mul_units_recursive(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y,
                           int n2, int dnx, int dny, const number_ptr_t &t)
{
  int n = n2 / 2, c1, c2;
  int tnx = n + dnx, tny = n + dny;
  unsigned int neg, zero;
  unit_t ln, lo;

  if (n2 == 4)
  {
    __mul_4_units(z, x, y);
    return;
  }
  else if (n2 == 8 && dnx == 0 && dny == 0)
  {
    __mul_8_units(z, x, y);
    return;
  }

  /* 没达到使用递归的长度 */
  if (n2 < CALC_MUL_RECURSIVE_SIZE_NORMAL)
  {
    __mul_units_loop(z, x, n2 + dnx, y, n2 + dny);
    return;
  }

  /* z=(x[0]-y[1])*(y[1]-y[0]) */
  c1 = __cmp_part(x, x + n, tnx, n - tnx);
  c2 = __cmp_part(y + n, y, tny, tny - n);
  zero = neg = 0;
  switch (c1 * 3 + c2)
  {
  case -4:
    __sub_part(t, x + n, x, tnx, tnx - n);     /* - */
    __sub_part(t + n, y, y + n, tny, n - tny); /* - */
    break;
  case -3:
    zero = 1;
    break;
  case -2:
    __sub_part(t, x + n, x, tnx, tnx - n);     /* - */
    __sub_part(t + n, y + n, y, tny, tny - n); /* + */
    neg = 1;
    break;
  case -1:
  case 0:
  case 1:
    zero = 1;
    break;
  case 2:
    __sub_part(t, x, x + n, tnx, n - tnx);     /* + */
    __sub_part(t + n, y, y + n, tny, n - tny); /* - */
    neg = 1;
    break;
  case 3:
    zero = 1;
    break;
  case 4:
    __sub_part(t, x, x + n, tnx, n - tnx);
    __sub_part(t + n, y, y + n, tny, tny - n);
    break;
  }

  if (n == 4 && dnx == 0 && dny == 0)
  {
    if (!zero)
    {
      __mul_4_units(t + n2, t, t + n);
    }
    else
    {
      for (int i = 0; i < 8; i++)
        t[n2 + i] = 0;
    }

    __mul_4_units(z, x, y);
    __mul_4_units(z + n2, x + n, y + n);
  }
  else if (n == 8 && dnx == 0 && dny == 0)
  {
    if (!zero)
    {
      __mul_8_units(t + n2, t, t + n);
    }
    else
    {
      for (int i = 0; i < 16; i++)
        t[n2 + i] = 0;
    }

    __mul_8_units(z, x, y);
    __mul_8_units(z + n2, x + n, y + n);
  }
  else
  {
    const number_ptr_t p = t + (n2 * 2);
    if (!zero)
    {
      __mul_units_recursive(t + n2, t, t + n, n, 0, 0, p);
    }
    else
    {
      for (int i = 0; i < n2; i++)
        t[n2 + i] = 0;
    }

    __mul_units_recursive(z, x, y, n, 0, 0, p);
    __mul_units_recursive(z + n2, x + n, y + n, n, dnx, dny, p);
  }

  /*-
   * t[32] 存储 (a[0]-a[1])*(b[1]-b[0]), c1 is the sign
   * r[10] 存储 (a[0]*b[0])
   * r[32] 存储 (b[1]*b[1])
   */
  c1 = static_cast<int>(__add_units(t, z, z + n2, n2));

  if (neg)
  { /* 如果 t[32] 是负数 */
    c1 -= static_cast<int>(__sub_units(t + n2, t, t + n2, n2));
  }
  else
  {
    /* 可能有进位 */
    c1 += static_cast<int>(__add_units(t + n2, t + n2, t, n2));
  }

  /*-
   * t[32] 存储 (a[0]-a[1])*(b[1]-b[0])+(a[0]*b[0])+(a[1]*b[1])
   * r[10] 存储 (a[0]*b[0])
   * r[32] 存储 (b[1]*b[1])
   * c1 存储 进位
   */
  c1 += static_cast<int>(__add_units(z + n, z + n, t + n2, n2));
  if (c1)
  {
    number_ptr_t p = num_ptr_const_cast(z);
    p += (n + n2);
    lo = *p;
    ln = (lo + c1) & CALC_MASK;
    *p = ln;

    /*
     * 溢出将停止在写入数值前。
     */
    if (ln < static_cast<unit_t>(c1))
    {
      do
      {
        p++;
        lo = *p;
        ln = (lo + 1) & CALC_MASK;
        *p = ln;
      } while (ln == 0);
    }
  }
}

#define __zero_number_ptr_data(ptr, l) \
  {                                    \
    const number_ptr_t ___ttt = (ptr); \
    for (int i = 0; i < (l); i++)      \
    {                                  \
      ___ttt[i] = 0;                   \
    }                                  \
  }

/**
  * @brief         Karatsuba算法
  * @param[out]    z是2乘以n2个元素的长度的队列
  * @param[in]     x是n2个长度的队列
  * @param[in]     y是n2个长度的队列
  * @param[in]     n2是2的次幂
  * @param[in]     tnx是x队列要分治的长度，这里不能是负数，但是必须小于n。
  * @param[in]     tny是y队列要分治的长度，这里不能是负数，但是必须小于n。
  * @param[out]    t是2乘以n2个元素的长度的队列
  * @return        x * y
  * @note n+tn是t的长度，这个长度需要是n*4的长度，和z的长度一样。

  * @par Sample
  * @code
  * @endcode
  */
void __mul_part_units_recursive(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, int n,
                                int tnx, int tny, const number_ptr_t &t)
{
  int i, j, n2 = n * 2;
  int c1, c2, neg;
  unit_t ln, lo;

  if (n < 8)
  {
    __mul_units_loop(z, x, n + tnx, y, n + tny);
    return;
  }

  /* z=(x[0]-x[1])*(y[1]-y[0]) */
  c1 = __cmp_part(x, x + n, tnx, n - tnx);
  c2 = __cmp_part(y + n, y, tny, tny - n);
  neg = 0;
  switch (c1 * 3 + c2)
  {
  case -4:
    __sub_part(t, x + n, x, tnx, tnx - n);     /* - */
    __sub_part(t + n, y, y + n, tny, n - tny); /* - */
    break;
  case -3:
  case -2:
    __sub_part(t, x + n, x, tnx, tnx - n);     /* - */
    __sub_part(t + n, y + n, y, tny, tny - n); /* + */
    neg = 1;
    break;
  case -1:
  case 0:
  case 1:
  case 2:
    __sub_part(t, x, x + n, tnx, n - tnx);     /* + */
    __sub_part(t + n, y, y + n, tny, n - tny); /* - */
    neg = 1;
    break;
  case 3:
  case 4:
    __sub_part(t, x, x + n, tnx, n - tnx);
    __sub_part(t + n, y + n, y, tny, tny - n);
    break;
  }

  /* 0状况没有实现，这点速度的提升可能是微不足道的。*/
#if 0
    if (n == 4) {
        __mul_4_units(t + n2, t, t + n);
        __mul_4_units(z, x, y);
        __mul_units_loop(z + n2, x + n, tn, y + n, tn);
        __zero_number_ptr_data(z + n2 + tn * 2, n2 - tn * 2);
    } else
#endif
  if (n == 8)
  {
    __mul_8_units(t + n2, t, t + n);
    __mul_8_units(z, x, y);
    __mul_units_loop(z + n2, x + n, tnx, y + n, tny);
    __zero_number_ptr_data(z + n2 + tnx + tny, n2 - tnx - tny);
  }
  else
  {
    // const number_ptr_t p = num_ptr_const_cast(t + (n2 * 2));
    const number_ptr_t p = t + (n2 * 2);
    __mul_units_recursive(t + n2, t, t + n, n, 0, 0, p);
    __mul_units_recursive(z, x, y, n, 0, 0, p);
    i = n / 2;
    /*
     * 如果这个数只有下半部分，直接处理。
     */
    if (tnx > tny)
      j = tnx - i;
    else
      j = tny - i;
    if (j == 0)
    {
      __mul_units_recursive(z + n2, x + n, y + n, i, tnx - i, tny - i, p);
      __zero_number_ptr_data(z + n2 + i * 2, n2 - i * 2);
    }
    else if (j > 0)
    { /* eg, n == 16, i == 8 and tn == 11 */
      __mul_part_units_recursive(z + n2, x + n, y + n, i, tnx - i, tny - i, p);
      __zero_number_ptr_data(z + n2 + tnx + tny, n2 - tnx - tny);
    }
    else
    { /* (j < 0) eg, n == 16, i == 8 and tn == 5 */
      __zero_number_ptr_data(z + n2, n2);
      if (tnx < CALC_MUL_RECURSIVE_SIZE_NORMAL && tny < CALC_MUL_RECURSIVE_SIZE_NORMAL)
      {
        __mul_units_loop(z + n2, x + n, tnx, y + n, tny);
      }
      else
      {
        for (;;)
        {
          i /= 2;
          /*
           * 这些简化的条件只适用于tnx和tny的区别是1或0
           */
          if (i < tnx || i < tny)
          {
            __mul_part_units_recursive(z + n2, x + n, y + n, i, tnx - i, tny - i, p);
            break;
          }
          else if (i == tnx || i == tny)
          { 
            __mul_units_recursive(z + n2, x + n, y + n, i, tnx - i, tny - i, p);
            break;
          }
        }
      }
    }
  }

  /*-
   * t[32] 存储 (x[0]-x[1])*(y[1]-y[0]), c1 是符号位
   * z[10] 存储 (x[0]*y[0])
   * z[32] 存储 (y[1]*y[1])
   */

  c1 = static_cast<int>(__add_units(t, z, z + n2, n2));
  if (neg)
  { /* 如果 t[32] 是负数 */
    c1 -= static_cast<int>(__sub_units(t + n2, t, t + n2, n2));
  }
  else
  {
    /* 也许有进位 */
    c1 += static_cast<int>(__add_units(t + n2, t + n2, t, n2));
  }

  /*-
   * t[32] 存储 (x[0]-x[1])*(y[1]-y[0])+(x[0]*y[0])+(x[1]*y[1])
   * z[10] 存储 (x[0]*x[0])
   * z[32] 存储 (y[1]*y[1])
   * c1 存储进位
   */
  c1 += static_cast<int>(__add_units(z + n, z + n, t + n2, n2));
  if (c1)
  {
    number_ptr_t p = num_ptr_const_cast(z);
    p += (n + n2);
    lo = *p;
    ln = (lo + c1) & CALC_MASK;
    *p = ln;

    /*
     * 溢出将停止在写入数值前。
     */
    if (ln < static_cast<unit_t>(c1))
    {
      do
      {
        p++;
        lo = *p;
        ln = (lo + 1) & CALC_MASK;
        *p = ln;
      } while (ln == 0);
    }
  }
}

/**
 * @brief         Karatsuba算法
 * @param[out]    z是2乘以n2个元素的长度的队列
 * @param[in]     x是n2个长度的队列，与y长度相同。
 * @param[in]     y是n2个长度的队列，与x长度相同。
 * @param[in]     n2是2的次幂
 * @param[in]     t是n2*2长度
 * @return        x * y
 */
void __mul_units_low_recursive(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, int n2,
                               const number_ptr_t &t)
{
  int n = n2 / 2;

  __mul_units_recursive(z, x, y, n, 0, 0, t);
  if (n >= CALC_MUL_LOW_RECURSIVE_SIZE_NORMAL)
  {
    __mul_units_low_recursive(t, x, y + n, n, t + n2);
    __add_units(z + n, z + n, t, n);
    __mul_units_low_recursive(t, x + n, y, n, t + n2);
    __add_units(z + n, z + n, t, n);
  }
  else
  {
    __mul_units_low_loop(t, x, y + n, n);
    __mul_units_low_loop(t + n, x + n, y, n);
    __add_units(z + n, z + n, t, n);
    __add_units(z + n, z + n, t + n, n);
  }
}