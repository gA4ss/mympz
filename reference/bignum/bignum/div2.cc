#include <mympz/bignum.h>

namespace mympz {

division_result_t div2(const bignum_t& a, const bignum_t& b, my::uinteger_t precision) {
  if (a.empty() || b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }
  if (is_zero(b)) mympz_exception("%s", "b is zero.");
  division_result_t res = div(a, b);
  shrink_zero(res.first, true); shrink_zero(res.second, true);
  if (is_zero(res.second)) return res;    // 余数为0

  //
  // 如果不整除则开始计算
  //
  bignum_t quotient = {0}, integer_park = res.first,
           dividend = res.second,
           divisor = b;
  int cr = 0;
  size_t borrow_zero = 0;
  while (1) {
    //
    // 如果精度达到要求则停止
    // 这里有一种情况就是，如果遇到2/1413333333333333333333
    // 这样的情况，由于被除数与除数相差太远，这样补位的0就占据了
    // 大多数的精度，所以这里要舍去由于0补位的精度，判断的是除法
    // 本身计算得到的精度。
    //
    // quotient.size() 初始值为 1
    //
    // 因为这里减去了借位的数量，所以这里得到的结果，可能比设置
    // 的精度位数大。
    //
    if ((quotient.size() - borrow_zero) > precision)
      break;

    //
    // Debug Info
    //
    // std::cout << "dividend = ";
    // for (int i = dividend.size()-1; i >= 0; i--) {
    //   std::cout << std::to_string(dividend[i]);
    // }
    // std::cout << std::endl;

    // std::cout << "divisor = ";
    // for (int i = divisor.size()-1; i >= 0; i--) {
    //   std::cout <<  std::to_string(divisor[i]);
    // }
    // std::cout << std::endl;

    //
    // 借位
    //
    // 第一次借位商不压入0，从第二次开始才要压入。
    //
    cr = cmp(dividend, divisor);
    if (cr == -1) { \
      dividend.push_front(0);  // 压入一个0
      while (cmp(dividend, divisor) == -1) {
        dividend.push_front(0);
        quotient.push_front(0);
        ++borrow_zero;
      }
    }
    res = div(dividend, divisor);
    shrink_zero(res.first, true); shrink_zero(res.second, true);
    quotient.insert(quotient.begin(), res.first.begin(), res.first.end());
    //
    // 如果余数为0，说明除法完毕。
    //
    if (is_zero(res.second)) {
      break;
    }

    //
    // 更新被除数
    //
    dividend = res.second;
  } // end while

  //
  // 这里需要判断quotient的值，如果发现末尾是以0结尾，则舍去。
  // 这是因为quotient这里表示的是尾数，0.5这样只有一个0的情况
  // 0部分属于整数。只有0.05,这里的05才属于尾数。
  //
  if (quotient.back() == 0) quotient.pop_back();
  return division_result_t(integer_park, quotient);
}

} // namespace mympz