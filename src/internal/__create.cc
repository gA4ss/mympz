/**
  * @brief         将缓冲区数据转换为大数结构。
  * @param[in]     s          缓冲区
  * @param[in]     len        长度
  * @param[in]     little     小端字序
  * @param[in]     unsign     无符号
  * @return        构造好的大数结构。
  */
number_t __bin2bn(const unsigned char *s, size_t len, int* oneg, bool little, bool is_sign) {
  number_t x;

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
    x.push_back(0);
    return x;
  }

  //
  // 给结果分配空间
  //
  size_t n = ((len - 1) / UNIT_BYTES) + 1;      ///< 返回结构的长度
  num_resize(x, n);
  if (oneg) *oneg = neg;

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
    x[i] = l;
  }

  //
  // 清除首位的0
  //
  size_t l = x.size();
  while (l && x[l-1] == 0) {
    --l;
  }
  x.resize(l);
  return x;
}

size_t __bn2bin(const number_t& x, int neg, unsigned char *to, size_t tolen, 
                bool little, bool is_sign) {
  if (x.empty()) {
    mympz_exception("%s", "bignum is null.");
  }

  size_t n8 = __number_bits(x);           // 获取总共的位数
  size_t n = (n8 + 7) / 8;                // 获取需要多少个字节

  // 存在符号
  int x0r = 0, carry = 0;
  size_t ext = 0;
  if (is_sign == true) {
    x0r = neg ? 0xff : 0x00;
    carry = neg;

    // 位数与字节*8相当，说明数值的最高位为1。
    ext = (n * 8 == n8)
        ? !neg            /* 最高位设置为非负数 */
        : neg;            /* 最高位没有设置为非负数 */
  }

  // 修正要输出的长度
  if (tolen == UNIT_MAX) {
    tolen = n + ext;
  } else if (tolen < n + ext) {
    mympz_exception("buffer size is not enought. size = \'%lu\'", tolen);
  }

  if (tolen != 0)
    memset((void*)to, '\0', tolen);

  int inc = 0;
  if (little == true) {
    inc = 1;
  } else {
    inc = -1;
    to += tolen - 1;
  }

  size_t xl = num_size(x);                      // x的长度
  size_t lasti = xl - 1;                        // x的最低位索引
  size_t xs = xl * UNIT_BYTES;                  // x总共有多少个字节
  unit_t l = 0, mask = 0;
  for (size_t i = 0, j = 0; j < (size_t)tolen; j++) {
    unsigned char byte, byte_xored;
    l = x[i / UNIT_BYTES];                      // 取得到哪个位置
    mask = 0 - ((j - xs) >> (8 * sizeof(i) - 1));
    byte = (unsigned char)(l >> (8 * (i % UNIT_BYTES)) & mask);
    byte_xored = byte ^ x0r;
    *to = (unsigned char)(byte_xored + carry);
    carry = byte_xored > *to;                   // 蕴含 1 或者 0
    to += inc;
    i += (i - lasti) >> (8 * sizeof(i) - 1);
  }

  return tolen;
}

unsigned char __output_byte(char c) {
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

number_t __create_hex_str(const char* str) {
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

  number_t x = __bin2bn(buf, buf_size);
  if (buf) delete [] buf;

  return x;
}

number_t __create_dec_str(const char* str) {
  return __string_to_bignum(str);
}
