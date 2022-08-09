/*
 * Here follows specialised variants of bn_add_words() and bn_sub_words().
 * They have the property performing operations on arrays of different sizes.
 * The sizes of those arrays is expressed through cl, which is the common
 * length ( basically, min(len(a),len(b)) ), and dl, which is the delta
 * between the two lengths, calculated as len(a)-len(b). All lengths are the
 * number of BN_ULONGs...  For the operations that require a result array as
 * parameter, it must have the length cl+abs(dl). These functions should
 * probably end up in bn_asm.c as soon as there are assembler counterparts
 * for the systems that use assembler files.
 */

unit_t __sub_part(number_ptr_t& z, const number_ptr_t& x, const number_ptr_t& y, int cl, int dl) {
  unit_t c, t;

  my_assert(cl >= 0);
  c = bn_sub_words(r, a, b, cl);

  if (dl == 0)
      return c;

  r += cl;
  a += cl;
  b += cl;

  if (dl < 0) {
      for (;;) {
          t = b[0];
          r[0] = (0 - t - c) & BN_MASK2;
          if (t != 0)
              c = 1;
          if (++dl >= 0)
              break;

          t = b[1];
          r[1] = (0 - t - c) & BN_MASK2;
          if (t != 0)
              c = 1;
          if (++dl >= 0)
              break;

          t = b[2];
          r[2] = (0 - t - c) & BN_MASK2;
          if (t != 0)
              c = 1;
          if (++dl >= 0)
              break;

          t = b[3];
          r[3] = (0 - t - c) & BN_MASK2;
          if (t != 0)
              c = 1;
          if (++dl >= 0)
              break;

          b += 4;
          r += 4;
      }
  } else {
      int save_dl = dl;
      while (c) {
          t = a[0];
          r[0] = (t - c) & BN_MASK2;
          if (t != 0)
              c = 0;
          if (--dl <= 0)
              break;

          t = a[1];
          r[1] = (t - c) & BN_MASK2;
          if (t != 0)
              c = 0;
          if (--dl <= 0)
              break;

          t = a[2];
          r[2] = (t - c) & BN_MASK2;
          if (t != 0)
              c = 0;
          if (--dl <= 0)
              break;

          t = a[3];
          r[3] = (t - c) & BN_MASK2;
          if (t != 0)
              c = 0;
          if (--dl <= 0)
              break;

          save_dl = dl;
          a += 4;
          r += 4;
      }
      if (dl > 0) {
          if (save_dl > dl) {
              switch (save_dl - dl) {
              case 1:
                  r[1] = a[1];
                  if (--dl <= 0)
                      break;
                  /* fall thru */
              case 2:
                  r[2] = a[2];
                  if (--dl <= 0)
                      break;
                  /* fall thru */
              case 3:
                  r[3] = a[3];
                  if (--dl <= 0)
                      break;
              }
              a += 4;
              r += 4;
          }
      }
      if (dl > 0) {
          for (;;) {
              r[0] = a[0];
              if (--dl <= 0)
                  break;
              r[1] = a[1];
              if (--dl <= 0)
                  break;
              r[2] = a[2];
              if (--dl <= 0)
                  break;
              r[3] = a[3];
              if (--dl <= 0)
                  break;

              a += 4;
              r += 4;
          }
      }
  }
  return c;
}


/*
 * We calculate
 * a[0]*b[0]
 * a[0]*b[0]+a[1]*b[1]+(a[0]-a[1])*(b[1]-b[0])
 * a[1]*b[1]
 */

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
void __mul_recursive(number_t& z, const number_t& x, const number_t& y,
                     int n2, int dnx, int dny, number_t& t) {
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
  c1 = __cmp_park(bn_ptr(x), bn_ptr(x,n), tnx, n - tnx);
  c2 = __cmp_park(bn_ptr(y,n), bn_ptr(y), tny, tny - n);
  zero = neg = 0;
  switch (c1 * 3 + c2) {
  case -4:
    __sub_park(bn_ptr(t), bn_ptr(x,n), bn_ptr(x), tnx, tnx - n); /* - */
    __sub_park(bn_ptr(t,n), bn_ptr(y), bn_ptr(y, n), tny, n - tny); /* - */
    break;
  case -3:
    zero = 1;
    break;
  case -2:
    __sub_park(bn_ptr(t), bn_ptr(x,n), bn_ptr(x), tnx, tnx - n); /* - */
    __sub_park(bn_ptr(t,n), bn_ptr(y,n), bn_ptr(y), tny, tny - n); /* + */
    neg = 1;
    break;
  case -1:
  case 0:
  case 1:
    zero = 1;
    break;
  case 2:
    __sub_park(bn_ptr(t), bn_ptr(x), bn_ptr(x,n), tnx, n - tnx); /* + */
    __sub_park(bn_ptr(t,n), bn_ptr(y), bn_ptr(y,n), tny, n - tny); /* - */
    neg = 1;
    break;
  case 3:
    zero = 1;
    break;
  case 4:
    __sub_park(bn_ptr(t), bn_ptr(x), bn_ptr(x,n), tnx, n - tnx);
    __sub_park(bn_ptr(t,n), bn_ptr(y), bn_ptr(y,n), tny, tny - n);
    break;
  }

# ifdef BN_MUL_COMBA
    if (n == 4 && dnx == 0 && dny == 0) { /* XXX: bn_mul_comba4 could take
                                           * extra args to do this well */
        if (!zero)
            bn_mul_comba4(&(t[n2]), t, &(t[n]));
        else
            memset(&t[n2], 0, sizeof(*t) * 8);

        bn_mul_comba4(r, a, b);
        bn_mul_comba4(&(r[n2]), &(a[n]), &(b[n]));
    } else if (n == 8 && dnx == 0 && dny == 0) { /* XXX: bn_mul_comba8 could
                                                  * take extra args to do
                                                  * this well */
        if (!zero)
            bn_mul_comba8(&(t[n2]), t, &(t[n]));
        else
            memset(&t[n2], 0, sizeof(*t) * 16);

        bn_mul_comba8(r, a, b);
        bn_mul_comba8(&(r[n2]), &(a[n]), &(b[n]));
    } else
# endif                         /* BN_MUL_COMBA */
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

    if (neg) {                  /* if t[32] is negative */
        c1 -= (int)(bn_sub_words(&(t[n2]), t, &(t[n2]), n2));
    } else {
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
    if (c1) {
        p = &(r[n + n2]);
        lo = *p;
        ln = (lo + c1) & BN_MASK2;
        *p = ln;

        /*
         * The overflow will stop before we over write words we should not
         * overwrite
         */
        if (ln < (BN_ULONG)c1) {
            do {
                p++;
                lo = *p;
                ln = (lo + 1) & BN_MASK2;
                *p = ln;
            } while (ln == 0);
        }
    }
}