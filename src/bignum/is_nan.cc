#include <mynum/bignum.h>

namespace mynum {

bool is_nan(const bignum_t& a) {
  return a.empty();
}

} // namespace mynum