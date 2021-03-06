typedef std::deque<unit_t> number_t;
typedef number_t::iterator __bignum_iter_t;
typedef std::pair<number_t, number_t> internal_division_result_t;

static bool __is_zero(const number_t& a) {
  if (a.empty())
    return false;
  
  for (size_t i = 0; i < a.size(); i++) {
    if (a[i] != 0)
      return false;
  }
  return true;
}

void __zero(number_t& a) {
  a.clear();
  a.push_back(0);
}

static size_t __shrink_zero(number_t& a, bool reverse=false) {
  if (a.empty()) return 0;
  if ((a.size() == 1) && (a[0] == 0)) return 0;

  size_t ret = 0;
  if (reverse) {
    int l = static_cast<int>(a.size());
    for (int i = l-1; i > 0; i--) {
      if (a[i] == 0) {
        a.pop_back(); ++ret;
      } else {
        break;
      }
    }
  } else {
    __bignum_iter_t it = a.begin();
    while (it < a.end()) {
      if (*it == 0) {
        it = a.erase(it);
        ++ret;
      } else {
        break;
      }
    }
  }
  return ret;
}

static int __cmp(const number_t& a, const number_t& b, bool push_front=false) {
  int res = 0, l = 0;
  number_t _a = a, _b = b;

  if (_a.empty() || _b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }

  if (_a.size() > _b.size()) {
    l = static_cast<int>(_a.size() - _b.size());
    for (int i = 0; i < l; i++) {
      if (push_front) _b.push_front(0);
      else _b.push_back(0);
    }
  } else if (_a.size() < _b.size()) {
    l = static_cast<int>(_b.size() - _a.size());
    for (int i = 0; i < l; i++) {
      if (push_front) _a.push_front(0);
      else _a.push_back(0);
    }
  }

  my_assert(
    _a.size() == _b.size(), 
    "(_a.size() = %d, _b.size() = %d) is not equal.", 
    _a.size(), _b.size()
  );

  l = static_cast<int>(_a.size());
  for (int i = l-1; i >= 0; i--) {
    if (_a[i] > _b[i]) {
      res = 1;
      break;
    } else if (_a[i] < _b[i]) {
      res = -1;
      break;
    }
  }
  return res;
}

static number_t __add(const number_t& a, const number_t& b, bool o) {
  number_t x, y, z;

  if (a.empty() || b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }

  if (a.empty() && !b.empty()) {
    x.push_back(0);
    y = b;
  } else if (!a.empty() && b.empty()) {
    x = a;
    y.push_back(0);
  } else if (a.empty() && b.empty()) {
    x.push_back(0);
    y.push_back(0);
  } else {
    x = a;
    y = b;
  }

  if (x.size() > y.size()) {
    y.resize(x.size());
  } else if (x.size() < y.size()) {
    x.resize(y.size());
  }

  unit_t c = 0, r = static_cast<unit_t>(o);
  size_t l = x.size();
  for (size_t i = 0; i < l; i++) {
    c = x[i] + y[i] + r;
    if (c >= 10) {
      c = c - 10;
      r = 1;
    } else {
      r = 0;
    }
    z.push_back(c);
  }
  // ???????????????????????????
  if (r) z.push_back(r);
  return z;
}

static number_t __sub(number_t& a, number_t& b, bool t) {
  number_t x, y, z;

  if (a.empty() || b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }

  if (a.empty() && !b.empty()) {
    x.push_back(0);
    y = b;
  } else if (!a.empty() && b.empty()) {
    x = a;
    y.push_back(0);
  } else if (a.empty() && b.empty()) {
    x.push_back(0);
    y.push_back(0);
  } else {
    x = a;
    y = b;
  }

  //
  // x??? y????????????????????????
  //
  if (__cmp(x, y) < 0) {
    z = x;
    x = y;
    y = z;
    z.clear();
  }

  if (x.size() > y.size()) {
    y.resize(x.size());
  } else if (x.size() < y.size()) {
    x.resize(y.size());
  }

  unit_t r = static_cast<unit_t>(t), c = 0;
  size_t l = x.size();
  for (size_t i = 0; i < l; i++) {
    if (static_cast<sunit_t>(x[i]) - static_cast<sunit_t>(r) >= static_cast<sunit_t>(y[i])) {
      c = x[i] - y[i] - r;
      r = 0;
    } else {
      c = x[i] + 10 - y[i] - r;
      r = 1;
    }
    z.push_back(c);
  }
  if (r != 0) {
    r = 1;
  }
  my_assert(r == 0, "%s", "borrow must be 0.");
  return z;
}

static number_t __mul(const number_t& a, const number_t& b) {
  if (a.empty() || b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }

  number_t x, y, z;
  size_t m = 0, n = 0;

  // ?????? m >= n
  if (a.size() >= b.size()) {
    x = a; m = a.size();
    y = b; n = b.size();
  } else if (a.size() < b.size()) {
    y = a; n = a.size();
    x = b; m = b.size();
  }
  z.resize(m+n);

  unit_t q = 0, t = 0;
  for (size_t j = 0; j < n; j++) {
    q = 0;
    for (size_t i = 0; i < m; i++) {
      t = x[i] * y[j] + z[i+j] + q;
      z[i+j] = t % 10;
      q = t / 10;
    }
    z[j+m] = q;
  }

  __shrink_zero(z, true);
  return z;
}

/*
 * a1??????a?????????????????????????????????B??????????????????
 * 1.???a,b??????????????????a1 >= b1???a > b???
 *  1-1 : a1 = b1 ---> q = 1.
 *  1-2 : a1 > b1 ---> q1 = floor(a_1/b_1), q2 = max(floor( (a1*B+a2-B+1) / (b1*B+b2)) ), 1)
 *        q1 <= q <= q2
 * 2.???a????????????b????????????a1 <= b1???a > b, a < b*B(????????????)???
 *  2-1 : q1 = min(floor( (a1*B+a2) / b1 ), B-1)
 *        q <= q1
 * ????????????????????????????????????????????????????????? 25-26??????
 */
static std::pair<unit_t, unit_t> __get_quotient_range(const number_t& a, const number_t& b) {
  const unit_t B = 10;
  unit_t a1 = *(a.end()-1), b1 = *(b.end()-1);
  unit_t a2 = 0, b2 = 0;

  if (a.size() >= 2) a2 = *(a.end()-2);
  if (b.size() >= 2) b2 = *(b.end()-2);

  if (a.size() == b.size()) {
    //
    // ???????????????
    //
    if (a1 == b1)
      return std::make_pair(1, 1);
    else { // ??????????????? a1 > b1
      my_assert(a1 > b1, "a1(%d) < b1(%d)", a1, b1);
      unit_t q1 = a1 / b1, q2 = std::max<unit_t>((a1*B+a2-B+1)/(b1*B+b2), 1);
      return q1 < q2 ? std::make_pair(q1, q2) : std::make_pair(q2, q1);
    }
  } else {
    //
    // ???????????????
    //
    unit_t q = std::min((a1*B+a2)/b1, B-1);
    return std::make_pair(1, q);
  }
  my_assert(false, "%s", "never go here!");
  return std::make_pair(0, 0);
}

/* ??????????????? */
static internal_division_result_t __div(const number_t& a, const number_t& b) {
  if (a.empty() || b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }
  if (__is_zero(b)) mympz_exception("%s", "b is zero.");
  //const number_t ten = {0, 1};    // ??????10

  number_t x = a, y = b;
  if (__cmp(x, y) == 0)
    return internal_division_result_t({1}, {0});
  else if (__cmp(x, y) == -1)
    return internal_division_result_t({0}, {x});

  number_t quotient, product, dividend = x, divisor = y, remainder = {0};
  size_t dividend_remainder_digits = dividend.size();

  //
  // ??????????????????????????????????????????????????????
  // ????????????a???b??????????????? : a_n, b_n
  // ?????? a_n < b_n, ??????a_{n+1}???
  //
  size_t n = divisor.size();
  dividend_remainder_digits -= n;
  dividend = number_t(x.begin()+dividend_remainder_digits, x.end());
  if (__cmp(dividend, divisor) == -1) {
    // ??????????????????dividend??????divisor????????????
    --dividend_remainder_digits;
    dividend = number_t(x.begin()+dividend_remainder_digits, x.end());
  }

  // ???????????????????????????????????????????????????
  while (true) {
    // ???????????????????????????
    std::pair<unit_t, unit_t> quo_range = __get_quotient_range(dividend, divisor);
    unit_t q = 0;
    for (q = quo_range.first; q <= quo_range.second; q++) {
      product = __mul(divisor, {q});
      __shrink_zero(product, true); // ???????????????????????????0???
      //
      // product ?????????????????? dividend
      //
      remainder = __sub(dividend, product, false);
      int c = __cmp(divisor, remainder);
      if ((c == -1) || (c == 0)) {
        continue;
      } else {
        quotient.push_front(q);
        break;
      }
    }

    //
    // ???????????????????????????????????????????????????
    //
    if (dividend_remainder_digits == 0)
      break;

    //
    // ???????????????
    //
    dividend = remainder;

    //
    // ?????????????????????????????????????????????????????????
    // ??????????????????????????????0?????????
    // ??????????????????????????????
    //
    if ((__cmp(dividend, divisor) == -1) && (dividend_remainder_digits != 0)) {
      if (__cmp(dividend, {0}) == 0) dividend.clear();  // ?????????????????????10000/2???????????????
      dividend.push_front(x[dividend_remainder_digits-1]);
      --dividend_remainder_digits;

      //
      // ??????????????????????????????????????????????????????
      //
      while ((__cmp(dividend, divisor) == -1) && (dividend_remainder_digits != 0)) {
        dividend.push_front(x[dividend_remainder_digits-1]);
        quotient.push_front(0);
        --dividend_remainder_digits;
      }
    }

    //
    // ????????????1,?????????8589934590 / 791621423
    // ????????????????????????????????????????????????????????????????????? dividend_remainder_digits???0???
    // quotient??????0???dividend????????????
    //
    if (__cmp(dividend, divisor) == -1) {
      quotient.push_front(0);
      break;
    }

    //
    // ????????????2
    // ??????????????????????????????0???????????????????????????????????? dividend_remainder_digits???0???
    // quotient??????0???dividend????????????
    //
    if (__cmp(dividend, {0}) == 0) {
      // quotient.insert(quotient.begin(), dividend.begin(), dividend.end());
      quotient.push_front(0);
      break;
    }

    //
    // ?????????10001 / 2 ??????????????????????????? dividend = 0010
    // ?????????????????????0?????????
    //
    if (dividend.back() == 0) __shrink_zero(dividend, true);
  }/* end while */

  //
  // ????????????
  //
  remainder = dividend;
  // __shrink_zero(remainder, true);
  return internal_division_result_t(quotient, remainder);
}

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

  __invalid_base(base);   // FIXME: ???????????????10???????????????

  // ??????number??????????????????
  if (!__check_number_format(number)) {
    invalid_arguments_exception("number = %s", number);
  }

  char* ptr = const_cast<char*>(number);
  do {
    unit_t j = static_cast<unit_t>(*ptr - 48);
    res.push_front(j);
  } while (*(++ptr));

  __shrink_zero(res, shrink_reverse);
  if (res.empty()) res.push_back(0);      // ????????????????????????????????????0???????????????
  return res;
}

/*
 * ???????????????????????????
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
 * ??????????????????????????????????????????????????????10????????????
 * ????????????????????????????????????????????????????????????????????????
 */
static const number_t __internal_max_integer = __internal_string_to_bignum(UNIT_STRING);
number_t __string_to_bignum(const char* number) {
  number_t res, target = __internal_string_to_bignum(number);

  unit_t x = 0;
  while (true) {
    // ????????????
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
  // ???unit_t???????????????10????????????
  //
  // for (int i = static_cast<int>(a.size()) - 1; i >= 0; i--) {
  for (size_t i = 0; i < a.size(); i++) {
    //
    // ??????????????????
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

    // ????????????
    ++e;
  }
  return __internal_bignum_to_string(y);
}
