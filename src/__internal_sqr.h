#define sqr64(lo, ho, in)                           \
  {                                                 \
    unit_t l, h, m;                                 \
                                                    \
    h = (in);                                       \
    l = LBITS(h);                                   \
    h = HBITS(h);                                   \
    m = (l) * (h);                                  \
    l *= l;                                         \
    h *= h;                                         \
    h += (m & CALC_MASKh1) >> (UNIT_HALF_BITS - 1); \
    m = (m & CALC_MASKl) << (UNIT_HALF_BITS + 1);   \
    l = (l + m) & CALC_MASK;                        \
    if (l < m)                                      \
      h++;                                          \
    (lo) = l;                                       \
    (ho) = h;                                       \
  }

/* sqr_add_c(a,i,c0,c1,c2)  -- c+=a[i]^2 三位数：c=(c2,c1,c0) */
#define sqr_add_c(a, i, c0, c1, c2) \
  do                                \
  {                                 \
    unit_t lo, hi;                  \
    sqr64(lo, hi, (a)[i]);          \
    c0 = (c0 + lo) & CALC_MASK;     \
    if (c0 < lo)                    \
      hi++;                         \
    c1 = (c1 + hi) & CALC_MASK;     \
    if (c1 < hi)                    \
      c2++;                         \
  } while (0)

/*
 * sqr_add_c2(a,i,c0,c1,c2) -- c+=2*a[i]*a[j]
 * 三位数：c=(c2,c1,c0)
 */
#define sqr_add_c2(a, i, j, c0, c1, c2) \
  mul_add_c2((a)[i], (a)[j], c0, c1, c2)
