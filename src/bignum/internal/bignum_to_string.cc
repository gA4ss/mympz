#include <mympz/bignum.h>

namespace mympz {
namespace internal {

static const unit_t kZeroCode = 48;
std::string bignum_to_string(const bignum_t& a) {
  std::string res = "";
  char c = 0;

  my::integer_t i = static_cast<my::integer_t>(a.size()) - 1;
  for (; i >= 0; i--) {
    c = static_cast<char>(a[i] + kZeroCode);
    res.push_back(c);
  }
  return res;
}

} // namespace internal
} // namespace mympz