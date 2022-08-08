#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz {

#include "./create/__internal_bn.cc"
#include "./create/__create.cc"

/**
  * @brief         将缓冲区数据转换为大数结构。
  * @param[in]     s          缓冲区
  * @param[in]     len        长度
  * @param[in]     little     小端字序
  * @param[in]     unsign     无符号
  * @return        构造好的大数结构。
  */
static bignum_t __bin2bn(const unsigned char *s, size_t len, 
                         bool little=true, bool is_sign=false) {
  bignum_t x;

  //
  // 按照字序进行设定读取方式
  // s2的任务就是负责确定符号位
  // s的任务是填充数值队列
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
  // 如果是有符号并且为负数
  // 那么
  // xor = 0xff
  // neg = 1
  // carry = 1
  //
  int neg = 0, x0r = 0, carry = 0;
  if (is_sign) {
    neg = !!(*s2 & 0x80);
    x0r = neg ? 0xff : 0x00;
    carry = neg;
  }

  //
  // 跳过符号位的扩展（|x0r|的值）
  // 如果是有符号，则跳过前面的0xff字节。
  // 如果是无符号，则跳过前面的0x00字节。
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
      //
      // 正数这里是相同的
      // 如果是负数则反码+1为负数
      //
      unit_t byte_xored = *s ^ x0r;                 ///< 求反码
      unit_t byte = (byte_xored + carry) & 0xff;    ///< 反码+1，转为负数的保存形式。

      carry = byte_xored > byte;
      l |= (byte << m);
    }
    x.number[i] = l;
  }

  //
  // 清除首位的0
  //
  clear_head_zero(x);
  return x;
}

static unsigned char __output_byte(char c) {
  unsigned char b = 0;
  if ((c >= 'a') && (c <= 'f')) {
    b = c - 87;//97 + 10;
  } else if ((c >= 'A') && (c <= 'F')) {
    b = c - 55;//65 + 10;
  } else if ((c >= '0') && (c <= '9'))  {
    b = c - 48;
  } else {
    b = 0;
  }
  return b;
}

static number_t __create_hex_str(const char* str) {
  size_t len = strlen(str);
  size_t buf_size = len % 2 == 0 ? len / 2 : len / 2 + 1;
  unsigned char* buf = new unsigned char [buf_size];
  memset(buf, 0, buf_size);

  size_t end = len % 2 == 0 ? 0 : 1;      ///< 包装fix_len是偶数
  size_t j = 0;
  for (int i = static_cast<int>(len - 1); i >= static_cast<int>(end); i -= 2) {
    unsigned char bl = __output_byte(str[i]), 
                  bh = __output_byte(str[i-1]);
    unsigned char b = bh * 16 + bl;
    buf[j++] = b;
  }

  if (len % 2) {
    buf[buf_size-1] = __output_byte(str[0]);
  }

  bignum_t x = __bin2bn(buf, buf_size);
  if (buf) delete [] buf;

  return x.number;
}

static number_t __create_dec_str(const char* str) {
  return __string_to_bignum(str);
}

/**
  * @brief         创建大数类型
  * @param[in]     str         大数的字符串，可存在负号'-'。
  * @param[in]     hex         字符串是16进制的。
  * @return        大数结构
  */
bignum_t create(std::string str, bool hex) {
  bignum_t x;
  const char* ptr = str.c_str();
  int neg = 0;

  if (str[0] == '-') {
    ptr++;
    neg = 1;
  }

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
  return __bin2bn(buf, len, little, is_sign);
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

static const char __hex_str[] = "0123456789ABCDEF";     ///< 在输出转换时使用
static std::string __print_string_hex(const number_t& x, bool low_case) {
  std::string str;
  int h = 4;
  int v = 0, z = 0;
  for (int i = static_cast<int>(x.size()-1); i >= 0; i--) {
    //
    // 遍历每个单位的每个字节
    //
    for (int j = UNIT_BITS2 - h; j >= 0; j -= h) {
      ///< 提出首位的0
      v = static_cast<int>((x[i] >> j) & 0x0f);
      if (z || v != 0) {
        char c = __hex_str[v];
        if (low_case) {
          if (v > 9) {
            c += 32;
          }
        }
        str.push_back(c);
        z = 1;
      }/* end if */
    }
  }/* end for */
  return str;
}

static std::string __print_string_dec(const number_t& x) {
  return __bignum_to_string(x);
}

/**
  * @brief         按照大数结构输出字符串。
  * @param[in]     x          大数结构。
  * @param[in]     hex        按照16进制输出。
  * @param[in]     low_case   如果是16进制，则按小写输出。
  * @return        返回一个字符串。
  */
std::string print_string(const bignum_t& x, bool hex, bool low_case) {
  std::string str;

  if (is_zero(x)) {
    str.push_back('0');
    return str;
  }

  if (x.neg) {
    str.push_back('-');
  }

  if (hex)
    str += __print_string_hex(x.number, low_case);
  else
    str += __print_string_dec(x.number);
  return str;
}

} // namespace mympz