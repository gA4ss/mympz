#include <mympz/bignum.h>

namespace mympz {
namespace internal {

void nan(bignum_t& a) {
  a.clear();
}

} // namespace internal
} // namespace mympz