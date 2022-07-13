#ifndef MYMPZ_BIGNUM_H_
#define MYMPZ_BIGNUM_H_

#include <cmath>
#include <algorithm>

#include <my/my_common.h>
#include <mympz/exception.h>

namespace mympz {

//
// 大数位进制
//
#define BIGNUM_SIZE    64

#if BIGNUM_SIZE == 64

  typedef uint64_t  unit_t;

#elif BIGNUM_SIZE == 32

  typedef uint32_t  unit_t;

#elif BIGNUM_SIZE == 16

  typedef uint16_t  unit_t;

#else

  typedef uint8_t   unit_t;

#endif

typedef std::deque<unit_t> number_t;
typedef struct __bignum_t {
  number_t number;
  int sign;
} bignum_t;

typedef std::pair<bignum_t, bignum_t> division_result_t;

#define is_negative(x)       (x.sign < 0)
#define is_positive(x)       (x.sign > 0)

#define size(x)              (x.number.size())

bignum_t create(int sign=1);
bignum_t create(std::string str);
bignum_t create(unit_t number, int sign=1);
bignum_t create(const std::deque<unit_t>& number, int sign=1);


void zero(bignum_t& x);
bool is_zero(const bignum_t& x);

void one(bignum_t& x, int sign=1);
bool is_one(const bignum_t& x);

bignum_t add(const bignum_t& x, const bignum_t& y);
bignum_t sub(const bignum_t& x, const bignum_t& y);
bignum_t uadd(const bignum_t& x, const bignum_t& y);
bignum_t usub(const bignum_t& x, const bignum_t& y);

std::string print_string(const bignum_t& x);

} // namespace mympz

#endif // MYMPZ_BIGNUM_H_