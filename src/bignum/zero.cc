#include <mynum/bignum.h>

namespace mynum {

void zero(bignum_t& a) {
  a.clear();
  a.push_back(0);
}

} // namespace mynum