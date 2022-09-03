static void __invalid_base(int base) {
  if (base != 10 && base != 2 && base != 8 && base != 16) {
    invalid_arguments_exception("base = %d", base);
  }
}

static bool __check_number_format(const char* number) {
  if (number == nullptr) return false;
  char* ptr = const_cast<char*>(number);
  do {
    if (!std::isdigit(*ptr)) return false;
  } while (*(++ptr));
  return true;
}

static number_t __internal_string_to_bignum(const char* number, int base=10, bool shrink_reverse=true) {
  number_t res;

  if (strlen(number) == 0 || number == nullptr) {
    __zero(res);
    return res;
  }

  __invalid_base(base);   // FIXME: 目前只支持10进制构造。

  // 检查number字符串的格式
  if (!__check_number_format(number)) {
    invalid_arguments_exception("number = %s", number);
  }

  char* ptr = const_cast<char*>(number);
  do {
    unit_t j = static_cast<unit_t>(*ptr - 48);
    res.push_front(j);
  } while (*(++ptr));

  __shrink_zero(res, shrink_reverse);
  if (res.empty()) res.push_back(0);      // 如果削减为空，则添加一个0保留位数。
  return res;
}

/*
 * 内部大数转大数单元
 */
static unit_t __internal_bignum_to_unit(const number_t& a, size_t s, size_t e) {
  my_assert(!a.empty(), "size of a = %l", a.size());

  unit_t b = 0, c = 0;
  for (size_t i = s; i < e; i++) {
    if (c == 0)
      c = 1;
    else
      c *= 10;
    
    b += c * a[i];
  }
  return b;
}

/*
 * 首先转换为内部的一个存储单元保存一个10进制数。
 * 然后从这个内部的存储格式转为正式运算的保存格式。
 */
static const number_t __internal_max_integer = __internal_string_to_bignum(UNIT_STRING);
number_t __string_to_bignum(const char* number) {
  number_t res, target = __internal_string_to_bignum(number);

  unit_t x = 0;
  while (true) {
    // 结束条件
    if (__cmp(target, __internal_max_integer) == -1) {
      x = __internal_bignum_to_unit(target, 0, target.size());
      res.push_back(x);
      break;
    }

    internal_division_result_t y = __div(target, __internal_max_integer);
    if (!__is_zero(y.second)) {
      x = __internal_bignum_to_unit(y.second, 0, y.second.size());
    } else {
      x = 0;
    }
    target = y.first;
    res.push_back(x);
  }

  return res;
}

static std::string __internal_bignum_to_string(const number_t& a) {
  std::string res = "";
  char c = 0;

  int i = static_cast<int>(a.size()) - 1;
  for (; i >= 0; i--) {
    c = static_cast<char>(a[i] + 48);
    res.push_back(c);
  }
  return res;
}

static std::string __bignum_to_string(const number_t& a) {
  std::string res = "";
  std::ostringstream ostr;
  std::string s;
  size_t e = 0;
  number_t x, y, b, o = __internal_string_to_bignum("1");
  __zero(y);

  //
  // 将unit_t进制转换为10进制数值
  //
  // for (int i = static_cast<int>(a.size()) - 1; i >= 0; i--) {
  for (size_t i = 0; i < a.size(); i++) {
    //
    // 计算当前的基
    //
    if (e == 0) {
      b = o;
    } else if (e == 1) {
      b = __internal_max_integer;
    } else {
      b = __mul(b, __internal_max_integer);
    }

    ostr << a[i];
    s = ostr.str();
    ostr.str("");
    x = __internal_string_to_bignum(s.c_str());
    x = __mul(x, b);
    y = __add(x, y, false);

    // 指数增加
    ++e;
  }
  return __internal_bignum_to_string(y);
}
