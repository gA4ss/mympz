#include <mympz/mympz.h>

namespace mympz {

/**
  * @brief         将缓冲区数据转换为大数结构。
  * @param[in]     s          缓冲区
  * @param[in]     len        长度
  * @param[in]     little     小端字序
  * @param[in]     unsign     无符号
  * @return        构造好的大数结构。
  */
static bignum_t __bin2bn(const unsigned char *s, size_t len, 
                         bool little=true, bool unsign=true) {
  bignum_t x;

  //
  // 按照字序进行设定读取方式
  //
  const unsigned char *s2;
  int inc, inc2;
  if (little) {
    s2 = s + len - 1;
    inc2 = -1;
    inc = 1;
  } else {
    s2 = s;
    inc2 = 1;
    inc = -1;
    s += len - 1;
  }

  //
  // 输入是否是有符号数
  //
  int neg = 0, x0r = 0, carry = 0;
  if (unsign) {
    neg = !!(*s2 & 0x80);
    x0r = neg ? 0xff : 0x00;
    carry = neg;
  }

  //
  // 跳过符号位的扩展（|x0r|的值）
  //
  for ( ;len > 0 && *s2 == x0r; s2 += inc2, len--)
    continue;

  /*
   * 如果有一组0xff，我们回溯一个字节，除非下一个字节有符号位，
   * 因为最后一个0xff是实际数字的一部分，而不是仅仅是符号扩展。
   */
  if (x0r == 0xff) {
    if (len == 0 || !(*s2 & 0x80))
      len++;
  }

  /* 如果是0则结束 */
  if (len == 0) {
    zero(x);
    return x;
  }


  //
  // 给结果分配空间
  //
  size_t n = ((len - 1) / UNIT_BYTES) + 1;      ///< 返回结构的长度
  bn_resize(x, n);
  x.neg = neg;

  //
  // 填充结果
  //
  for (size_t i = 0; n-- > 0; i++) {
    unit_t l = 0;         ///< 一个单元
    size_t m = 0;         ///< 控制当字节所在整个单元的位位置

    //
    // 填充一个单元，64位下是8个字节。
    // m += 8 为加8位，一个字节。
    //
    for (; len > 0 && m < UNIT_BYTES * 8; len--, s += inc, m += 8) {
      unit_t byte_xored = *s ^ x0r;
      unit_t byte = (byte_xored + carry) & 0xff;

      carry = byte_xored > byte;
      l |= (byte << m);
    }
    x.number[i] = l;
  }

  //
  // 清除首位的0
  //
  return x;
}

/**
  * @brief         创建大数类型
  * @param[in]     str         大数的字符串，可存在负号'-'。
  * @return        大数结构
  */
bignum_t create(std::string str) {
  unsigned char* buf = new unsigned char [str.length()+1];
  memset(buf, 0, str.length()+1);
  const char* ptr = str.c_str();
  int neg = 0;

  size_t len = str.length();
  if (str[0] == '-') {
    ptr++;
    len--;
    neg = 1;
  }

  size_t j = len-1;
  for (size_t i = 0; i < len; i++)
    buf[j--] = ptr[i] - 48;

  bignum_t x = __bin2bn(buf, len, true, true);
  if (buf) delete [] buf;

  x.neg = neg;
  return x;
}

/**
  * @brief         创建大数类型
  * @param[in]     buf         大数的缓冲区
  * @param[in]     len         缓冲区长度
  * @param[in]     little      小端字序
  * @param[in]     unsign      是否是无符号数
  * @return        大数结构
  */
bignum_t create(unsigned char* buf, size_t len, bool little, bool unsign) {
  return __bin2bn(buf, len, little, unsign);
}

/**
  * @brief         创建大数类型
  * @param[in]     number       单位数。
  * @param[in]     neg          表明是否是负数。
  * @return        大数结构
  */
bignum_t create(unit_t number, int neg) {
  bignum_t x;
  x.neg = neg;
  x.number.push_back(number);
  return x;
}

/**
  * @brief         创建大数类型
  * @param[in]     number       单位数队列。
  * @param[in]     neg          表明是否是负数。
  * @return        大数结构
  */
bignum_t create(const std::deque<unit_t>& number, int neg) {
  bignum_t x;
  x.neg = neg;
  x.number = number;
  return x;
}

} // namespace mympz