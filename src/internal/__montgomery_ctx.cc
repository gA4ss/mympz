#define __init_montgomery_ctx(mctx) \
  {                                 \
    (mctx).ri = 0;                  \
    (mctx).n0[0] = 0;               \
    (mctx).n0[1] = 0;               \
  }

void __clear_montgomery_ctx(const montgomery_ctx_t &mctx)
{
  mctx.ri = 0;
  mctx.n0[0] = mctx.n0[1] = 0;
  mctx.N.clear();
  mctx.Ni.clear();
  mctx.RR.clear();
}

montgomery_ctx_t montgomery_ctx_create(const number_t &m)
{
  montgomery_ctx_t mctx;
  __init_montgomery_ctx(mctx);

  if (__is_zero(m))
  {
    mympz_divide_by_zero(m);
  }

  // 设置模数
  mctx.N = m;

  //
  // 设置R
  //
  mctx.ri = __number_bits(mctx.N);              // R的位数应与模数N的位数相同
  mctx.RR.clear(); mctx.RR.push_back(0);
  __set_bit(mctx.RR, mctx.ri);                  // R = 2^ri
  number_ptr_t R = num_ptr(mctx.RR);            // 将RR当作一个临时变量

  //
  // 求R的模拟
  // Ri = R^-1 mod N
  //
  if ((BN_mod_inverse(Ri, R, &mont->N, ctx)) == NULL)
    goto err;

  //
  // 计算 R*Ri-1
  // Ri左移ri位，相当于乘以R
  // R的模逆减去1
  //
  BN_lshift(Ri, Ri, mont->ri)
  BN_sub_word(Ri, 1)

  // 计算 Ni = (R*Ri-1) / N
  if (!BN_div(&(mont->Ni), NULL, Ri, &mont->N, ctx))
    goto err;

  /* 设置RR */
  __zero(mctx.RR);
  __set_bit(mctx.RR, mctx.ri * 2);        // RR = 2^{2 * ri}

  // RR = RR % N
  if (!BN_mod(&(mont->RR), &(mont->RR), &(mont->N), ctx))
    goto err;

  // 将比模数N小又大于
  for (size_t i = num_size(mctx.RR), ret = num_size(mctx.N); i < ret; i++)
    mctx.RR[i] = 0;
  return mctx;
}