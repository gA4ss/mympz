#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         按照大数结构输出字符串。
  * @param[in]     x          大数结构。
  * @param[in]     hex        按照16进制输出。
  * @param[in]     low_case   如果是16进制，则按小写输出。
  * @return        返回一个字符串。
  */
std::string print_string(const bignum_t& x, bool hex, bool low_case) {
  std::string str;

  if (x.number.empty()) return "";

  if (is_zero(x)) {
    str.push_back('0');
    return str;
  }

  if (hex)
    str += __print_string_hex(x.number, low_case);
  else
    str += __print_string_dec(x.number);

  if (x.neg) {
    str = std::string("-") + str;
  }
  return str;
}

/**
  * @brief         按照大数结构输出到缓存。
  * @param[in]     x          大数结构。
  * @param[in]     to         输出缓存。
  * @param[in]     tolen      缓存长度。
  * @return        返回转化后的长度。
  */
size_t print_buffer(const bignum_t& x, unsigned char *to, size_t tolen,
                    bool little, bool is_sign) {
  return __bn2bin(x, to, tolen, little, is_sign);
}

} // namespace mympz