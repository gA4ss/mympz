#include <mympz/bignum.h>

namespace mympz {

void zero(bignum_t& a) {
  a.clear();
  a.push_back(0);
}

} // namespace mympz