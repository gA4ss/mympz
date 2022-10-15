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

/* 求 x^{-1}x == 1 (mod n) */
number_t __mod_inverse(const number_ptr_t &a, size_t al,
                       const number_ptr_t &n, size_t nl)
{
  number_t A, B, X, Y;
  number_t xi;
  int sign;

  __one(X);
  _zero(Y);

  num_resize(B, xl);
  for (size_t i = 0; i < xl, i++)
    B[i] = x[i];

  num_resize(A, nl);
  for (size_t i = 0; i < nl, i++)
    A[i] = n[i];

  sign = -1;

  //
  // B = a mod |n|,  A = |n| 得到
  // 0 <= B < A,
  // -sign*X*a  ==  B   (mod |n|),
  // sign*Y*a  ==  A   (mod |n|).
  //
  if (__is_odd(n) && (__number_bits(n, nl) <= 2048))
  {
    //
    // 二进制模逆算法；需要奇数的模数。如果模数充分小这比一般的模算法更快。
    // 在32位系统上大概400 - 500位，在64位系统上更多些。
    //
    int shift;

    while (!__is_zero(B))
    {
      //
      //      0 < B < |n|,
      //      0 < A <= |n|,
      // (1) -sign*X*a  ==  B   (mod |n|),
      // (2)  sign*Y*a  ==  A   (mod |n|)
      //
      //
      // 现在用整数的最大的2次幂除以B，并且使用同样数模|n|再除以X。
      // 当所有的完成后，(1)式成立。
      //
      shift = 0;
      while (!__is_bit_set(B, shift))
      { /* 0 < B */
        shift++;

        if (_is_odd(X))
        {
          X = __add_units(num_ptr(X), num_size(X), n, nl);
        }

        // 现在X是偶数，因此我们能简单的将它除以2。
        X = __rshift1(num_ptr(X), num_size(X));
      }
      if (shift > 0)
      {
        B = __rshift(num_ptr(B), num_size(B), shift);
      }

      //
      // 同样对于A与Y，运行以下代码后，式(2)成立。
      //
      shift = 0;
      while (!_is_bit_set(A, shift))
      { /* 0 < A */
        shift++;

        if (_is_odd(Y))
        {
          Y = __add_units(num_ptr(Y), num_size(Y), n, nl);
        }
        /* Y是偶数 */
        Y = __rshift1(num_ptr(Y), num_size(Y));
      }
      if (shift > 0)
      {
        A = __rshift(num_ptr(A), num_size(A), shift);
      }

      //
      // 式(1),(2)都满足。
      // A与B是奇数。
      // 确保以下计算正确。
      //
      //     0 <= B < |n|,
      //      0 < A < |n|,
      // (1) -sign*X*a  ==  B   (mod |n|),
      // (2)  sign*Y*a  ==  A   (mod |n|),
      //
      // 下面的迭代要么A是偶数要么B是偶数。
      //
      //
      if (__cmp(B, A) >= 0)
      {
        /* -sign*(X + Y)*a == B - A  (mod |n|) */
        X = __add_units(num_ptr(X), num_size(X), num_ptr(Y), num_size(Y));
        B = __sub_units(num_ptr(B), num_size(B), num_ptr(A), num_size(A));
      }
      else
      {
        /*  sign*(X + Y)*a == A - B  (mod |n|) */
        Y = __add_units(num_ptr(Y), num_size(Y), num_ptr(X), num_size(X));
        A = __sub_units(num_ptr(A), num_size(A), num_ptr(B), num_size(B));
      }
    }
  }
  else
  {
    /* 一般的模逆算法 */
    while (!__is_zero(B))
    {
      number_t tmp;
      number_t M, D, T, R;

      //
      //      0 < B < A,
      // (*) -sign*X*a  ==  B   (mod |n|),
      //      sign*Y*a  ==  A   (mod |n|)
      //

      /* (D, M) := (A/B, A%B) ... */
      if (__number_bits(A) == __number_bits(B))
      {
        __one(D);
        M = __sub_units(num_ptr(A), num_size(A), num_ptr(B), num_size(B));
      }
      else if (__number_bits(A) == __number_bits(B) + 1)
      {
        /* A/B 是 1, 2, 或者 3 */
        T = __lshift1(num_ptr(B), num_size(B));
        if (BN_ucmp(A, T) < 0)
        {
          /* A < 2*B, 因此 D=1 */
          __one(D);
          M = __sub_units(num_ptr(A), num_size(A), num_ptr(B), num_size(B));
        }
        else
        {
          /* A >= 2*B, 因此 D=2 或者 D=3 */
          M = __sub_units(num_ptr(A), num_size(A), num_ptr(T), num_size(T));
          D = __add_units(num_ptr(T), num_size(T), num_ptr(B), num_size(B));
          if (__cmp(A, D) < 0)
          {
            /* A < 3*B, 因此 D=2 */
            D.clear(); D.push_back(2);
            //
            // M (= A - 2*B) 已经有了正确的值。
            //
          }
          else
          {
            /* 仅仅 D=3 剩余 */
            D.clear(); D.push_back(3);
            //
            // 当前 M = A - 2*B，但是我们需要 M = A - 3*B
            //
            M = __add_units(num_ptr(M), num_size(M), num_ptr(B), num_size(B));
          }
        }
      }
      else
      {
        if (!BN_div(D, M, A, B, ctx))
          goto err;
      }

      /*-
       * Now
       *      A = D*B + M;
       * thus we have
       * (**)  sign*Y*a  ==  D*B + M   (mod |n|).
       */

      tmp = A; /* keep the BIGNUM object, the value does not matter */

      /* (A, B) := (B, A mod B) ... */
      A = B;
      B = M;
      /* ... so we have  0 <= B < A  again */

      /*-
       * Since the former  M  is now  B  and the former  B  is now  A,
       * (**) translates into
       *       sign*Y*a  ==  D*A + B    (mod |n|),
       * i.e.
       *       sign*Y*a - D*A  ==  B    (mod |n|).
       * Similarly, (*) translates into
       *      -sign*X*a  ==  A          (mod |n|).
       *
       * Thus,
       *   sign*Y*a + D*sign*X*a  ==  B  (mod |n|),
       * i.e.
       *        sign*(Y + D*X)*a  ==  B  (mod |n|).
       *
       * So if we set  (X, Y, sign) := (Y + D*X, X, -sign), we arrive back at
       *      -sign*X*a  ==  B   (mod |n|),
       *       sign*Y*a  ==  A   (mod |n|).
       * Note that  X  and  Y  stay non-negative all the time.
       */

      /*
       * most of the time D is very small, so we can optimize tmp := D*X+Y
       */
      if (BN_is_one(D))
      {
        if (!BN_add(tmp, X, Y))
          goto err;
      }
      else
      {
        if (BN_is_word(D, 2))
        {
          if (!BN_lshift1(tmp, X))
            goto err;
        }
        else if (BN_is_word(D, 4))
        {
          if (!BN_lshift(tmp, X, 2))
            goto err;
        }
        else if (D->top == 1)
        {
          if (!BN_copy(tmp, X))
            goto err;
          if (!BN_mul_word(tmp, D->d[0]))
            goto err;
        }
        else
        {
          if (!BN_mul(tmp, D, X, ctx))
            goto err;
        }
        if (!BN_add(tmp, tmp, Y))
          goto err;
      }

      M = Y; /* keep the BIGNUM object, the value does not matter */
      Y = X;
      X = tmp;
      sign = -sign;
    }
  }

  /*-
   * The while loop (Euclid's algorithm) ends when
   *      A == gcd(a,n);
   * we have
   *       sign*Y*a  ==  A  (mod |n|),
   * where  Y  is non-negative.
   */

  if (sign < 0)
  {
    if (!BN_sub(Y, n, Y))
      goto err;
  }
  /* Now  Y*a  ==  A  (mod |n|).  */

  if (BN_is_one(A))
  {
    /* Y*a == 1  (mod |n|) */
    if (!Y->neg && BN_ucmp(Y, n) < 0)
    {
      if (!BN_copy(R, Y))
        goto err;
    }
    else
    {
      if (!BN_nnmod(R, Y, n, ctx))
        goto err;
    }
  }
  else
  {
    *pnoinv = 1;
    goto err;
  }
  ret = R;
err:
  if ((ret == NULL) && (in == NULL))
    BN_free(R);
  BN_CTX_end(ctx);
  bn_check_top(ret);
  return ret;
}

// //
// // 蒙哥马利模算法
// //
// #include "__montgomery_ctx.cc"
// #include "__montgomery_reduction.cc"
// #include "__montgomery_mod.cc"