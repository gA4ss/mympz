/* 面是__sub_units()的特殊变体。*/
unit_t __sub_part(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y,
                  int cl, int dl) {
  my_assert(cl >= 0);
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
  if (dl < 0) {
    for (;;) {
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
  } else {
    int save_dl = dl;
    while (c) {
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
    } if (dl > 0) {
      if (save_dl > dl) {
        switch (save_dl - dl) {
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
    } if (dl > 0) {
      for (;;) {
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
  * @return        x * y

  * @par Sample
  * @code
  * @endcode
  */
void __mul_units_recursive(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y,
                           int n2, int dnx, int dny, const number_ptr_t &t) {
  int n = n2 / 2, c1, c2;
  int tnx = n + dnx, tny = n + dny;
  unsigned int neg, zero;
  unit_t ln, lo, *p;

  if (n2 == 4) {
    __mul_4_units(z, x, y);
    return;
  } else if (n2 == 8 && dnx == 0 && dny == 0) {
    __mul_8_units(z, x, y);
    return;
  }

  /* 没达到使用递归的长度 */
  if (n2 < CALC_MUL_RECURSIVE_SIZE_NORMAL) {
    z.clear();
    __mul_units_loop(z, x, y);
    return;
  }

  /* z=(x[0]-y[1])*(y[1]-y[0]) */
  c1 = __cmp_park(x, x + n, tnx, n - tnx);
  c2 = __cmp_park(y + n, y, tny, tny - n);
  zero = neg = 0;
  switch (c1 * 3 + c2) {
  case -4:
    __sub_park(t, x + n, x, tnx, tnx - n);     /* - */
    __sub_park(t + n, y, y + n, tny, n - tny); /* - */
    break;
  case -3:
    zero = 1;
    break;
  case -2:
    __sub_park(t, x + n, x, tnx, tnx - n);     /* - */
    __sub_park(t + n, y + n, y, tny, tny - n); /* + */
    neg = 1;
    break;
  case -1:
  case 0:
  case 1:
    zero = 1;
    break;
  case 2:
    __sub_park(t, x, x + n, tnx, n - tnx);     /* + */
    __sub_park(t + n, y, y + n, tny, n - tny); /* - */
    neg = 1;
    break;
  case 3:
    zero = 1;
    break;
  case 4:
    __sub_park(t, x, x + n, tnx, n - tnx);
    __sub_park(t + n, y, y + n, tny, tny - n);
    break;
  }

  if (n == 4 && dnx == 0 && dny == 0) {
    if (!zero)
      __mul_4_units(&(t[n2]), t, &(t[n]));
    else
      memset(&t[n2], 0, sizeof(*t) * 8);

    bn_mul_comba4(r, a, b);
    bn_mul_comba4(&(r[n2]), &(a[n]), &(b[n]));
  }
  else if (n == 8 && dnx == 0 && dny == 0)
  { /* XXX: bn_mul_comba8 could
     * take extra args to do
     * this well */
    if (!zero)
      bn_mul_comba8(&(t[n2]), t, &(t[n]));
    else
      memset(&t[n2], 0, sizeof(*t) * 16);

    bn_mul_comba8(r, a, b);
    bn_mul_comba8(&(r[n2]), &(a[n]), &(b[n]));
  }
  else
  {
    p = &(t[n2 * 2]);
    if (!zero)
      bn_mul_recursive(&(t[n2]), t, &(t[n]), n, 0, 0, p);
    else
      memset(&t[n2], 0, sizeof(*t) * n2);
    bn_mul_recursive(r, a, b, n, 0, 0, p);
    bn_mul_recursive(&(r[n2]), &(a[n]), &(b[n]), n, dnx, dny, p);
  }

  /*-
   * t[32] holds (a[0]-a[1])*(b[1]-b[0]), c1 is the sign
   * r[10] holds (a[0]*b[0])
   * r[32] holds (b[1]*b[1])
   */

  c1 = (int)(bn_add_words(t, r, &(r[n2]), n2));

  if (neg)
  { /* if t[32] is negative */
    c1 -= (int)(bn_sub_words(&(t[n2]), t, &(t[n2]), n2));
  }
  else
  {
    /* Might have a carry */
    c1 += (int)(bn_add_words(&(t[n2]), &(t[n2]), t, n2));
  }

  /*-
   * t[32] holds (a[0]-a[1])*(b[1]-b[0])+(a[0]*b[0])+(a[1]*b[1])
   * r[10] holds (a[0]*b[0])
   * r[32] holds (b[1]*b[1])
   * c1 holds the carry bits
   */
  c1 += (int)(bn_add_words(&(r[n]), &(r[n]), &(t[n2]), n2));
  if (c1)
  {
    p = &(r[n + n2]);
    lo = *p;
    ln = (lo + c1) & CALC_MASK;
    *p = ln;

    /*
     * The overflow will stop before we over write words we should not
     * overwrite
     */
    if (ln < (BN_ULONG)c1)
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