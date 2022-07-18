#include <mympz/mympz.h>

namespace mympz {

static const char Hex[] = "0123456789ABCDEF";

/**
  * @brief         按照大数结构输出字符串。
  * @param[in]     x          大数结构。
  * @return        返回一个字符串。
  */
std::string print_string(const bignum_t& x, bool hex) {
  std::string str;

  if (is_zero(x)) {
    str.push_back('0');
    return str;
  }

  if (x.neg) {
    str.push_back('-');
  }

  int h = hex ? 4 : 8;        ///< 控制位移，16进制输出每次4位，按照10进制每次8位。
  int v = 0, z = 0;
  for (int i = static_cast<int>(bn_size(x)-1); i >= 0; i--) {
    //
    // 遍历每个单位的每个字节
    //
    for (int j = UNIT_BITS2 - h; j >= 0; j -= h) {
      ///< 提出首位的0
      v = static_cast<int>((x.number[i] >> j) & 0x0f);
      if (z || v != 0) {
        str.push_back(hex ? Hex[v] : static_cast<char>(v + 48));
        z = 1;
      }/* end if */
    }
  }/* end for */
  return str;
}

} // namespace mympz