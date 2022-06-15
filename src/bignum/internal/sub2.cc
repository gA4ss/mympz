#include <mympz/bignum.h>

namespace mympz {
namespace internal {

bignum_t sub2(const bignum_t& a, const bignum_t& b, bool* t) {
  my_assert(t, "%s", "t is nullptr.");
  bignum_t x, y, z;
  if (t) *t = false;
  if (a.empty() && !b.empty()) {
    x.push_back(0);
    y = b;
  } else if (!a.empty() && b.empty()) {
    x = a;
    y.push_back(0);
  } else if (a.empty() && b.empty()) {
    bignum_t c = {0};
    return c;
  } else {
    x = a;
    y = b;
  }

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
    //
    // 这里要考虑x[i] - r 小于0的情况。
    // 这种情况通常发生在x[i]为0时。
    //
    if (x[i] - r >= y[i]) {
      c = x[i] - y[i] - r;
      r = 0;
    } else {
      c = x[i] - r + 10 - y[i];
      r = 1; 
    }
    // if (c == 10) {
      // std::cout << i << std::endl;
    // }
    z.push_back(c);
  }
  if (t) *t = static_cast<bool>(r);    // 设置是否借位
  return z;
}

} // namespace internal
} // namespace mympz