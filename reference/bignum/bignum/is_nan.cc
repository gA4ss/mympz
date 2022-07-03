#include <mympz/bignum.h>

namespace mympz {

bool is_nan(const bignum_t& a) {
  return a.empty();
}

} // namespace mympz