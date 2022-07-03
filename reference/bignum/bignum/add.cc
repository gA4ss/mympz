#include <mympz/bignum.h>

namespace mympz {

bignum_t add(const bignum_t& a, const bignum_t& b, bool o) {
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

  if (x.size() > y.size()) {
    y.resize(x.size());
  } else if (x.size() < y.size()) {
    x.resize(y.size());
  }

  unit_t c = 0, r = static_cast<unit_t>(o);
  size_t l = x.size();
  for (size_t i = 0; i < l; i++) {
    c = x[i] + y[i] + r;
    if (c >= 10) {
      c = c - 10;
      r = 1;
    } else {
      r = 0;
    }
    z.push_back(c);
  }
  // 如果存在进位则保存
  if (r) z.push_back(r);
  return z;
}

} // namespace mympz