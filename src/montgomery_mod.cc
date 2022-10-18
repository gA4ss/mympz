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

} // namespace mympz