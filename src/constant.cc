#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{

  bignum_t const_null = create("null");
  bignum_t const_0 = create(0);
  bignum_t const_1 = create(1);
  bignum_t const_10 = create(10);

  void init()
  {
    if (bn_size(const_0) == 0)
    {
      const_0 = create(0);
    }

    if (bn_size(const_1) == 0)
    {
      const_1 = create(1);
    }

    if (bn_size(const_10) == 0)
    {
      const_10 = create(10);
    }
  }

} // namespace mympz