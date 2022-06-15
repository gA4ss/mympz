#include <mynum/bignum.h>

namespace mynum {

static void __invalid_base(int base) {
  if (base != 10 && base != 2 && base != 8 && base != 16) {
    invalid_arguments_exception("base = %d", base);
  }
}

static bool __check_number_format(const char* number) {
  if (number == nullptr) return false;
  char* ptr = const_cast<char*>(number);
  do {
    if (!std::isdigit(*ptr)) return false;
  } while (*(++ptr));
  return true;
}

static const unit_t kZeroCode = 48;
bignum_t string_to_bignum(const char* number, int base, bool shrink_reverse/*=false*/) {
  bignum_t res;

  if (strlen(number) == 0 || number == nullptr) {
    zero(res);
    return res;
  }

  __invalid_base(base);   // FIXME: 目前只支持10进制构造。

  // 检查number字符串的格式
  if (!__check_number_format(number)) {
    invalid_arguments_exception("number = %s", number);
  }

  char* ptr = const_cast<char*>(number);
  do {
    unit_t j = static_cast<unit_t>(*ptr - kZeroCode);
    res.push_front(j);
  } while (*(++ptr));

  shrink_zero(res, shrink_reverse);
  if (res.empty()) res.push_back(0);      // 如果削减为空，则添加一个0保留位数。
  return res;
}

} // namespace mynum