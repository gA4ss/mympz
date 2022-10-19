#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{

  montgomery_ctx_t montgomery_ctx_create(const bignum_t &m)
  {
    montgomery_ctx_t mctx;
    init_montgomery_ctx(mctx);

    if (is_zero(m))
    {
      mympz_divide_by_zero(m);
    }

    // 设置模数
    mctx.N = m;

    //
    // 设置R
    //
    mctx.ri = bignum_bits(mctx.N); // R的位数应与模数N的位数相同
    zero(mctx.RR);
    set_bit(mctx.RR, mctx.ri); // R = 2^ri
    bignum_t &R = mctx.RR;     // 将RR当作一个临时变量

    //
    // 求R的模拟
    // Ri = R^-1 mod N
    //
    bignum_t Ri = mod_inverse(R, mctx.N);
    if (is_null(Ri))
    {
      clear_montgomery_ctx(mctx);
      return mctx;
    }

    //
    // 计算 R*Ri-1
    // Ri左移ri位，相当于乘以R
    // R的模逆减去1
    //
    Ri = lshift(Ri, mctx.ri);
    Ri = sub(Ri, 1);

    // 计算 Ni = (R*Ri-1) / N
    mctx.Ni = div(Ri, mctx.N).first;

    /* 设置RR */
    zero(mctx.RR);
    set_bit(mctx.RR, mctx.ri * 2); // RR = 2^{2 * ri}

    // RR = RR % N
    mctx.RR = mod(mctx.RR, mctx.N);

    // 将比模数N小且大于R的部分清0
    for (size_t i = bn_size(mctx.RR), ret = bn_size(mctx.N); i < ret; i++)
      mctx.RR.number[i] = 0;
    return mctx;
  }

  bignum_t montgomery_reduction(const bignum_t &x, const montgomery_ctx_t &mctx)
  {
    bignum_t t1 = x;
    mask_bits(t1, mctx.ri);
    bignum_t t2 = mul(t1, mctx.Ni);
    mask_bits(t2, mctx.ri);

    t1 = mul(t2, mctx.N);
    t2 = add(x, t1);
    bignum_t y = rshift(t2, mctx.ri);

    if (ucmp(y, mctx.N) >= 0)
    {
      y = usub(y, mctx.N);
    }

    clear_head_zero(y);
    return y;
  }

  bignum_t montgomery_mod_mul(const bignum_t &x, const bignum_t &y, const montgomery_ctx_t &mctx)
  {
    size_t num = bn_size(mctx.N);
    bignum_t z;

    if ((bn_size(x) + bn_size(y)) > 2 * num)
    {
      mympz_mod_mul_montgomery_invalid_size(bn_size(x), bn_size(y), 2 * num);
    }

    if (cmp(x, y) == 0)
    {
      z = sqr(x);
    }
    else
    {
      z = mul(x, y);
    }
    /* 从 xRR 约减到 xR */
    z = montgomery_reduction(z, mctx);
    return z;
  }

#if 0
  bignum_t montgomery_mod_exp(const bignum_t &x, const bignum_t &p,
                              const bignum_t &m, const montgomery_ctx_t &mctx)
  {
    int i, j, bits, ret = 0, wstart, wend, window, wvalue;
    int start = 1;
    BIGNUM *d, *r;
    const BIGNUM *aa;
    /* Table of variables obtained from 'ctx' */
    BIGNUM *val[TABLE_SIZE];
    BN_MONT_CTX *mont = NULL;

    if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0 || BN_get_flags(a, BN_FLG_CONSTTIME) != 0 || BN_get_flags(m, BN_FLG_CONSTTIME) != 0)
    {
      return BN_mod_exp_mont_consttime(rr, a, p, m, ctx, in_mont);
    }

    bn_check_top(a);
    bn_check_top(p);
    bn_check_top(m);

    if (!BN_is_odd(m))
    {
      ERR_raise(ERR_LIB_BN, BN_R_CALLED_WITH_EVEN_MODULUS);
      return 0;
    }
    bits = BN_num_bits(p);
    if (bits == 0)
    {
      /* x**0 mod 1, or x**0 mod -1 is still zero. */
      if (BN_abs_is_word(m, 1))
      {
        ret = 1;
        BN_zero(rr);
      }
      else
      {
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
    else
    {
      if ((mont = BN_MONT_CTX_new()) == NULL)
        goto err;
      if (!BN_MONT_CTX_set(mont, m, ctx))
        goto err;
    }

    if (a->neg || BN_ucmp(a, m) >= 0)
    {
      if (!BN_nnmod(val[0], a, m, ctx))
        goto err;
      aa = val[0];
    }
    else
      aa = a;
    if (!bn_to_mont_fixed_top(val[0], aa, mont, ctx))
      goto err; /* 1 */

    window = BN_window_bits_for_exponent_size(bits);
    if (window > 1)
    {
      if (!bn_mul_mont_fixed_top(d, val[0], val[0], mont, ctx))
        goto err; /* 2 */
      j = 1 << (window - 1);
      for (i = 1; i < j; i++)
      {
        if (((val[i] = BN_CTX_get(ctx)) == NULL) ||
            !bn_mul_mont_fixed_top(val[i], val[i - 1], d, mont, ctx))
          goto err;
      }
    }

    start = 1;         /* This is used to avoid multiplication etc
                        * when there is only the value '1' in the
                        * buffer. */
    wvalue = 0;        /* The 'value' of the window */
    wstart = bits - 1; /* The top bit of the window */
    wend = 0;          /* The bottom bit of the window */

#if 1           /* by Shay Gueron's suggestion */
    j = m->top; /* borrow j */
    if (m->d[j - 1] & (((BN_ULONG)1) << (BN_BITS2 - 1)))
    {
      if (bn_wexpand(r, j) == NULL)
        goto err;
      /* 2^(top*BN_BITS2) - m */
      r->d[0] = (0 - m->d[0]) & BN_MASK2;
      for (i = 1; i < j; i++)
        r->d[i] = (~m->d[i]) & BN_MASK2;
      r->top = j;
      r->flags |= BN_FLG_FIXED_TOP;
    }
    else
#endif
        if (!bn_to_mont_fixed_top(r, BN_value_one(), mont, ctx))
      goto err;
    for (;;)
    {
      if (BN_is_bit_set(p, wstart) == 0)
      {
        if (!start)
        {
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
      for (i = 1; i < window; i++)
      {
        if (wstart - i < 0)
          break;
        if (BN_is_bit_set(p, wstart - i))
        {
          wvalue <<= (i - wend);
          wvalue |= 1;
          wend = i;
        }
      }

      /* wend is the size of the current window */
      j = wend + 1;
      /* add the 'bytes above' */
      if (!start)
        for (i = 0; i < j; i++)
        {
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
    if (OPENSSL_sparcv9cap_P[0] & (SPARCV9_VIS3 | SPARCV9_PREFER_FPU))
    {
      j = mont->N.top;  /* borrow j */
      val[0]->d[0] = 1; /* borrow val[0] */
      for (i = 1; i < j; i++)
        val[0]->d[i] = 0;
      val[0]->top = j;
      if (!BN_mod_mul_montgomery(rr, r, val[0], mont, ctx))
        goto err;
    }
    else
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