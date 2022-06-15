#ifndef MYMPZ_BIGNUM_H_
#define MYMPZ_BIGNUM_H_

#include <my/my_common.h>

namespace mympz {

//
// 大数位进制
//
#define BIGNUM_SIZE    64

#if BIGNUM_SIZE == 64

  typedef uint64_t bnres_t;
  typedef uint32_t unit_t;

  static const char* bignum_string = "4294967295";

#elif BIGNUM_SIZE == 32

  typedef uint32_t bnres_t;
  typedef uint16_t unit_t;

  static const char* bignum_string = "65535"

#else

  typedef uint16_t bnres_t;
  typedef uint8_t unit_t;

  static const char* bignum_string = "255"

#endif

typedef std::deque<unit_t> bignum_t;
typedef bignum_t::iterator bignum_iter_t;
typedef bignum_t::reverse_iterator bignum_riter_t;
typedef std::pair<bignum_t, bignum_t> division_result_t;

namespace internal {

bignum_t cast(const bignum_t& a);
my::uinteger_t shrink_zero(bignum_t& a, bool reverse=false);
bignum_t string_to_bignum(const char* number=nullptr, int base=10, bool shrink_reverse=true);
std::string bignum_to_string(const bignum_t& a);
void nan(bignum_t& a);
void zero(bignum_t& a);
bool is_nan(const bignum_t& a);
bool is_zero(const bignum_t& a);
int cmp(const bignum_t& a, const bignum_t& b, bool push_front=false);
bignum_t add(const bignum_t& a, const bignum_t& b, bool o=false);
bignum_t add2(const bignum_t& a, const bignum_t& b, bool* o=nullptr);
bignum_t sub(bignum_t& a, bignum_t& b, bool t=false);
bignum_t sub2(const bignum_t& a, const bignum_t& b, bool* t=nullptr);
bignum_t mul(const bignum_t& a, const bignum_t& b);
division_result_t div(const bignum_t& a, const bignum_t& b);
division_result_t div2(const bignum_t& a, const bignum_t& b, my::uinteger_t precision=16);
bignum_t mod(const bignum_t& a, const bignum_t& b);

} // namespace internal

my::uinteger_t shrink_zero(bignum_t& a, bool reverse=false);
bignum_t string_to_bignum(const char* number=nullptr, int base=10, bool shrink_reverse=true);
std::string bignum_to_string(const bignum_t& a);
void nan(bignum_t& a);
void zero(bignum_t& a);
bool is_nan(const bignum_t& a);
bool is_zero(const bignum_t& a);
int cmp(const bignum_t& a, const bignum_t& b, bool push_front=false);
bignum_t add(const bignum_t& a, const bignum_t& b, bool o=false);
bignum_t add2(const bignum_t& a, const bignum_t& b, bool* o=nullptr);
bignum_t sub(bignum_t& a, bignum_t& b, bool t=false);
bignum_t sub2(const bignum_t& a, const bignum_t& b, bool* t=nullptr);
bignum_t mul(const bignum_t& a, const bignum_t& b);
division_result_t div(const bignum_t& a, const bignum_t& b);
division_result_t div2(const bignum_t& a, const bignum_t& b, my::uinteger_t precision=16);
bignum_t mod(const bignum_t& a, const bignum_t& b);

} // namespace mympz

#endif // MYMPZ_BIGNUM_H_