#include <mynum/bignum.h>

namespace mynum {

bignum_t add2(const bignum_t& a, const bignum_t& b, bool* o) {
  my_assert(o, "%s", "o is nullptr.");
  *o = false;
  if (a.empty() && !b.empty()) {
    return b;
  } else if (!a.empty() && b.empty()) {
    return a;
  } else if (a.empty() && b.empty()) {
    bignum_t c = {0};
    return c;
  }
  bignum_t x = a, y = b, z;

  if (x.size() > y.size()) {
    size_t l = x.size() - y.size();
    for (size_t i = 0; i < l; i++) {
      y.push_front(0);
    }
  } else if (x.size() < y.size()) {
    size_t l = y.size() - x.size();
    for (size_t i = 0; i < l; i++) {
      x.push_front(0);
    }
  }

  unit_t r = 0, c = 0;
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
  if (o) *o = static_cast<bool>(r);    // 设置是否进位
  return z;
}

} // namespace mynum