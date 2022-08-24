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
    number_ptr_t it = a.begin();
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
  // 如果存在进位则保存
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
  // x比 y小则交换两者位置
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

  // 保证 m >= n
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
 * a1表示a的首项系数，以此类推。B为某个进制。
 * 1.当a,b的位数一致，a1 >= b1，a > b。
 *  1-1 : a1 = b1 ---> q = 1.
 *  1-2 : a1 > b1 ---> q1 = floor(a_1/b_1), q2 = max(floor( (a1*B+a2-B+1) / (b1*B+b2)) ), 1)
 *        q1 <= q <= q2
 * 2.当a的位数比b大一位，a1 <= b1，a > b, a < b*B(仅差一位)。
 *  2-1 : q1 = min(floor( (a1*B+a2) / b1 ), B-1)
 *        q <= q1
 * 证明见《计算机代数》【陈玉福，张智勇】 25-26页。
 */
static std::pair<unit_t, unit_t> __get_quotient_range(const number_t& a, const number_t& b) {
  const unit_t B = 10;
  unit_t a1 = *(a.end()-1), b1 = *(b.end()-1);
  unit_t a2 = 0, b2 = 0;

  if (a.size() >= 2) a2 = *(a.end()-2);
  if (b.size() >= 2) b2 = *(b.end()-2);

  if (a.size() == b.size()) {
    //
    // 第一种情况
    //
    if (a1 == b1)
      return std::make_pair(1, 1);
    else { // 这里一定是 a1 > b1
      my_assert(a1 > b1, "a1(%d) < b1(%d)", a1, b1);
      unit_t q1 = a1 / b1, q2 = std::max<unit_t>((a1*B+a2-B+1)/(b1*B+b2), 1);
      return q1 < q2 ? std::make_pair(q1, q2) : std::make_pair(q2, q1);
    }
  } else {
    //
    // 第二种情况
    //
    unit_t q = std::min((a1*B+a2)/b1, B-1);
    return std::make_pair(1, q);
  }
  my_assert(false, "%s", "never go here!");
  return std::make_pair(0, 0);
}

/* 不考虑符号 */
static internal_division_result_t __div(const number_t& a, const number_t& b) {
  if (a.empty() || b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }
  if (__is_zero(b)) mympz_exception("%s", "b is zero.");
  //const number_t ten = {0, 1};    // 表示10

  number_t x = a, y = b;
  if (__cmp(x, y) == 0)
    return internal_division_result_t({1}, {0});
  else if (__cmp(x, y) == -1)
    return internal_division_result_t({0}, {x});

  number_t quotient, product, dividend = x, divisor = y, remainder = {0};
  size_t dividend_remainder_digits = dividend.size();

  //
  // 这里对被除数与除数做一些初始化工作，
  // 从末尾取a与b相同的位数 : a_n, b_n
  // 如果 a_n < b_n, 则取a_{n+1}。
  //
  size_t n = divisor.size();
  dividend_remainder_digits -= n;
  dividend = number_t(x.begin()+dividend_remainder_digits, x.end());
  if (__cmp(dividend, divisor) == -1) {
    // 如果小于这里dividend必比divisor多一位。
    --dividend_remainder_digits;
    dividend = number_t(x.begin()+dividend_remainder_digits, x.end());
  }

  // 第一次运行必须保证被除数大于除数。
  while (true) {
    // 尝试获取商的范围。
    std::pair<unit_t, unit_t> quo_range = __get_quotient_range(dividend, divisor);
    unit_t q = 0;
    for (q = quo_range.first; q <= quo_range.second; q++) {
      product = __mul(divisor, {q});
      __shrink_zero(product, true); // 乘法可能产生多余的0。
      //
      // product 必然小于等于 dividend
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
    // 算法结束条件：被除数没有多余的位。
    //
    if (dividend_remainder_digits == 0)
      break;

    //
    // 更新被除数
    //
    dividend = remainder;

    //
    // 这里首先借一位，如果大于则直接做运算。
    // 如果还是小于则商使用0补位。
    // 保证被除数大于除数。
    //
    if ((__cmp(dividend, divisor) == -1) && (dividend_remainder_digits != 0)) {
      if (__cmp(dividend, {0}) == 0) dividend.clear();  // 余数为零，多见10000/2这种情况。
      dividend.push_front(x[dividend_remainder_digits-1]);
      --dividend_remainder_digits;

      //
      // 这里如果借了一位还是小于那么开始补位
      //
      while ((__cmp(dividend, divisor) == -1) && (dividend_remainder_digits != 0)) {
        dividend.push_front(x[dividend_remainder_digits-1]);
        quotient.push_front(0);
        --dividend_remainder_digits;
      }
    }

    //
    // 特殊情况1,例如：8589934590 / 791621423
    // 如果被除数还是小于除数，这里蕴含的一个条件就是 dividend_remainder_digits为0。
    // quotient压入0比dividend少一个。
    //
    if (__cmp(dividend, divisor) == -1) {
      quotient.push_front(0);
      break;
    }

    //
    // 特殊情况2
    // 如果被除数剩余全部是0，这里蕴含的一个条件就是 dividend_remainder_digits为0。
    // quotient压入0比dividend少一个。
    //
    if (__cmp(dividend, {0}) == 0) {
      // quotient.insert(quotient.begin(), dividend.begin(), dividend.end());
      quotient.push_front(0);
      break;
    }

    //
    // 例如：10001 / 2 这种情况，可能造成 dividend = 0010
    // 这里将前面两个0清除。
    //
    if (dividend.back() == 0) __shrink_zero(dividend, true);
  }/* end while */

  //
  // 更新余数
  //
  remainder = dividend;
  // __shrink_zero(remainder, true);
  return internal_division_result_t(quotient, remainder);
}