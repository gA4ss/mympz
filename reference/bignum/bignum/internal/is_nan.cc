#include <mympz/bignum.h>

namespace mympz {
namespace internal {

bool is_nan(const bignum_t& a) {
  return a.empty();
}

} // namespace internal
} // namespace mympz