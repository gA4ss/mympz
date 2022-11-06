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

  bignum_t bignum_to_montnum(const bignum_t &x, const montgomery_ctx_t &mctx)
  {
    return montgomery_mod_mul(x, mctx.RR, mctx);
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

  bignum_t montgomery_mod_exp(const bignum_t &x, const bignum_t &p,
                              const bignum_t &m, const montgomery_ctx_t *pmctx)
  {
    bignum_t r;
    bignum_t val[MODEXP_TABLE_SIZE];
    const bignum_t const_0 = create(0);
    const bignum_t const_1 = create(1);

    if (!is_odd(m))
    {
      mympz_mod_exp_montgomery_modulo_is_even(m);
    }

    //
    // 指数为0的情况。
    //
    size_t bits = bignum_bits(p);
    if (bits == 0)
    {
      /* x**0 mod 1, 或者 x**0 mod -1 仍然是0. */
      if (is_abs_word(m, 1))
      {
        return const_0;
      }
      else
      {
        return const_1;
      }
      return r;
    }

    //
    // 构建蒙哥马利模数
    //
    montgomery_ctx_t mctx;
    if (pmctx)
    {
      mctx = *pmctx;
    }
    else
    {
      mctx = montgomery_ctx_create(m);
    }

    //
    // 缩减x
    //
    if (x.neg || ucmp(x, m) >= 0)
    {
      val[0] = nnmod(x, m);
      val[0] = bignum_to_montnum(val[0], mctx);
    }
    else
    {
      val[0] = bignum_to_montnum(x, mctx);
    }

    size_t window = window_bits_for_exponent_size(bits);
    if (window > 1)
    {
      bignum_t d = montgomery_mod_mul(val[0], val[0], mctx);
      size_t j = 1 << (window - 1);
      for (size_t i = 1; i < j; i++)
      {
        val[i] = montgomery_mod_mul(val[i - 1], d, mctx);
      }
    }

    size_t start = 1;
    size_t wvalue = 0, wend = 0;
    int wstart = static_cast<int>(bits - 1);

    size_t j = bn_size(m);
    if (m.number[j - 1] & (((UNIT_ULONG)1) << (UNIT_BITS - 1)))
    {
      bn_resize(r, j);

      /* 2^(top*UNIT_BITS) - m */
      r.number[0] = (0 - m.number[0]) & CALC_MASK;
      for (size_t i = 1; i < j; i++)
        r.number[i] = (~m.number[i]) & CALC_MASK;
    }
    else
    {
      r = bignum_to_montnum(const_1, mctx);
    }

    for (;;)
    {
      if (is_bit_set(p, wstart) == 0)
      {
        if (!start)
        {
          r = montgomery_mod_mul(r, r, mctx);
        }
        if (wstart == 0)
          break;
        wstart--;
        continue;
      }

      // 我们现在有wstart在'set'位上，我们现在需要计算出是多少位
      // 一个可以做的窗口。要做到这一点，我们需要向前扫描到最后
      // 设置窗口结束前的位
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

      // wend是当前窗口的长度
      j = wend + 1;
      // 增加以上的字节
      if (!start)
        for (size_t i = 0; i < j; i++)
        {
          r = montgomery_mod_mul(r, r, mctx);
        }

      // wvalue将是一个奇数 < 2^window
      r = montgomery_mod_mul(r, val[wvalue >> 1], mctx);

      // 将“窗口”进一步向下移动
      wstart -= static_cast<int>(wend + 1);
      wvalue = 0;
      start = 0;
      if (wstart < 0)
        break;
    }

    r = montgomery_reduction(r, mctx);
    return r;
  }

} // namespace mympz