#include <mympz/mympz.h>

namespace mympz {

/**
  * @brief         将任意大数设置为0。
  * @param[in,out] x      要判断的大数。
  * @note   本大数结构中的0，需要的是整个数值内存存在唯一的0，数值队列为空，是null值并非0。

  * @par Sample
  * @code
  * bignum_t x;
  * zero(x);
  * std::cout << print_string(x) << std::endl;
  * @endcode
  */
void zero(bignum_t& x) {
  x.neg = false;
  x.number.clear();
  x.number.push_back(0);
}

/**
  * @brief         判断当前大数是否是0。
  * @param[in]     x     要判断的大数。
  * @return        判断是0返回true，反之false。

  * @par Sample
  * @code
  * bignum_t x;
  * zero(x);
  * if (is_zero(x)) {
  *   std::cout << "x == 1" << std::endl;
  * }
  * @endcode
  */
bool is_zero(const bignum_t& x) {
  if (x.number.empty()) return false;
  if (x.number.size() >= 1) {
    ///< 确保数值队列都是0
    for (size_t i = 0; i < x.number.size(); i++) {
      if (x.number[i] != 0) return false;
    }
  }
  return true;
}

} // namespace mympz