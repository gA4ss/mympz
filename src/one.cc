#include <mympz/mympz.h>

namespace mympz {

/**
  * @brief         将任意大数设置为1。
  * @param[in,out] x      要判断的大数。
  * @param[in]     neg    是否是负数。

  * @par Sample
  * @code
  * bignum_t x;
  * one(x, -1);
  * std::cout << print_string(x) << std::endl;
  * @endcode
  */
void one(bignum_t& x, int neg) {
  x.neg = neg;
  x.number.clear();
  x.number.push_back(1);
}

/**
  * @brief         判断当前大数是否是1。
  * @param[in]     x     要判断的大数。
  * @note          这里忽略x的符号。
  * @return        如果判断正确则返回true，反之false。

  * @par Sample
  * @code
  * bignum_t x;
  * one(x);
  * if (is_one(x)) {
  *   std::cout << "x == 1" << std::endl;
  * }
  * @endcode
  */
bool is_one(const bignum_t& x) {
  if (x.number.empty()) return false;
  if (x.number.size() > 1) {
    // 确认前面的数都是0
    for (size_t i = 1; i < x.number.size(); i++) {
      if (x.number[i] != 0) return false;
    }
  }
  return x.number[0] == 1;
}

} // namespace mympz