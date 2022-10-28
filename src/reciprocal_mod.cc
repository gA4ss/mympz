#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{

  //
  // len是结果的期望大小，我们实际上用一个额外的值来计算
  // 字的精度，所以如果余数不是，我们可以做更快的除法要求。
  //
  /* r := 2^len / m */
  bignum_t reciprocal(const bignum_t &m, size_t len)
  {
    bignum_t t; init_bignum(t);
    set_bit(t, len);
    bignum_t r = idiv(t, m);
    return r;
  }

  reciprocal_ctx_t reciprocal_set(const bignum_t &d)
  {
    reciprocal_ctx_t rctx;
    init_reciprocal_ctx(rctx);

    rctx.N = d;
    zero(rctx.Nr);

    rctx.num_bits = bignum_bits(d);
    rctx.shift = 0;
    return rctx;
  }

  bignum_t reciprocal_mod_mul(const bignum_t &x, const bignum_t *y, reciprocal_ctx_t &rctx)
  {
    bignum_t z;
    if (y)
    {
      if (ucmp(x, *y) == 0)
      {
        z = sqr(x);
        z.neg = x.neg ^ y->neg;
      }
      else
      {
        z = mul(x, *y);
      }
    }
    else
    {
      z = x;
    }

    division_result_t r = reciprocal_div(z, rctx);
    return r.second;
  }

  division_result_t reciprocal_div(const bignum_t &m, reciprocal_ctx_t &rctx)
  {
    bignum_t d;

    if (ucmp(m, rctx.N) < 0)
    {
      zero(d);
      return {d, m};
    }

    // 给定输入ABCDEF / ab，我们需要相乘ABCDEF乘以ab的倒数的3位
    // i := max(bignum_bits(m), 2*bignum_bits(N))

    size_t i = bignum_bits(m);
    size_t j = rctx.num_bits << 1;
    if (j > i)
      i = j;

    // Nr := round(2^i / N)
    if (i != rctx.shift)
    {
      rctx.Nr = reciprocal(rctx.N, i);
      rctx.shift = i;
    }

    //
    // d := |round(round(m / 2^bignum_bits(N)) * rctx.Nr / 2^(i - bignum_bits(N)))|
    //    = |round(round(m / 2^bignum_bits(N)) * round(2^i / N) / 2^(i - bignum_bits(N)))|
    //    <= |(m / 2^bignum_bits(N)) * (2^i / N) * (2^bignum_bits(N) / 2^i)|
    //    = |m/N|
    //
    bignum_t a = rshift(m, rctx.num_bits);
    bignum_t b = mul(a, rctx.Nr);
    d = rshift(b, i - rctx.num_bits);
    d.neg = 0;

    b = mul(rctx.N, d);
    bignum_t r = usub(m, b);
    r.neg = 0;

    j = 0;
    while (ucmp(r, rctx.N) >= 0)
    {
      if (j++ > 2)
      {
        mympz_reciprocal_is_bad(r);
      }

      r = usub(r, rctx.N);
      d = add(d, 1);
    }

    r.neg = is_zero(r) ? 0 : m.neg;
    d.neg = m.neg ^ rctx.N.neg;

    return {d, r};
  }

  bignum_t reciprocal_mod_exp(const bignum_t &x, const bignum_t &p, const bignum_t &m)
  {
    bignum_t r;
    bignum_t val[MODEXP_TABLE_SIZE];
    reciprocal_ctx_t rctx;
    init_reciprocal_ctx(rctx);

    size_t bits = bignum_bits(p);
    if (bits == 0)
    {
      /* x**0 mod 1, 或者 x**0 mod -1 仍然为0。 */
      if (is_abs_word(m, 1))
      {
        return const_0;
      }
      else
      {
        return const_1;
      }
    }

    if (m.neg)
    {
      /* 忽略'm'的符号 */
      bignum_t t = m;
      t.neg = 0;
      rctx = reciprocal_set(t);
    }
    else
    {
      rctx = reciprocal_set(m);
    }

    val[0] = nnmod(x, m);
    if (is_zero(val[0]))
    {
      return const_0;
    }

    size_t window = window_bits_for_exponent_size(bits);
    if (window > 1)
    {
      bignum_t xx = reciprocal_mod_mul(val[0], &val[0], rctx);
      size_t j = 1 << (window - 1);
      for (size_t i = 1; i < j; i++)
      {
        val[i] = reciprocal_mod_mul(val[i - 1], &xx, rctx);
      }
    }

    size_t start = 1;
    size_t wvalue = 0, wend = 0;
    int wstart = static_cast<int>(bits - 1);

    one(r);
    for (;;)
    {
      if (is_bit_set(p, wstart) == 0)
      {
        if (!start)
          r = reciprocal_mod_mul(r, &r, rctx);
        if (wstart == 0)
          break;
        wstart--;
        continue;
      }

      wvalue = 1;
      wend = 0;
      for (size_t i = 1; i < window; i++)
      {
        if (wstart - i < 0)
          break;
        if (is_bit_set(p, wstart - i))
        {
          wvalue <<= (i - wend);
          wvalue |= 1;
          wend = i;
        }
      }

      size_t j = wend + 1;
      if (!start)
        for (size_t i = 0; i < j; i++)
        {
          r = reciprocal_mod_mul(r, &r, rctx);
        }

      r = reciprocal_mod_mul(r, &val[wvalue >> 1], rctx);

      wstart -= static_cast<int>(wend + 1);
      wvalue = 0;
      start = 0;
      if (wstart < 0)
        break;
    }

    return r;
  }

} // namespace mympz