#include <mympz/bignum.h>

namespace mympz {

bignum_t sub(bignum_t& a, bignum_t& b, bool t) {
  bignum_t x, y, z;

  if (a.empty() || b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }

  if (a.empty() && !b.empty()) {
    x.push_back(0);
    y = b;
  } else if (!a.empty() && b.empty()) {
    x = a;
    y.push_back(0);
  } else if (a.empty() && b.empty()) {
    x.push_back(0);
    y.push_back(0);
  } else {
    x = a;
    y = b;
  }

  //
  // x比 y小则交换两者位置
  //
  if (cmp(x, y) < 0) {
    z = x;
    x = y;
    y = z;
    z.clear();
  }

  if (x.size() > y.size()) {
    y.resize(x.size());
  } else if (x.size() < y.size()) {
    x.resize(y.size());
  }

  unit_t r = static_cast<unit_t>(t), c = 0;
  size_t l = x.size();
  for (size_t i = 0; i < l; i++) {
    if (x[i] - r >= y[i]) {
      c = x[i] - y[i] - r;
      r = 0;
    } else {
      c = x[i] + 10 - y[i] - r;
      r = 1;
    }
    z.push_back(c);
  }
  if (r != 0) {
    r = 1;
  }
  my_assert(r == 0, "%s", "borrow must be 0.");
  return z;
}

} // namespace mympz