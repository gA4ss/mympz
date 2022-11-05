#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{

  bignum_t gcd(const bignum_t &x, const bignum_t &y)
  {
    bignum_t r;
    init_bignum(r);
    r.number = __gcd(bn_ptr(bn_const_cast(x)), bn_size(x),
                     bn_ptr(bn_const_cast(y)), bn_size(y));
    clear_head_zero(r);
    return r;
  }

} // namespace mympz