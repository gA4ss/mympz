#include <mympz/mympz.h>

namespace mympz {

/**
  * @brief         创建大数类型
  * @param[in]     sign         符号，默认是正数。
  * @return        大数结构

  * @par Sample
  * @code
  * 
  * @endcode
  */
bignum_t create(int sign) {
  bignum_t x;
  x.sign = sign;
  return x;
}

bignum_t create(std::string str) {
  bignum_t x;
  return x;
}

bignum_t create(unit_t number, int sign) {
  bignum_t x;
  x.sign = sign;
  x.number.push_back(number);
  return x;
}

bignum_t create(const std::deque<unit_t>& number, int sign) {
  bignum_t x;
  x.sign = sign;
  x.number = number;
  return x;
}

} // namespace mympz