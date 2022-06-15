#include <mympz/bignum.h>

namespace mympz {
namespace internal {

void zero(bignum_t& a) {
  a.clear();
  a.push_back(0);
}

} // namespace internal
} // namespace mympz