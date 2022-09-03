#ifndef MYMPZ_INTERNAL_H_
#define MYMPZ_INTERNAL_H_

#include "mympz.h"

namespace mympz {

#define UNIT_STRING      "18446744073709551616"
#define UNIT_ULONG       unit_t
#define UNIT_BYTES       8
#define UNIT_BITS        64
#define UNIT_HALF_BITS   32
#define UNIT_BITS2       (UNIT_BYTES * 8)
// #define UNIT_BITS        (UNIT_BITS2 * 2)
#define UNIT_TBIT        ((UNIT_ULONG)1 << (UNIT_BITS2 - 1))

#define CALC_MASK        (0xffffffffffffffffL)
#define CALC_MASKl       (0x00000000ffffffffL)
#define CALC_MASKh       (0xffffffff00000000L)
#define CALC_MASKh1      (0xffffffff80000000L)

#define LBITS(a)         ((a)&CALC_MASKl)
#define HBITS(a)         (((a)>>UNIT_HALF_BITS)&CALC_MASKl)
#define L2HBITS(a)       (((a)<<UNIT_HALF_BITS)&CALC_MASK)

//#define SMALL_FOOTPRINT  1        ///< 在运算时进行单步运算，便于观察运算。

/* Pentium pro 16,16,16,32,64 */
/* Alpha       16,16,16,16.64 */
#define CALC_MULL_SIZE_NORMAL                     (16)/* 32 */
#define CALC_MUL_RECURSIVE_SIZE_NORMAL            (16)/* 32 less than */
#define CALC_SQR_RECURSIVE_SIZE_NORMAL            (16)/* 32 */
#define CALC_MUL_LOW_RECURSIVE_SIZE_NORMAL        (32)/* 32 */
#define CALC_MONT_CTX_SET_SIZE_WORD               (64)/* 32 */

//
// 一些乘法宏的内部支持
//
#include "__internal_mul.h"
#include "__internal_sqr.h"

bool __is_zero(const number_t& x);

size_t __count_bits(unit_t l);
size_t __number_bits(const number_t& x);
#define __number_bytes(x) ((__number_bits(x)+7)/8)

int __cmp(const number_ptr_t& x, const number_ptr_t& y, int n);
int __cmp_part(const number_ptr_t& x, const number_ptr_t& y, int cl, int dl);

unit_t __add_units(const number_ptr_t& z, const number_ptr_t &x, const number_ptr_t &y, size_t n);
unit_t __sub_units(const number_ptr_t& z, const number_ptr_t &x, const number_ptr_t &y, size_t n);

unit_t __mul_units_unit(const number_ptr_t& z, const number_ptr_t& x, size_t nx, unit_t y);
void __mul_4_units(const number_ptr_t& z, const number_ptr_t& x, const number_ptr_t& y);
void __mul_8_units(const number_ptr_t& z, const number_ptr_t& x, const number_ptr_t& y);
void __mul_units_loop(const number_ptr_t& z, const number_ptr_t &x, size_t nx, const number_ptr_t &y, 
                      size_t ny);
void __mul_units_low_loop(const number_ptr_t& z, const number_ptr_t& x, const number_ptr_t& y, int n);
void __mul_units_recursive(const number_ptr_t& z, const number_ptr_t& x, const number_ptr_t& y,
                           int n2, int dnx, int dny, const number_ptr_t& t);
void __mul_part_units_recursive(const number_ptr_t &z, const number_ptr_t &x, const number_ptr_t &y, int n,
                                int tnx, int tny, const number_ptr_t &t);
void __mul_units_low_recursive(const number_ptr_t& z, const number_ptr_t& x, const number_ptr_t& y, int n2,
                               const number_ptr_t& t);

void __sqr_4_units(const number_ptr_t& y, const number_ptr_t& x);
void __sqr_8_units(const number_ptr_t& y, const number_ptr_t& x);
void __sqr_units(const number_ptr_t& y, const number_ptr_t& x, size_t n);
void __sqr_units_loop(const number_ptr_t& y, const number_ptr_t& x, size_t nx, 
                         const number_ptr_t& t);
void __sqr_units_recursive(const number_ptr_t& y, const number_ptr_t& x, size_t n2,
                              const number_ptr_t& t);

unit_t __div_unit(unit_t h, unit_t l, unit_t d);

number_t __mod_add(const number_ptr_t& x, size_t xl, const number_ptr_t& y, size_t yl,
                   const number_ptr_t& m, size_t ml);
number_t __mod_sub(const number_ptr_t& x, size_t xl, const number_ptr_t& y, size_t yl,
                   const number_ptr_t& m, size_t ml);

} // namespace mympz

#endif // MYMPZ_INTERNAL_H_
