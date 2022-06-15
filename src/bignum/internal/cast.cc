#include <mympz/bignum.h>

namespace mympz {
namespace internal {

/**
 * @brief 这里需要保证
 * 
 * @return bignum_t 
 */
static bignum_t __direct_cast(const bignum_t& a) {
  bignum_t b;
}

static bignum_t __internal_max_integer = internal::string_to_bignum(bignum_string);
bignum_t cast(const bignum_t& a) {
  bignum_t b;

  if (a.empty()) {
    mympz_exception(
      "%s", "operand a is nan."
    );
  }

  size_t l = a.size();
  for (size_t i = 0; i < l; i++) {

  }
  return b;
}

} // namespace internal
} // namespace mympz