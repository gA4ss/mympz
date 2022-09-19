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
#include <iostream>

#include <mympz/compile.h>
#include <mympz/exception.h>

namespace mympz {

/**
  * @brief         大数结构的单位。
  */
typedef int64_t          sunit_t;
typedef uint64_t         unit_t;
#define UNIT_MAX         (0xffffffffffffffffL)

/**
  * @brief         大数类型的保存结构。
  */
typedef std::deque<unit_t>      number_t;
typedef number_t::iterator      number_ptr_t;

/**
  * @brief         大数结构。
  */
typedef struct {
  number_t number;          ///< 保存数值的队列
  int neg;                  ///< 当前数是负数
} bignum_t;

/**
  * @brief         除法结果结构
  */
typedef std::pair<bignum_t, bignum_t> division_result_t;

#define init_bignum(x)                {(x).neg = 0;(x).number.clear();}

#define is_negative(x)                ((x).neg)
#define is_positive(x)                (!(x).neg)
#define is_null(x)                    ((x).number.empty())
#define is_error(x)                   is_null((x))

#define set_negative(x)               ((x).neg = 1)
#define set_positive(x)               ((x).neg = 0)
#define set_null(x)                   {(x).neg = 0;(x).number.clear();}
#define set_error(x)                  set_null((x))

#define bn_size(x)                    ((x).number.size())
#define bn_resize(x, n)               ((x).number.resize(n))
#define bn_ptr(x)                     ((x).number.begin())
#define bn_ptr2(x, n)                 ((x).number.begin()+(n))
#define bn_const_cast(x)              (const_cast<bignum_t&>((x)))

#define num_size(x)                   ((x).size())
#define num_resize(x, n)              ((x).resize(n))
#define num_ptr(x)                    ((x).begin())
#define num_ptr2(x, n)                ((x).begin()+(n))
#define num_ptr_const_cast(x)         (const_cast<number_ptr_t&>((x)))

/**
  * @brief         清除掉大数结构无效的单元位
  */
#define clear_head_zero(x)   { \
  size_t __l = (x).number.size(); \
  while (__l && (x).number[__l-1] == 0) { \
    --__l; \
  } \
  (x).number.resize(__l); \
}

bignum_t create(std::string str, bool hex=false);
bignum_t create(unsigned char* buf, size_t len, bool little=true, bool is_sign=false);
bignum_t create(unit_t number, int neg=0);
bignum_t create(const number_t& number, int neg=0);

bool is_zero(const bignum_t& x);
bool is_one(const bignum_t& x);
bool is_odd(const bignum_t& x);

void zero(bignum_t& x);
void one(bignum_t& x, int neg=0);

unit_t get_word(const bignum_t& x);
void set_word(bignum_t& x, unit_t w);

void set_bit(bignum_t& x, size_t n);
void clear_bit(bignum_t& x, size_t n);
int is_bit_set(const bignum_t& x, size_t n);
void mask_bits(bignum_t& x, size_t n);

int cmp(const bignum_t& x, const bignum_t& y);
int ucmp(const bignum_t& x, const bignum_t& y);

bignum_t add(const bignum_t& x, unit_t w, int wneg=0);
bignum_t add(const bignum_t& x, const bignum_t& y);
bignum_t uadd(const bignum_t& x, const bignum_t& y);

bignum_t sub(const bignum_t& x, unit_t w, int wneg=0);
bignum_t sub(const bignum_t& x, const bignum_t& y);
bignum_t usub(const bignum_t& x, const bignum_t& y);

bignum_t mul(const bignum_t& x, unit_t w, int wneg=0);
bignum_t mul(const bignum_t& x, const bignum_t& y);
bignum_t sqr(const bignum_t& x);

bignum_t div(const bignum_t& x, unit_t w, int wneg=0, unit_t* r=nullptr);
division_result_t div(const bignum_t& x, const bignum_t& y);
bignum_t idiv(const bignum_t& x, const bignum_t& y);

bignum_t lshift1(const bignum_t& x);
bignum_t rshift1(const bignum_t& x);
bignum_t lshift(const bignum_t& x, size_t n);
bignum_t rshift(const bignum_t& x, size_t n);

unit_t mod(const bignum_t& x, unit_t w, int wneg=0);
bignum_t mod(const bignum_t& x, const bignum_t& y);
bignum_t nnmod(const bignum_t& x, const bignum_t& y);
bignum_t mod_add(const bignum_t& x, const bignum_t& y, const bignum_t& m);
bignum_t mod_sub(const bignum_t& x, const bignum_t& y, const bignum_t& m);
bignum_t mod_mul(const bignum_t& x, const bignum_t& y, const bignum_t& m);
bignum_t mod_sqr(const bignum_t& x, const bignum_t& m);
bignum_t mod_lshift1(const bignum_t& x, const bignum_t& m);
bignum_t mod_lshift(const bignum_t& x, size_t n, const bignum_t& m);
bignum_t mod_exp(const bignum_t& x, const bignum_t& p, const bignum_t& m);
bignum_t exp(const bignum_t& x, const bignum_t& p);

std::string print_string(const bignum_t& x, bool hex=false, bool low_case=false);
size_t print_buffer(const bignum_t& x, unsigned char *to, size_t tolen,
                    bool little=true, bool is_sign=false);

} // namespace mympz

#endif // MYMPZ_H_