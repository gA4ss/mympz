#include <mympz/bignum.h>

namespace mympz {

int cmp(const bignum_t& a, const bignum_t& b, bool push_front/*=false*/) {
  int res = 0, l = 0;
  bignum_t _a = a, _b = b;

  if (_a.empty() || _b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }

  if (_a.size() > _b.size()) {
    l = static_cast<int>(_a.size() - _b.size());
    for (int i = 0; i < l; i++) {
      if (push_front) _b.push_front(0);
      else _b.push_back(0);
    }
  } else if (_a.size() < _b.size()) {
    l = static_cast<int>(_b.size() - _a.size());
    for (int i = 0; i < l; i++) {
      if (push_front) _a.push_front(0);
      else _a.push_back(0);
    }
  }

  my_assert(
    _a.size() == _b.size(), 
    "(_a.size() = %d, _b.size() = %d) is not equal.", 
    _a.size(), _b.size()
  );

  l = static_cast<int>(_a.size());
  for (int i = l-1; i >= 0; i--) {
    if (_a[i] > _b[i]) {
      res = 1;
      break;
    } else if (_a[i] < _b[i]) {
      res = -1;
      break;
    }
  }
  return res;
}

} // namespace mympz