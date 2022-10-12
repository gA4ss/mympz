//
// nl > 1 && xl == nl && yl == nl
//
number_t __internal_mod_mul_montgomery(const number_ptr_t &x, const number_ptr_t &y,
                                       const number_ptr_t &n, const unit_t *n0p, size_t nl)
{
  number_t z;
  num_resize(z, nl);
  number_ptr_t zp = num_ptr(z);

  unit_t c0, c1, n0 = *n0p;

  number_t v;
  num_resize(v, nl + 2);
  number_ptr_t tp = num_ptr(v);

  // for (size_t i = 0; i <= nl; i++)
  //   tp[i] = 0;

  // (x + y) * n
  for (size_t i = 0; i < nl; i++)
  {
    c0 = __mul_add_units(tp, x, nl, y[i]);
    c1 = (tp[nl] + c0) & CALC_MASK;
    tp[nl] = c1;
    tp[nl + 1] = (c1 < c0 ? 1 : 0);

    c0 = __mul_add_units(tp, n, nl, tp[0] * n0);
    c1 = (tp[nl] + c0) & CALC_MASK;
    tp[nl] = c1;
    tp[nl + 1] += (c1 < c0 ? 1 : 0);
    for (size_t j = 0; j <= nl; j++)
      tp[j] = tp[j + 1];
  }

  // t > n then t = t - n
  if (tp[nl] != 0 || tp[nl - 1] >= n[nl - 1])
  {
    c0 = __sub_units(zp, tp, n, nl);
    if (tp[nl] != 0 || c0 == 0)
    {
      return z;
    }
  }

  // t < n then t = n
  for (size_t i = 0; i < nl; i++)
    z[i] = tp[i];
  return z;
}

number_t __mod_mul_montgomery(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl,
                              const montgomery_ctx_t &mctx)
{
  number_t z;

  size_t nl = num_size(mctx.N);
  if (nl > 1 && xl == nl && yl == nl)
  {
    z = __internal_mod_mul_montgomery(x, y, mctx.N, mctx.n0, nl);
  }
  else
  {
    if ((xl + yl) > 2 * nl)
    {
      mympz_mod_mul_montgomery_invalid_size(xl, yl, nl);
    }

    //
    // 计算 x 乘以 y
    //
    number_t xy = __mul_units(x, xl, y, yl);

    //
    // 从xRR约减到xR
    //
    z = __montgomery_reduction(xy, num_size(xy), mctx);
  }

  return z;
}
