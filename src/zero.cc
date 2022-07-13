#include <mympz/mympz.h>

namespace mympz {

void zero(bignum_t& x) {
  x.sign = 0;
  x.number.clear();
  return x;
}

bool is_zero(const bignum_t& x) {
  bool r = (x.sign == 0);
  if (r && !x.number.empty()) x.number.clear();
  return r;
}

} // namespace mympz