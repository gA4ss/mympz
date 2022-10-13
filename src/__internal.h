#ifndef MYMPZ_INTERNAL_H_
#define MYMPZ_INTERNAL_H_

#include "mympz.h"

namespace mympz
{

#ifdef ARCH_64BITS

#define UNIT_STRING "18446744073709551616"
#define UNIT_ULONG unit_t
#define UNIT_BYTES 8
#define UNIT_HALF_BYTES 4
#define UNIT_BITS 64
#define UNIT_HALF_BITS 32
#define UNIT_BITS2 (UNIT_BYTES * 8)
// #define UNIT_BITS        (UNIT_BITS2 * 2)
#define UNIT_TBIT ((UNIT_ULONG)1 << (UNIT_BITS2 - 1))

#define CALC_MASK (0xffffffffffffffffL)
#define CALC_MASKl (0x00000000ffffffffL)
#define CALC_MASKh (0xffffffff00000000L)
#define CALC_MASKh1 (0xffffffff80000000L)

#else // ARCH_32BITS

#define UNIT_STRING "4294967296"
#define UNIT_ULONG unit_t
#define UNIT_BYTES 4
#define UNIT_HALF_BYTES 2
#define UNIT_BITS 32
#define UNIT_HALF_BITS 16
#define UNIT_BITS2 (UNIT_BYTES * 8)
// #define UNIT_BITS        (UNIT_BITS2 * 2)
#define UNIT_TBIT ((UNIT_ULONG)1 << (UNIT_BITS2 - 1))

#define CALC_MASK (0xffffffffL)
#define CALC_MASKl (0x0000ffffL)
#define CALC_MASKh (0xffff0000L)
#define CALC_MASKh1 (0xffff8000L)

#endif

#define LBITS(a) ((a)&CALC_MASKl)
#define HBITS(a) (((a) >> UNIT_HALF_BITS) & CALC_MASKl)
#define L2HBITS(a) (((a) << UNIT_HALF_BITS) & CALC_MASK)

  //#define SMALL_FOOTPRINT  1        ///< 在运算时进行单步运算，便于观察运算。

#ifndef SMALL_FOOTPRINT
#define CALC_MUL_COMBA
#define CALC_SQR_COMBA
#define CALC_RECURSION
#endif

/* Pentium pro 16,16,16,32,64 */
/* Alpha       16,16,16,16.64 */
#define CALC_MULL_SIZE_NORMAL (16)              /* 32 */
#define CALC_MUL_RECURSIVE_SIZE_NORMAL (16)     /* 32 less than */
#define CALC_SQR_RECURSIVE_SIZE_NORMAL (16)     /* 32 */
#define CALC_MUL_LOW_RECURSIVE_SIZE_NORMAL (32) /* 32 */
#define CALC_MONT_CTX_SET_SIZE_WORD (64)        /* 32 */

//
// 定义128位的辅助运算类型
//
#ifdef CALC_INT128_OPTIMIZE
  typedef __uint128_t dunit_t;
#endif

#define clear_number_head_zero(x)    \
  {                                  \
    size_t __l = (x).size();         \
    while (__l && (x)[__l - 1] == 0) \
    {                                \
      --__l;                         \
    }                                \
    if (__l == 0)                    \
      __l++;                         \
    (x).resize(__l);                 \
  }

//
// 内部大数的支持
//
#include "__internal_bn.h"

  void __zero(number_t &x);
  void __zero(const number_ptr_t &x, size_t xl);
  bool __is_zero(const number_t &x);
  bool __is_zero(const number_ptr_t &x, size_t xl);

  size_t __count_bits(unit_t l);
  size_t __number_bits(const number_t &x);
#define __number_bytes(x) ((__number_bits(x) + 7) / 8)

  void __set_bit(number_t &x, size_t n);
  int __is_bit_set(const number_t &x, size_t n);
  void __clear_bit(number_t &x, size_t n);
  void __mask_bits(number_t &x, size_t n);

  size_t __left_align(const number_ptr_t &x, size_t xl);
  void __consttime_swap(unit_t condition,
                        const number_ptr_t &x, size_t *xl, int *xneg,
                        const number_ptr_t &y, size_t *yl, int *yneg,
                        size_t nwords);
  int __cmp(const number_ptr_t &x, const number_ptr_t &y, int n);
  int __cmp_part(const number_ptr_t &x, const number_ptr_t &y, int cl, int dl);

  unit_t __add_units(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, size_t n);
  number_t __add_units(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl);
  unit_t __sub_units(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, size_t n);
  number_t __sub_units(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl);

  unit_t __mul_add_units(const number_ptr_t &z, const number_ptr_t &x, size_t nx,
                         const unit_t &y);
  unit_t __mul_units_unit(const number_ptr_t &z, const number_ptr_t &x, size_t nx, unit_t y);
  void __mul_4_units(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y);
  void __mul_8_units(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y);
  void __mul_units_loop(const number_ptr_t &z, const number_ptr_t &x, size_t nx, const number_ptr_t &y,
                        size_t ny);
  void __mul_units_low_loop(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, int n);
  void __mul_units_recursive(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y,
                             int n2, int dnx, int dny, const number_ptr_t &t);
  void __mul_part_units_recursive(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, int n,
                                  int tnx, int tny, const number_ptr_t &t);
  void __mul_units_low_recursive(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, int n2,
                                 const number_ptr_t &t);
  number_t __mul_units(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl);

  void __sqr_4_units(const number_ptr_t &y, const number_ptr_t &x);
  void __sqr_8_units(const number_ptr_t &y, const number_ptr_t &x);
  void __sqr_units(const number_ptr_t &y, const number_ptr_t &x, size_t n);
  void __sqr_units_loop(const number_ptr_t &y, const number_ptr_t &x, size_t nx,
                        const number_ptr_t &t);
  void __sqr_units_recursive(const number_ptr_t &y, const number_ptr_t &x, size_t n2,
                             const number_ptr_t &t);

  number_t __div_units_unit(const number_ptr_t &x, size_t xl, unit_t w, unit_t *r);
  number_t __div_units(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl);

  number_t __lshift1(const number_ptr_t &x, size_t xl);
  number_t __rshift1(const number_ptr_t &x, size_t xl);
  number_t __lshift(const number_ptr_t &x, size_t xl, size_t n);
  number_t __rshift(const number_ptr_t &x, size_t xl, size_t n);

  number_t __mod(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl);
  number_t __mod_add(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl,
                     const number_ptr_t &m, size_t ml);
  number_t __mod_sub(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl,
                     const number_ptr_t &m, size_t ml);

  number_t __gcd(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl);

  typedef struct
  {
    size_t ri;    /* R的位数 */
    number_t RR;  /* 用来转换成蒙哥马利形式，可能是零填充 */
    number_t N;   /* 模数 */
    number_t Ni;  /* R*(1/R mod N) - N*Ni = 1) */
    unit_t n0[2]; /* Ni的最低有效字 */
  } montgomery_ctx_t;

  void __clear_montgomery_ctx(const montgomery_ctx_t &mctx);
  number_t __montgomery_reduction(const number_ptr_t &x, size_t xl, const montgomery_ctx_t &mctx);
  number_t __mod_mul_montgomery(const number_ptr_t &x, size_t xl, const number_ptr_t &y, size_t yl,
                                const montgomery_ctx_t &mctx);

  number_t __bin2bn(const unsigned char *s, size_t len, int *oneg = nullptr, bool little = true, bool is_sign = false);
  size_t __bn2bin(const number_t &x, int neg, unsigned char *to, size_t tolen, bool little = true, bool is_sign = false);
  number_t __create_hex_str(const char *str);
  number_t __create_dec_str(const char *str);

  std::string __print_string_dec(const number_ptr_t &x, size_t xl);
  std::string __print_string_hex(const number_ptr_t &x, size_t xl, bool low_case = false);
  std::string __print_string(const number_ptr_t &x, size_t xl, bool hex = false, bool low_case = false);
  std::string __print_string(number_t &x, bool hex = false, bool low_case = false);

} // namespace mympz

#endif // MYMPZ_INTERNAL_H_
