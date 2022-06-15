#include <mympz/bignum.h>

namespace mympz {
namespace internal {

bool is_zero(const bignum_t& a) {
  if (a.empty())
    return false;
  
  for (size_t i = 0; i < a.size(); i++) {
    if (a[i] != 0)
      return false;
  }
  return true;
}

} // namespace internal
} // namespace mympz