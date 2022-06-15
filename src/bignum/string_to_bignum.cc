#include <mympz/bignum.h>

namespace mympz {

/*
 * 首先转换为内部的一个存储单元保存一个10进制数。
 * 然后从这个内部的存储格式转为正式运算的保存格式。
 */
static bignum_t __internal_max_integer = internal::string_to_bignum(bignum_string);
bignum_t string_to_bignum(const char* number, int base, bool shrink_reverse/*=false*/) {
  bignum_t res, middle_res = internal::string_to_bignum(number, base, shrink_reverse);

  //
  // 输入的数小于等于最大值
  //
  if (internal::cmp(middle_res, __internal_max_integer) < 1) {
    size_t l = middle_res.size();
  } else {

  }

  return res;
}

} // namespace mympz