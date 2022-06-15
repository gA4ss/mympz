#include <mynum/bignum.h>

namespace mynum {

/*
 * a1表示a的首项系数，以此类推。B为某个进制。
 * 1.当a,b的位数一致，a1 >= b1，a > b。
 *  1-1 : a1 = b1 ---> q = 1.
 *  1-2 : a1 > b1 ---> q1 = floor(a_1/b_1), q2 = max(floor( (a1*B+a2-B+1) / (b1*B+b2)) ), 1)
 *        q1 <= q <= q2
 * 2.当a的位数比b大一位，a1 <= b1，a > b, a < b*B(仅差一位)。
 *  2-1 : q1 = min(floor( (a1*B+a2) / b1 ), B-1)
 *        q <= q1
 * 证明见《计算机代数》【陈玉福，张智勇】 25-26页。
 */
static std::pair<unit_t, unit_t> __get_quotient_range(const bignum_t& a, const bignum_t& b) {
  const unit_t B = 10;
  unit_t a1 = *(a.end()-1), b1 = *(b.end()-1);
  unit_t a2 = 0, b2 = 0;

  if (a.size() >= 2) a2 = *(a.end()-2);
  if (b.size() >= 2) b2 = *(b.end()-2);

  if (a.size() == b.size()) {
    //
    // 第一种情况
    //
    if (a1 == b1)
      return std::make_pair(1, 1);
    else { // 这里一定是 a1 > b1
      my_assert(a1 > b1, "a1(%d) < b1(%d)", a1, b1);
      unit_t q1 = a1 / b1, q2 = std::max((a1*B+a2-B+1)/(b1*B+b2), 1);
      return q1 < q2 ? std::make_pair(q1, q2) : std::make_pair(q2, q1);
    }
  } else {
    //
    // 第二种情况
    //
    unit_t q = std::min((a1*B+a2)/b1, B-1);
    return std::make_pair(1, q);
  }
  my_assert(false, "%s", "never go here!");
  return std::make_pair(0, 0);
}

/* 不考虑符号 */
division_result_t div(const bignum_t& a, const bignum_t& b) {
  if (a.empty() || b.empty()) {
    operand_value_is_invalid_exception(
      "%s", "operand a or b is nan."
    );
  }
  if (is_zero(b)) divisor_is_zero_exception("%s", "b is zero.");
  //const bignum_t ten = {0, 1};    // 表示10

  bignum_t x = a, y = b;
  if (cmp(x, y) == 0)
    return division_result_t({1}, {0});
  else if (cmp(x, y) == -1)
    return division_result_t({0}, {x});

  bignum_t quotient, product, dividend = x, divisor = y, remainder = {0};
  my::uinteger_t dividend_remainder_digits = dividend.size();

  //
  // 这里对被除数与除数做一些初始化工作，
  // 从末尾取a与b相同的位数 : a_n, b_n
  // 如果 a_n < b_n, 则取a_{n+1}。
  //
  size_t n = divisor.size();
  dividend_remainder_digits -= n;
  dividend = bignum_t(x.begin()+dividend_remainder_digits, x.end());
  if (cmp(dividend, divisor) == -1) {
    // 如果小于这里dividend必比divisor多一位。
    --dividend_remainder_digits;
    dividend = bignum_t(x.begin()+dividend_remainder_digits, x.end());
  }

  // 第一次运行必须保证被除数大于除数。
  while (true) {
    // 尝试获取商的范围。
    std::pair<unit_t, unit_t> quo_range = __get_quotient_range(dividend, divisor);
    unit_t q = 0;
    for (q = quo_range.first; q <= quo_range.second; q++) {
      product = mul(divisor, {q});
      shrink_zero(product, true); // 乘法可能产生多余的0。
      //
      // product 必然小于等于 dividend
      //
      remainder = sub(dividend, product, false);
      int c = cmp(divisor, remainder);
      if ((c == -1) || (c == 0)) {
        continue;
      } else {
        quotient.push_front(q);
        break;
      }
    }

    //
    // 算法结束条件：被除数没有多余的位。
    //
    if (dividend_remainder_digits == 0)
      break;

    //
    // 更新被除数
    //
    dividend = remainder;

    //
    // 这里首先借一位，如果大于则直接做运算。
    // 如果还是小于则商使用0补位。
    // 保证被除数大于除数。
    //
    if ((cmp(dividend, divisor) == -1) && (dividend_remainder_digits != 0)) {
      if (cmp(dividend, {0}) == 0) dividend.clear();  // 余数为零，多见10000/2这种情况。
      dividend.push_front(x[dividend_remainder_digits-1]);
      --dividend_remainder_digits;

      //
      // 这里如果借了一位还是小于那么开始补位
      //
      while ((cmp(dividend, divisor) == -1) && (dividend_remainder_digits != 0)) {
        dividend.push_front(x[dividend_remainder_digits-1]);
        quotient.push_front(0);
        --dividend_remainder_digits;
      }
    }

    //
    // 特殊情况1,例如：8589934590 / 791621423
    // 如果被除数还是小于除数，这里蕴含的一个条件就是 dividend_remainder_digits为0。
    // quotient压入0比dividend少一个。
    //
    if (cmp(dividend, divisor) == -1) {
      quotient.push_front(0);
      break;
    }

    //
    // 特殊情况2
    // 如果被除数剩余全部是0，这里蕴含的一个条件就是 dividend_remainder_digits为0。
    // quotient压入0比dividend少一个。
    //
    if (cmp(dividend, {0}) == 0) {
      // quotient.insert(quotient.begin(), dividend.begin(), dividend.end());
      quotient.push_front(0);
      break;
    }

    //
    // 例如：10001 / 2 这种情况，可能造成 dividend = 0010
    // 这里将前面两个0清除。
    //
    if (dividend.back() == 0) shrink_zero(dividend, true);
  }/* end while */

  shrink_zero(remainder, true);
  return division_result_t(quotient, remainder);
}

} // namespace mynum