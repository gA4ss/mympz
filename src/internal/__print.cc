static const char __hex_str[] = "0123456789ABCDEF";     ///< 在输出转换时使用
std::string __print_string_hex(const number_ptr_t& x, size_t xl, bool hex) {
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

std::string __print_string_hex(const number_t& x, bool low_case) {
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

std::string __print_string_dec(const number_t& x) {
  return __ibn_b2s(x);
}

std::string __print_string(const number_t& x, bool hex, bool low_case) {
  return hex ? __print_string_hex()
}