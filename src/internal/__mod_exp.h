/* 设窗口大小'w'(w >= 2)，指数的位数为'b'。
 *    2^(w-1) + (b-w)/(w+1);
 * 这里的 2^(w-1)是预计算的表，(b-w)/(w+1)是一个近似w位窗口的期望数量。
 *
 * 因此我们有
 *    w >= 6  if        b > 671
 *     w = 5  if  671 > b > 239
 *     w = 4  if  239 > b >  79
 *     w = 3  if   79 > b >  23
 *    w <= 2  if   23 > b
 */
#define window_bits_for_exponent_size(b) \
  ((b) > 671 ? 6 : (b) > 239 ? 5         \
               : (b) > 79    ? 4         \
               : (b) > 23    ? 3         \
                             : 1)

#define MODEXP_TABLE_SIZE 32

  typedef struct
  {
    size_t ri;   /* R的位数 */
    bignum_t RR; /* 用来转换成蒙哥马利形式，可能是零填充 */
    bignum_t N;  /* 模数 */
    bignum_t Ni; /* R*(1/R mod N) - N*Ni = 1) */
  } montgomery_ctx_t;

#define init_montgomery_ctx(mctx) \
  {                               \
    (mctx).ri = 0;                \
    init_bignum((mctx).RR);       \
    init_bignum((mctx).N);        \
    init_bignum((mctx).RR);       \
  }

#define clear_montgomery_ctx(mctx)                    \
  {                                                   \
    (mctx).ri = 0;                                    \
    (mctx).N.number.clear();                          \
    (mctx).Ni.number.clear();                         \
    (mctx).RR.number.clear();                         \
    (mctx).N.neg = (mctx).Ni.neg = (mctx).RR.neg = 0; \
  }

  montgomery_ctx_t montgomery_ctx_create(const bignum_t &m);
  bignum_t bignum_to_montnum(const bignum_t &x, const montgomery_ctx_t &mctx);
  bignum_t montgomery_reduction(const bignum_t &x, const montgomery_ctx_t &mctx);
  bignum_t montgomery_mod_mul(const bignum_t &x, const bignum_t &y, const montgomery_ctx_t &mctx);
  bignum_t montgomery_mod_exp(const bignum_t &x, const bignum_t &p,
                              const bignum_t &m, const montgomery_ctx_t *pmctx = nullptr);

typedef struct {
  bignum_t N;                   // 除数
  bignum_t Nr;                  // 倒数
  size_t num_bits;
  size_t shift;
} reciprocal_ctx_t;

#define init_reciprocal_ctx(rctx) \
  {                               \
    (rctx).num_bits = 0;          \
    (rctx).shift = 0;             \
    init_bignum((rctx).N);        \
    init_bignum((rctx).Nr);       \
  }

#define clear_reciprocal_ctx(rctx)                    \
  {                                                   \
    (rctx).num_bits = 0;                              \
    (rctx).shift = 0;                                 \
    (rctx).N.number.clear();                          \
    (rctx).Ni.number.clear();                         \
  }

  bignum_t reciprocal(const bignum_t &m, size_t len);
  reciprocal_ctx_t reciprocal_set(const bignum_t &d);
  division_result_t reciprocal_div(const bignum_t &m, reciprocal_ctx_t &rctx);
  bignum_t reciprocal_mod_mul(const bignum_t &x, const bignum_t *y, reciprocal_ctx_t &rctx);
  bignum_t reciprocal_mod_exp(const bignum_t &x, const bignum_t &p, const bignum_t &m);