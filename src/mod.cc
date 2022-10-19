#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{
  /**
   * @brief         一个大数与一个字取模数
   * @param[in]     x 大数
   * @param[in]     w 一个字
   * @return        r = x % w
   */
  unit_t mod(const bignum_t &x, unit_t w, int wneg)
  {
    unit_t r = 0;

    if (w == 0)
    {
      mympz_divide_by_zero(w);
    }

    if (w > ((unit_t)1 << UNIT_HALF_BITS))
    {
      div(x, w, wneg, &r);
      return r;
    }

    size_t xl = bn_size(x);
    w &= CALC_MASK;
    for (int i = static_cast<int>(xl) - 1; i >= 0; i--)
    {
      r = ((r << UNIT_HALF_BITS) | ((x.number[i] >> UNIT_HALF_BITS) & CALC_MASKl)) % w;
      r = ((r << UNIT_HALF_BITS) | (x.number[i] & CALC_MASKl)) % w;
    }
    return r;
  }

  /**
   * @brief         有符号的两个大数的模数
   * @param[in]     x
   * @param[in]     y
   * @return        r = x % y
   */
  // bignum_t mod(const bignum_t& x, const bignum_t& y) {
  //   division_result_t z = div(x, y);
  //   return z.second;
  // }
  bignum_t mod(const bignum_t &x, const bignum_t &y)
  {
    bignum_t r;
    r.neg = 0;
    division_result_t res = div(x, y);
    r = res.second;
    return r;
  }

  /**
   * @brief         有符号的两个大数去与模|y|
   * @param[in]     x
   * @param[in]     y
   * @return        r = x % |y|
   * @note 与mod运算一致，但是返回非负的余数。
   *       $0 <= r < |y|$
   */
  bignum_t nnmod(const bignum_t &x, const bignum_t &y)
  {
    bignum_t r = mod(x, y);
    if (!r.neg) // 如果为正数
      return r;
    /* 现在 -|y| < r < 0, 因此我们需要设置 r = r + |y| */
    if (y.neg)
      return sub(r, y);
    return add(r, y);
  }

  /**
   * @brief         有符号的两个大数的模数相加取模后取绝对值
   * @param[in]     x
   * @param[in]     y
   * @return        r = (x + y) % |m|
   */
  bignum_t mod_add(const bignum_t &x, const bignum_t &y, const bignum_t &m)
  {
    bignum_t z;
    // x,y > 0 && x,y <= m
    if ((cmp(x, m) <= 0) && (cmp(y, m) <= 0) &&
        (x.neg == 0) && (y.neg == 0))
    {
      bignum_t _x = bn_const_cast(x);
      bignum_t _y = bn_const_cast(y);
      bignum_t _m = bn_const_cast(m);
      z.number = __mod_add(bn_ptr(_x), bn_size(_x), bn_ptr(_y), bn_size(_y),
                           bn_ptr(_m), bn_size(_m));
      z.neg = 0;
      return z;
    }
    z = add(x, y);
    return nnmod(z, m);
  }

  /**
   * @brief         有符号的两个大数的模数相减取模后取绝对值
   * @param[in]     x
   * @param[in]     y
   * @return        r = (x - y) % |m|
   */
  bignum_t mod_sub(const bignum_t &x, const bignum_t &y, const bignum_t &m)
  {
    bignum_t z;
    // x,y > 0 && x,y <= m
    if ((cmp(x, m) <= 0) && (cmp(y, m) <= 0) &&
        (x.neg == 0) && (y.neg == 0))
    {
      bignum_t _x = bn_const_cast(x);
      bignum_t _y = bn_const_cast(y);
      bignum_t _m = bn_const_cast(m);
      z.number = __mod_sub(bn_ptr(_x), bn_size(_x), bn_ptr(_y), bn_size(_y),
                           bn_ptr(_m), bn_size(_m));
      z.neg = 0;
      return z;
    }
    z = sub(x, y);
    return nnmod(z, m);
  }

  /**
   * @brief         有符号的两个大数的模数相乘取模后取绝对值
   * @param[in]     x
   * @param[in]     y
   * @return        r = (x * y) % |m|
   */
  bignum_t mod_mul(const bignum_t &x, const bignum_t &y, const bignum_t &m)
  {
    bignum_t z;

    if (cmp(x, y) == 0)
    {
      z = sqr(x);
      return z;
    }

    z = mul(x, y);
    return nnmod(z, m);
  }

  /**
   * @brief         大数的平方取模后取绝对值
   * @param[in]     x
   * @return        r = (x * x) % m
   */
  bignum_t mod_sqr(const bignum_t &x, const bignum_t &m)
  {
    bignum_t y = sqr(x);
    return mod(y, m);
  }

  /**
   * @brief         大数的左移1位取模|m|
   * @param[in]     x
   * @return        r = (x << 1) % |m|
   */
  bignum_t mod_lshift1(const bignum_t &x, const bignum_t &m)
  {
    bignum_t r = lshift1(x);
    if (cmp(r, m) >= 0)
    {
      return sub(r, m);
    }
    return nnmod(r, m);
  }

  /**
   * @brief         大数的左移n位取模|m|
   * @param[in]     x x < m && x >= 0
   * @param[in]     n 要移动的位数
   * @return        r = x << n
   */
  bignum_t __mod_lshift(const bignum_t &x, int n, const bignum_t &m)
  {
    bignum_t r = x;

    while (n > 0)
    {
      int max_shift;

      /* 0 < r < m */
      max_shift = static_cast<int>(__number_bits(m.number) - __number_bits(r.number));
      /* max_shift >= 0 */
      if (max_shift < 0)
      {
        mympz_exception("%s", "invalid argument : r > m");
      }

      if (max_shift > n)
        max_shift = n;

      // m与x的位数相同
      if (max_shift)
      {
        r = lshift(r, max_shift);
        n -= max_shift;
      }
      else
      {
        r = lshift1(r);
        --n;
      }

      if (cmp(r, m) >= 0)
      {
        r = sub(r, m);
      }
    }

    return r;
  }

  /**
   * @brief         大数的左移n位取模后模|m|
   * @param[in]     x
   * @param[in]     n 要移动的位数
   * @return        r = x % |m| << n
   */
  bignum_t mod_lshift(const bignum_t &x, size_t n, const bignum_t &m)
  {
    bignum_t r, abs_m;

    r = nnmod(x, m);

    // 如果模数为负数
    if (m.neg)
    {
      abs_m = m;
      abs_m.neg = 0;
    }

    return __mod_lshift(r, static_cast<int>(n), (m.neg ? abs_m : m));
  }

#if 0
  /**
   * @brief         大数的模幂算法
   * @param[in]     x
   * @param[in]     p 指数
   * @param[in]     m 模数
   * @return        x^p mod m
   * @note
   *       对于任意的模数 m = 2^k * m_odd，可以分成两个步骤进行计算：x^p mod m_odd
   *       与 x^p mod 2^k，对于奇数部分使用蒙哥马利算法进行运算，使用近似指数消减并且
   *       通过CRT进行合并结果。
   *
   *       当且仅当指数是奇数的时候，使用蒙哥马利算法；其余情况使用指数消减进行运算。
   */
  bignum_t mod_exp(const bignum_t &x, const bignum_t &p, const bignum_t &m)
  {
    bignum_t y;
    if (is_odd(m))
    {
      y = mod_exp_mont(x, p, m);
    }
    else
    {
      y = mod_exp_recp(x, p, m);
    }

    return y;
  }

int BN_mod_exp_recp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                    const BIGNUM *m, BN_CTX *ctx)
{
    int i, j, bits, ret = 0, wstart, wend, window, wvalue;
    int start = 1;
    BIGNUM *aa;
    /* Table of variables obtained from 'ctx' */
    BIGNUM *val[TABLE_SIZE];
    BN_RECP_CTX recp;

    if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(a, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(m, BN_FLG_CONSTTIME) != 0) {
        /* BN_FLG_CONSTTIME only supported by BN_mod_exp_mont() */
        ERR_raise(ERR_LIB_BN, ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return 0;
    }

    bits = BN_num_bits(p);
    if (bits == 0) {
        /* x**0 mod 1, or x**0 mod -1 is still zero. */
        if (BN_abs_is_word(m, 1)) {
            ret = 1;
            BN_zero(r);
        } else {
            ret = BN_one(r);
        }
        return ret;
    }

    BN_RECP_CTX_init(&recp);

    BN_CTX_start(ctx);
    aa = BN_CTX_get(ctx);
    val[0] = BN_CTX_get(ctx);
    if (val[0] == NULL)
        goto err;

    if (m->neg) {
        /* ignore sign of 'm' */
        if (!BN_copy(aa, m))
            goto err;
        aa->neg = 0;
        if (BN_RECP_CTX_set(&recp, aa, ctx) <= 0)
            goto err;
    } else {
        if (BN_RECP_CTX_set(&recp, m, ctx) <= 0)
            goto err;
    }

    if (!BN_nnmod(val[0], a, m, ctx))
        goto err;               /* 1 */
    if (BN_is_zero(val[0])) {
        BN_zero(r);
        ret = 1;
        goto err;
    }

    window = BN_window_bits_for_exponent_size(bits);
    if (window > 1) {
        if (!BN_mod_mul_reciprocal(aa, val[0], val[0], &recp, ctx))
            goto err;           /* 2 */
        j = 1 << (window - 1);
        for (i = 1; i < j; i++) {
            if (((val[i] = BN_CTX_get(ctx)) == NULL) ||
                !BN_mod_mul_reciprocal(val[i], val[i - 1], aa, &recp, ctx))
                goto err;
        }
    }

    start = 1;                  /* This is used to avoid multiplication etc
                                 * when there is only the value '1' in the
                                 * buffer. */
    wvalue = 0;                 /* The 'value' of the window */
    wstart = bits - 1;          /* The top bit of the window */
    wend = 0;                   /* The bottom bit of the window */

    if (!BN_one(r))
        goto err;

    for (;;) {
        if (BN_is_bit_set(p, wstart) == 0) {
            if (!start)
                if (!BN_mod_mul_reciprocal(r, r, r, &recp, ctx))
                    goto err;
            if (wstart == 0)
                break;
            wstart--;
            continue;
        }
        /*
         * We now have wstart on a 'set' bit, we now need to work out how bit
         * a window to do.  To do this we need to scan forward until the last
         * set bit before the end of the window
         */
        wvalue = 1;
        wend = 0;
        for (i = 1; i < window; i++) {
            if (wstart - i < 0)
                break;
            if (BN_is_bit_set(p, wstart - i)) {
                wvalue <<= (i - wend);
                wvalue |= 1;
                wend = i;
            }
        }

        /* wend is the size of the current window */
        j = wend + 1;
        /* add the 'bytes above' */
        if (!start)
            for (i = 0; i < j; i++) {
                if (!BN_mod_mul_reciprocal(r, r, r, &recp, ctx))
                    goto err;
            }

        /* wvalue will be an odd number < 2^window */
        if (!BN_mod_mul_reciprocal(r, r, val[wvalue >> 1], &recp, ctx))
            goto err;

        /* move the 'window' down further */
        wstart -= wend + 1;
        wvalue = 0;
        start = 0;
        if (wstart < 0)
            break;
    }
    ret = 1;
 err:
    BN_CTX_end(ctx);
    BN_RECP_CTX_free(&recp);
    bn_check_top(r);
    return ret;
}

int BN_mod_exp_mont(BIGNUM *rr, const BIGNUM *a, const BIGNUM *p,
                    const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *in_mont)
{
    int i, j, bits, ret = 0, wstart, wend, window, wvalue;
    int start = 1;
    BIGNUM *d, *r;
    const BIGNUM *aa;
    /* Table of variables obtained from 'ctx' */
    BIGNUM *val[TABLE_SIZE];
    BN_MONT_CTX *mont = NULL;

    if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(a, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(m, BN_FLG_CONSTTIME) != 0) {
        return BN_mod_exp_mont_consttime(rr, a, p, m, ctx, in_mont);
    }

    bn_check_top(a);
    bn_check_top(p);
    bn_check_top(m);

    if (!BN_is_odd(m)) {
        ERR_raise(ERR_LIB_BN, BN_R_CALLED_WITH_EVEN_MODULUS);
        return 0;
    }
    bits = BN_num_bits(p);
    if (bits == 0) {
        /* x**0 mod 1, or x**0 mod -1 is still zero. */
        if (BN_abs_is_word(m, 1)) {
            ret = 1;
            BN_zero(rr);
        } else {
            ret = BN_one(rr);
        }
        return ret;
    }

    BN_CTX_start(ctx);
    d = BN_CTX_get(ctx);
    r = BN_CTX_get(ctx);
    val[0] = BN_CTX_get(ctx);
    if (val[0] == NULL)
        goto err;

    /*
     * If this is not done, things will break in the montgomery part
     */

    if (in_mont != NULL)
        mont = in_mont;
    else {
        if ((mont = BN_MONT_CTX_new()) == NULL)
            goto err;
        if (!BN_MONT_CTX_set(mont, m, ctx))
            goto err;
    }

    if (a->neg || BN_ucmp(a, m) >= 0) {
        if (!BN_nnmod(val[0], a, m, ctx))
            goto err;
        aa = val[0];
    } else
        aa = a;
    if (!bn_to_mont_fixed_top(val[0], aa, mont, ctx))
        goto err;               /* 1 */

    window = BN_window_bits_for_exponent_size(bits);
    if (window > 1) {
        if (!bn_mul_mont_fixed_top(d, val[0], val[0], mont, ctx))
            goto err;           /* 2 */
        j = 1 << (window - 1);
        for (i = 1; i < j; i++) {
            if (((val[i] = BN_CTX_get(ctx)) == NULL) ||
                !bn_mul_mont_fixed_top(val[i], val[i - 1], d, mont, ctx))
                goto err;
        }
    }

    start = 1;                  /* This is used to avoid multiplication etc
                                 * when there is only the value '1' in the
                                 * buffer. */
    wvalue = 0;                 /* The 'value' of the window */
    wstart = bits - 1;          /* The top bit of the window */
    wend = 0;                   /* The bottom bit of the window */

#if 1 /* by Shay Gueron's suggestion */
    j = m->top;                 /* borrow j */
    if (m->d[j - 1] & (((BN_ULONG)1) << (BN_BITS2 - 1))) {
        if (bn_wexpand(r, j) == NULL)
            goto err;
        /* 2^(top*BN_BITS2) - m */
        r->d[0] = (0 - m->d[0]) & BN_MASK2;
        for (i = 1; i < j; i++)
            r->d[i] = (~m->d[i]) & BN_MASK2;
        r->top = j;
        r->flags |= BN_FLG_FIXED_TOP;
    } else
#endif
    if (!bn_to_mont_fixed_top(r, BN_value_one(), mont, ctx))
        goto err;
    for (;;) {
        if (BN_is_bit_set(p, wstart) == 0) {
            if (!start) {
                if (!bn_mul_mont_fixed_top(r, r, r, mont, ctx))
                    goto err;
            }
            if (wstart == 0)
                break;
            wstart--;
            continue;
        }
        /*
         * We now have wstart on a 'set' bit, we now need to work out how bit
         * a window to do.  To do this we need to scan forward until the last
         * set bit before the end of the window
         */
        wvalue = 1;
        wend = 0;
        for (i = 1; i < window; i++) {
            if (wstart - i < 0)
                break;
            if (BN_is_bit_set(p, wstart - i)) {
                wvalue <<= (i - wend);
                wvalue |= 1;
                wend = i;
            }
        }

        /* wend is the size of the current window */
        j = wend + 1;
        /* add the 'bytes above' */
        if (!start)
            for (i = 0; i < j; i++) {
                if (!bn_mul_mont_fixed_top(r, r, r, mont, ctx))
                    goto err;
            }

        /* wvalue will be an odd number < 2^window */
        if (!bn_mul_mont_fixed_top(r, r, val[wvalue >> 1], mont, ctx))
            goto err;

        /* move the 'window' down further */
        wstart -= wend + 1;
        wvalue = 0;
        start = 0;
        if (wstart < 0)
            break;
    }
    /*
     * Done with zero-padded intermediate BIGNUMs. Final BN_from_montgomery
     * removes padding [if any] and makes return value suitable for public
     * API consumer.
     */
#if defined(SPARC_T4_MONT)
    if (OPENSSL_sparcv9cap_P[0] & (SPARCV9_VIS3 | SPARCV9_PREFER_FPU)) {
        j = mont->N.top;        /* borrow j */
        val[0]->d[0] = 1;       /* borrow val[0] */
        for (i = 1; i < j; i++)
            val[0]->d[i] = 0;
        val[0]->top = j;
        if (!BN_mod_mul_montgomery(rr, r, val[0], mont, ctx))
            goto err;
    } else
#endif
    if (!BN_from_montgomery(rr, r, mont, ctx))
        goto err;
    ret = 1;
 err:
    if (in_mont == NULL)
        BN_MONT_CTX_free(mont);
    BN_CTX_end(ctx);
    bn_check_top(rr);
    return ret;
}

#endif

} // namespace mympz