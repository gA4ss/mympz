#ifndef MYMPZ_INTERNAL_H_
#define MYMPZ_INTERNAL_H_

#include "mympz.h"

namespace mympz {

#define UNIT_STRING      "18446744073709551616"
#define UNIT_ULONG       unit_t
#define UNIT_BYTES       8
#define UNIT_BITS        64
#define UNIT_HALF_BITS   UNIT_BITS / 2
// #define UNIT_BITS2       (UNIT_BYTES * 8)
// #define UNIT_BITS        (UNIT_BITS2 * 2)
// #define UNIT_TBIT        ((UNIT_ULONG)1 << (UNIT_BITS2 - 1))

#define CALC_MASK        (0xffffffffffffffffL)
#define CALC_MASKl       (0xffffffffL)
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
// # define CALC_SQR_RECURSIVE_SIZE_NORMAL            (16)/* 32 */
// # define CALC_MUL_LOW_RECURSIVE_SIZE_NORMAL        (32)/* 32 */
// # define CALC_MONT_CTX_SET_SIZE_WORD               (64)/* 32 */

//
// 一些乘法宏的内部支持
//
#include "__internal_mul.h"

size_t __count_bits(unit_t l);

int __cmp(const number_t& x, const number_t& y, int n);
int __cmp_part(const number_t& x, const number_t& y, int cl, int dl);

unit_t __add_units(number_t& z, const number_t &x, const number_t &y, size_t n);
unit_t __sub_units(number_t& z, const number_t &x, const number_t &y, size_t n);

void __mul_4_units(number_t& z, const number_t& x, const number_t& y);
void __mul_8_units(number_t& z, const number_t& x, const number_t& y);
void __mul_units_loop(number_t& z, const number_t &x, const number_t &y);


} // namespace mympz

#endif // MYMPZ_INTERNAL_H_