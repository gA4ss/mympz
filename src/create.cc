#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

/**
  * @brief         将缓冲区数据转换为大数结构。
  * @param[in]     s          缓冲区
  * @param[in]     len        长度
  * @param[in]     little     小端字序
  * @param[in]     unsign     无符号
  * @return        构造好的大数结构。
  */
static bignum_t __bignum_bin2bn(const unsigned char *s, size_t len, 
                                bool little=true, bool is_sign=false) {
  bignum_t x;
  x.number = __bin2bn(s, len, &(x.neg), little, is_sign);
  return x;
}

/**
  * @brief         创建大数类型
  * @param[in]     str         大数的字符串，可存在负号'-'。
  * @param[in]     hex         字符串是16进制的。
  * @return        大数结构
  */
bignum_t create(std::string str, bool hex) {
  bignum_t x; init_bignum(x);
  const char* ptr = str.c_str();
  size_t i = 0;
  int neg = 0;

  if ((str.empty()) || (str == "null") || (str == "NULL"))
  {
    return x;
  }

  //
  // FIXME: 这里没有做str的长度验证，存在风险。
  //

  if (str[i] == '-') {
    i++;
    neg = 1;
  }

  if ((str[i] == '0') && ((str[i+1] == 'x') || (str[i+1] == 'X'))) {
    hex = true;
    i += 2;
  } else if  ((str[i] == 'x') || (str[i] == 'X')) {
    hex = true;
    i++;
  }

  // 操作指针
  ptr += i;

  if (hex) {
    x.number = __create_hex_str(ptr);
  } else {
    x.number = __create_dec_str(ptr);
  }
  // for (int i = static_cast<int>(x.number.size())-1; i >= 0; i--) {
  //   std::cout << std::hex << x.number[i];
  // }
  // std::cout << std::endl;
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
bignum_t create(unsigned char* buf, size_t len, bool little, bool is_sign) {
  return __bignum_bin2bn(buf, len, little, is_sign);
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
bignum_t create(const number_t& number, int neg) {
  bignum_t x;
  x.neg = neg;
  x.number = number;
  return x;
}

} // namespace mympz