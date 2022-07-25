/**
  * @file        mympz.h
  * @author      fr1140m@gmail.com
  * @version     1.0.0
  * @date        
  * @brief       
  * @attention   
  *
  *
  * @htmlonly
  * <span style="font-weight: bold">History</span>
  * @endhtmlonly
  * Version|Auther|Date|Describe
  * -------|------|----|--------
  * 
  * <h2><center>&copy;COPYRIGHT 2022 YanWenBin All Rights Reserved.</center></h2>
  */
#ifndef MYMPZ_H_
#define MYMPZ_H_

#include <cmath>
#include <algorithm>
#include <sstream>

#include <mympz/exception.h>

namespace mympz {

/**
  * @brief         大数结构的单位。
  */
typedef int64_t          sunit_t;
typedef uint64_t         unit_t;
#define UNIT_STRING      "18446744073709551616"
#define UNIT_ULONG       unsigned long
#define UNIT_BYTES       8
#define UNIT_BITS2       (UNIT_BYTES * 8)
#define UNIT_BITS        (UNIT_BITS2 * 2)
#define UNIT_TBIT        ((UNIT_ULONG)1 << (UNIT_BITS2 - 1))

#define CALC_MASK        (0xffffffffffffffffL)
#define CALC_MASKl       (0xffffffffL)
#define CALC_MASKh       (0xffffffff00000000L)
#define CALC_MASKh1      (0xffffffff80000000L)

//#define SMALL_FOOTPRINT  1        ///< 在运算时进行单步运算，便于观察运算。

/**
  * @brief         大数类型的保存结构。
  */
typedef std::deque<unit_t> number_t;

/**
  * @brief         大数结构。
  */
typedef struct __bignum_t {
  number_t number;          ///< 保存数值的队列
  int neg;                  ///< 当前数是负数
} bignum_t;

/**
  * @brief         除法结果结构
  */
typedef std::pair<bignum_t, bignum_t> division_result_t;

#define is_negative(x)       (x.neg)
#define is_positive(x)       (!x.neg)
#define is_null(x)           (x.number.empty())

#define bn_size(x)           (x.number.size())
#define bn_resize(x, n)      (x.number.resize(n))

/**
  * @brief         清除掉大数结构无效的单元位
  */
#define clear_head_zero(x)   { \
  size_t __l = x.number.size(); \
  while (__l && x.number[__l-1] == 0) { \
    --__l; \
  } \
  x.number.resize(__l); \
}

unit_t __add_units(number_t& z, const number_t &x, const number_t &y, size_t n);
unit_t __sub_units(number_t& z, const number_t &x, const number_t &y, size_t n);

bignum_t create(std::string str, bool hex=false);
bignum_t create(unsigned char* buf, size_t len, bool little=true, bool is_sign=false);
bignum_t create(unit_t number, int neg=0);
bignum_t create(const std::deque<unit_t>& number, int neg=0);

void zero(bignum_t& x);
bool is_zero(const bignum_t& x);

void one(bignum_t& x, int neg=0);
bool is_one(const bignum_t& x);

int cmp(const bignum_t& x, const bignum_t& y);
int ucmp(const bignum_t& x, const bignum_t& y);

bignum_t add(const bignum_t& x, const bignum_t& y);
bignum_t sub(const bignum_t& x, const bignum_t& y);
bignum_t uadd(const bignum_t& x, const bignum_t& y);
bignum_t usub(const bignum_t& x, const bignum_t& y);

std::string print_string(const bignum_t& x, bool hex=false, bool low_case=false);

} // namespace mympz

#endif // MYMPZ_H_