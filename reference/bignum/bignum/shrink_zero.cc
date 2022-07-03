#include <mympz/bignum.h>

namespace mympz {

/* 整数部分消减 true, 小数部分消减 false。
 * 保留最后一个0。
 * 在除法中div中不能使用缩减，因为
 * 最末尾的0是为了占位。
 */
my::uinteger_t shrink_zero(bignum_t& a, bool reverse) {
  if (a.empty()) return 0;
  if ((a.size() == 1) && (a[0] == 0)) return 0;

  my::uinteger_t ret = 0;
  if (reverse) {
    int l = static_cast<int>(a.size());
    for (int i = l-1; i > 0; i--) {
      if (a[i] == 0) {
        a.pop_back(); ++ret;
      } else {
        break;
      }
    }
  } else {
    bignum_iter_t it = a.begin();
    while (it < a.end()) {
      if (*it == 0) {
        it = a.erase(it);
        ++ret;
      } else {
        break;
      }
    }
  }
  return ret;
}

} // namespace mympz