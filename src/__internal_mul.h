//
// 交叉相乘
// 结果的低位保存在"l"，高位保存在"h"。
//
#define mul64(l, h, bl, bh)         \
  {                                 \
    UNIT_ULONG m, m1, lt, ht;       \
    lt = l;                         \
    ht = h;                         \
    m = (bh) * (lt);                \
    lt = (bl) * (lt);               \
    m1 = (bl) * (ht);               \
    ht = (bh) * (ht);               \
    m = (m + m1) & CALC_MASK;       \
    if (m < m1)                     \
      ht += L2HBITS((UNIT_ULONG)1); \
    ht += HBITS(m);                 \
    m1 = L2HBITS(m);                \
    lt = (lt + m1) & CALC_MASK;     \
    if (lt < m1)                    \
      ht++;                         \
    (l) = lt;                       \
    (h) = ht;                       \
  }

//
// mul_add_c(a,b,c0,c1,c2)  -- c+=a*b 三位数：c=(c2,c1,c0)
// 对宏变量hi的增加不会产生溢出，因为乘法结果的高位不可
// 能全是1。
//
#define mul_add_c(a,b,c0,c1,c2)   do {        \
  UNIT_ULONG lo = LBITS(a), hi = HBITS(a);    \
  UNIT_ULONG bl = LBITS(b), bh = HBITS(b);    \
  mul64(lo,hi,bl,bh);                         \
  c0 = (c0+lo)&CALC_MASK; if (c0<lo) hi++;    \
  c1 = (c1+hi)&CALC_MASK; if (c1<hi) c2++;    \
} while(0)

//
// mul_add_c2(a,b,c0,c1,c2) -- c+=2*a*b 三位数：c=(c2,c1,c0)
//
#define mul_add_c2(a,b,c0,c1,c2)  do {        \
  UNIT_ULONG tt;                              \
  UNIT_ULONG lo = LBITS(a), hi = HBITS(a);    \
  UNIT_ULONG bl = LBITS(b), bh = HBITS(b);    \
  mul64(lo,hi,bl,bh);                         \
  tt = hi;                                    \
  c0 = (c0+lo)&CALC_MASK; if (c0<lo) tt++;    \
  c1 = (c1+tt)&CALC_MASK; if (c1<tt) c2++;    \
  c0 = (c0+lo)&CALC_MASK; if (c0<lo) hi++;    \
  c1 = (c1+hi)&CALC_MASK; if (c1<hi) c2++;    \
} while(0)

//
// a 乘以 b(bl,bh)加上进位，最后再加上
// r的结果，高位放到上，低位放到r上。
//
#define mul_add(r, a, bl, bh, c) \
  {                              \
    UNIT_ULONG l, h;             \
                                 \
    h = (a);                     \
    l = LBITS(h);                \
    h = HBITS(h);                \
    mul64(l, h, (bl), (bh));     \
                                 \
    /* non-multiply part */      \
    l = (l + (c)) & CALC_MASK;   \
    if (l < (c))                 \
      h++;                       \
    (c) = (r);                   \
    l = (l + (c)) & CALC_MASK;   \
    if (l < (c))                 \
      h++;                       \
    (c) = h & CALC_MASK;         \
    (r) = l;                     \
  }

//
// c是进位，与mul64不同的是最终的计算
// 结果会加上c的进位数值。
//
#define mul(r, a, bl, bh, c)   \
  {                            \
    UNIT_ULONG l, h;           \
                               \
    h = (a);                   \
    l = LBITS(h);              \
    h = HBITS(h);              \
    mul64(l, h, (bl), (bh));   \
                               \
    /* non-multiply part */    \
    l += (c);                  \
    if ((l & CALC_MASK) < (c)) \
      h++;                     \
    (c) = h & CALC_MASK;       \
    (r) = l & CALC_MASK;       \
  }
