#include <sstream>
#include <mympz/bignum.h>

namespace mympz {

static 

static std::string __unit_to_string(unit_t a) {
  std::string out;
  std::stringstream in;
  in << a; in >> out;
  return out;
}

static const unit_t kZeroCode = 48;
static const bignum_t __internal_max_integer = internal::string_to_bignum(kBigNumString);
std::string bignum_to_string(const bignum_t& a) {
  std::string res = "";

  bignum_t x;
  size_t l = a.size();
  for (size_t i = 0; i < l; i++) {
    x = internal::string_to_bignum(__unit_to_string());
  }

  return res;
}

} // namespace mympz