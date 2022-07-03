#include <mympz/bignum.h>

namespace mympz {

/*
 * 内部大数转大数单元
 */
static unit_t __internal_bignum_to_unit(const bignum_t& a, size_t s, size_t e) {
  my_assert(!a.empty(), "size of a = %l", a.size());

  unit_t b = 0;
  for (size_t i = s; i < e; i++) {
    b += (std::pow(10, i) * a[i]);
  }
  return b;
}

/*
 * 首先转换为内部的一个存储单元保存一个10进制数。
 * 然后从这个内部的存储格式转为正式运算的保存格式。
 */
static const bignum_t __internal_max_integer = internal::string_to_bignum(kBigNumString);
bignum_t string_to_bignum(const char* number, int base, bool shrink_reverse/*=false*/) {
  bignum_t res, target = internal::string_to_bignum(number, base, shrink_reverse);

  unit_t x = 0;
  while (true) {
    // 结束条件
    if (internal::cmp(target, __internal_max_integer) == -1) {
      x = __internal_bignum_to_unit(target, 0, target.size());
      res.push_front(x);
      break;
    }

    division_result_t y = internal::div(target, __internal_max_integer);
    if (!internal::is_zero(y.second)) {
      x = __internal_bignum_to_unit(y.second, 0, y.second.size());
    } else {
      x = 0;
    }
    target = y.first;
    res.push_front(x);
  }

  return res;
}

} // namespace mympz