#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{
  /**
   * @brief         一个大数与一个字取模数
   * @param[in]     x 大数
   * @param[in]     w 一个字
   * @return        r = x % w
   */
  unit_t mod(const bignum_t &x, unit_t w, int wneg)
  {
    unit_t r = 0;

    if (w == 0)
    {
      mympz_divide_by_zero(w);
    }

    if (w > ((unit_t)1 << UNIT_HALF_BITS))
    {
      div(x, w, wneg, &r);
      return r;
    }

    size_t xl = bn_size(x);
    w &= CALC_MASK;
    for (int i = static_cast<int>(xl) - 1; i >= 0; i--)
    {
      r = ((r << UNIT_HALF_BITS) | ((x.number[i] >> UNIT_HALF_BITS) & CALC_MASKl)) % w;
      r = ((r << UNIT_HALF_BITS) | (x.number[i] & CALC_MASKl)) % w;
    }
    return r;
  }

  /**
   * @brief         有符号的两个大数的模数
   * @param[in]     x
   * @param[in]     y
   * @return        r = x % y
   */
  // bignum_t mod(const bignum_t& x, const bignum_t& y) {
  //   division_result_t z = div(x, y);
  //   return z.second;
  // }
  bignum_t mod(const bignum_t &x, const bignum_t &y)
  {
    bignum_t r; r.neg = 0;
    division_result_t res = div(x, y);
    r = res.second;
    return r;
  }

  /**
   * @brief         有符号的两个大数去与模|y|
   * @param[in]     x
   * @param[in]     y
   * @return        r = x % |y|
   * @note 与mod运算一致，但是返回非负的余数。
   *       $0 <= r < |y|$
   */
  static bignum_t __nnmod(const bignum_t &x, const bignum_t &y)
  {
    bignum_t r = mod(x, y);
    if (!r.neg) // 如果为正数
      return r;
    /* 现在 -|y| < r < 0, 因此我们需要设置 r = r + |y| */
    if (y.neg)
      return sub(r, y);
    return add(r, y);
  }

  /**
   * @brief         有符号的两个大数的模数相加取模后取绝对值
   * @param[in]     x
   * @param[in]     y
   * @return        r = (x + y) % |m|
   */
  bignum_t mod_add(const bignum_t &x, const bignum_t &y, const bignum_t &m)
  {
    bignum_t z;
    // x,y > 0 && x,y <= m
    if ((cmp(x, m) <= 0) && (cmp(y, m) <= 0) &&
        (x.neg == 0) && (y.neg == 0))
    {
      bignum_t _x = bn_const_cast(x);
      bignum_t _y = bn_const_cast(y);
      bignum_t _m = bn_const_cast(m);
      z.number = __mod_add(bn_ptr(_x), bn_size(_x), bn_ptr(_y), bn_size(_y),
                           bn_ptr(_m), bn_size(_m));
      z.neg = 0;
      return z;
    }
    z = add(x, y);
    return __nnmod(z, m);
  }

  /**
   * @brief         有符号的两个大数的模数相减取模后取绝对值
   * @param[in]     x
   * @param[in]     y
   * @return        r = (x - y) % |m|
   */
  bignum_t mod_sub(const bignum_t &x, const bignum_t &y, const bignum_t &m)
  {
    bignum_t z;
    // x,y > 0 && x,y <= m
    if ((cmp(x, m) <= 0) && (cmp(y, m) <= 0) &&
        (x.neg == 0) && (y.neg == 0))
    {
      bignum_t _x = bn_const_cast(x);
      bignum_t _y = bn_const_cast(y);
      bignum_t _m = bn_const_cast(m);
      z.number = __mod_sub(bn_ptr(_x), bn_size(_x), bn_ptr(_y), bn_size(_y),
                           bn_ptr(_m), bn_size(_m));
      z.neg = 0;
      return z;
    }
    z = sub(x, y);
    return __nnmod(z, m);
  }

  /**
   * @brief         有符号的两个大数的模数相乘取模后取绝对值
   * @param[in]     x
   * @param[in]     y
   * @return        r = (x * y) % |m|
   */
  bignum_t mod_mul(const bignum_t &x, const bignum_t &y, const bignum_t &m)
  {
    bignum_t z;

    if (cmp(x, y) == 0)
    {
      z = sqr(x);
      return z;
    }

    z = mul(x, y);
    return __nnmod(z, m);
  }

  /**
   * @brief         大数的平方取模后取绝对值
   * @param[in]     x
   * @return        r = (x * x) % m
   */
  bignum_t mod_sqr(const bignum_t &x, const bignum_t &m)
  {
    bignum_t y = sqr(x);
    return mod(y, m);
  }

  /**
   * @brief         大数的左移1位取模|m|
   * @param[in]     x
   * @return        r = (x << 1) % |m|
   */
  bignum_t mod_lshift1(const bignum_t &x, const bignum_t &m)
  {
    bignum_t r = lshift1(x);
    if (cmp(r, m) >= 0)
    {
      return sub(r, m);
    }
    return __nnmod(r, m);
  }

  /**
   * @brief         大数的左移n位取模|m|
   * @param[in]     x x < m && x >= 0
   * @param[in]     n 要移动的位数
   * @return        r = x << n
   */
  bignum_t __mod_lshift(const bignum_t &x, int n, const bignum_t &m)
  {
    bignum_t r = x;

    while (n > 0)
    {
      int max_shift;

      /* 0 < r < m */
      max_shift = static_cast<int>(__number_bits(m.number) - __number_bits(r.number));
      /* max_shift >= 0 */
      if (max_shift < 0)
      {
        mympz_exception("%s", "invalid argument : r > m");
      }

      if (max_shift > n)
        max_shift = n;

      // m与x的位数相同
      if (max_shift)
      {
        r = lshift(r, max_shift);
        n -= max_shift;
      }
      else
      {
        r = lshift1(r);
        --n;
      }

      if (cmp(r, m) >= 0)
      {
        r = sub(r, m);
      }
    }

    return r;
  }

  /**
   * @brief         大数的左移n位取模后模|m|
   * @param[in]     x
   * @param[in]     n 要移动的位数
   * @return        r = x % |m| << n
   */
  bignum_t mod_lshift(const bignum_t &x, size_t n, const bignum_t &m)
  {
    bignum_t r, abs_m;

    r = __nnmod(x, m);

    // 如果模数为负数
    if (m.neg)
    {
      abs_m = m;
      abs_m.neg = 0;
    }

    return __mod_lshift(r, static_cast<int>(n), (m.neg ? abs_m : m));
  }

  /**
   * @brief         求模逆
   * @param[in]     x
   * @param[in]     m 要移动的位数
   * @return        x^{-1}x = 1 (mod m)
   */
  bignum_t mod_inverse(const bignum_t &x, const bignum_t &m)
  {
    mympz_dbgprint_fmt_modinv("x = %s.\n", print_string(x).c_str());
    mympz_dbgprint_fmt_modinv("m = %s.\n", print_string(m).c_str());

    bignum_t A, B, X, Y, M, D, T, R;

    /* 如果模数为1或者0，则直接返回空值。 */
    if (is_zero(m) || is_one(m))
    {
      mympz_dbgprint_modinv("modulo is 0 or 1.\n");
      set_null(R);
      return R;
    }

    /* 如果被除数与模数相等则无模逆 */
    if (cmp(x, m) == 0)
    {
      mympz_dbgprint_modinv("dividend equal to modulo.\n");
      set_null(R);
      return R;
    }

    one(X);
    zero(Y);

    B = x;
    A = m;
    A.neg = 0;
    //
    // 如果被除数的符号为负号，或者比模数大，则进行约减。
    //
    if (B.neg || (ucmp(B, A) > 0))
    {
      mympz_dbgprint_modinv("dividend greater than modulo.\n");
      B = __nnmod(B, A);
      mympz_dbgprint_fmt_modinv("after reduce:\ndividend = %s.\nmodulo = %s.\n",
                                print_string(B).c_str(),
                                print_string(A).c_str());
    }
    int sign = -1;

    //
    // B = x mod |m|,  A = |m| 得到
    // 0 <= B < A,
    // -sign*X*x  ==  B   (mod |m|),
    // sign*Y*x  ==  A   (mod |m|).
    //
    if (is_odd(m) && (bignum_bits(m) <= 2048))
    {
      mympz_dbgprint_fmt_modinv("modulo is odd and bits(%lu) less then 2048.\n", bignum_bits(m));
      //
      // 二进制模逆算法；需要奇数的模数。如果模数充分小这比一般的模算法更快。
      // 在32位系统上大概400 - 500位，在64位系统上更多些。
      //
      int shift;
      while (!is_zero(B))
      {
        mympz_dbgprint_fmt_modinv("B = %s.\n", print_string(B).c_str());
        //
        //      0 < B < |m|,
        //      0 < A <= |m|,
        // (1) -sign*X*x  ==  B   (mod |m|),
        // (2)  sign*Y*x  ==  A   (mod |m|)
        //
        //
        // 现在用整数的最大的2次幂除以B，并且使用同样数模|n|再除以X。
        // 当所有的完成后，(1)式成立。
        //
        shift = 0;
        while (!is_bit_set(B, shift))
        { /* 0 < B */
          mympz_dbgprint_fmt_modinv("B(%lu) = %s.\n", shift, print_string(B).c_str());
          shift++;

          if (is_odd(X))
          {
            mympz_dbgprint_fmt_modinv("X is odd.\nX = %s.\n", print_string(X).c_str());
            X = uadd(X, m);
            mympz_dbgprint_fmt_modinv("B(%lu) = %s.\n", shift, print_string(B).c_str());
          }

          // 现在X是偶数，因此我们能简单的将它除以2。
          X = rshift1(X);
          mympz_dbgprint_fmt_modinv("X >> 1.\nX = %s.\n", print_string(X).c_str());
        }
        if (shift > 0)
        {
          mympz_dbgprint_fmt_modinv("shift(%lu) > 0.\n", shift);
          B = rshift(B, shift);
          mympz_dbgprint_fmt_modinv("B >> %lu.\nB = %s.\n", shift, print_string(B).c_str());
        }

        //
        // 同样对于A与Y，运行以下代码后，式(2)成立。
        //
        shift = 0;
        mympz_dbgprint_modinv("shift <- 0.\n");
        while (!is_bit_set(A, shift))
        { /* 0 < A */
          mympz_dbgprint_fmt_modinv("A(%lu) = %s.\n", shift, print_string(A).c_str());
          shift++;

          if (is_odd(Y))
          {
            mympz_dbgprint_fmt_modinv("Y is odd.\nY = %s.\n", print_string(Y).c_str());
            Y = uadd(Y, m);
            mympz_dbgprint_fmt_modinv("Y = Y + m.\nY = %s.\nm = %s.\n",
                                      print_string(Y).c_str(),
                                      print_string(m).c_str());
          }
          /* Y是偶数 */
          Y = rshift1(Y);
          mympz_dbgprint_fmt_modinv("Y >> 1.\nY = %s.\n", print_string(Y).c_str());
        }
        if (shift > 0)
        {
          mympz_dbgprint_fmt_modinv("shift = %lu > 0.\n", shift);
          A = rshift(A, shift);
          mympz_dbgprint_fmt_modinv("A >> %lu.\nA = %s.\n", shift, print_string(A).c_str());
        }

        //
        // 式(1),(2)都满足。
        // A与B是奇数。
        // 确保以下计算正确。
        //
        //     0 <= B < |n|,
        //      0 < A < |n|,
        // (1) -sign*X*a  ==  B   (mod |n|),
        // (2)  sign*Y*a  ==  A   (mod |n|),
        //
        // 下面的迭代要么A是偶数要么B是偶数。
        //
        //
        if (ucmp(B, A) >= 0)
        {
          mympz_dbgprint_fmt_modinv("B >= A.\nB = %s.\nA = %s.\n",
                                    print_string(B).c_str(),
                                    print_string(A).c_str());
          /* -sign*(X + Y)*a == B - A  (mod |n|) */
          X = uadd(X, Y);
          mympz_dbgprint_fmt_modinv("X = X + Y.\nX = %s.\nY = %s\n",
                                    print_string(X).c_str(),
                                    print_string(Y).c_str());
          B = usub(B, A);
          mympz_dbgprint_fmt_modinv("B = B - A.\nB = %s.\nA = %s\n",
                                    print_string(B).c_str(),
                                    print_string(A).c_str());
        }
        else
        {
          /*  sign*(X + Y)*a == A - B  (mod |n|) */
          Y = uadd(Y, X);
          mympz_dbgprint_fmt_modinv("Y = Y + X.\nY = %s.\nX = %s\n",
                                    print_string(Y).c_str(),
                                    print_string(X).c_str());
          A = usub(A, B);
          mympz_dbgprint_fmt_modinv("A = A - B.\nA = %s.\nB = %s\n",
                                    print_string(A).c_str(),
                                    print_string(B).c_str());
        }
      }
    }
    else
    {
      mympz_dbgprint_modinv("general modulo inverse.\n");
      /* 一般的模逆算法 */
      while (!is_zero(B))
      {
        bignum_t tmp;
        //
        //      0 < B < A,
        // (*) -sign*X*x  ==  B   (mod |m|),
        //      sign*Y*x  ==  A   (mod |m|)
        //

        /* (D, M) := (A/B, A%B) ... */
        if (bignum_bits(A) == bignum_bits(B))
        {
          mympz_dbgprint_fmt_modinv("A bits(%lu) == B bits(%lu).\n",
                                    bignum_bits(A), bignum_bits(B));
          mympz_dbgprint_fmt_modinv("A = %s.\nB = %s",
                                    print_string(A).c_str(),
                                    print_string(B).c_str());
          one(D);
          M = sub(A, B);
          mympz_dbgprint_fmt_modinv("M = %s.\n", print_string(M).c_str());
        }
        else if (bignum_bits(A) == bignum_bits(B) + 1)
        {
          mympz_dbgprint_fmt_modinv("A bits(%lu) == B bits(%lu) + 1.\n",
                                    bignum_bits(A), bignum_bits(B));
          /* A/B 是 1, 2, 或者 3 */
          T = lshift1(B);
          mympz_dbgprint_fmt_modinv("T == %s.\n",
                                    print_string(T).c_str());
          if (ucmp(A, T) < 0)
          {
            mympz_dbgprint_fmt_modinv("A < T.\nA = %s.\n",
                                      print_string(A).c_str());
            /* A < 2*B, 因此 D=1 */
            one(D);
            M = sub(A, B);
            mympz_dbgprint_fmt_modinv("T = A - B.\nB = %s.\nT = %s.\n",
                                      print_string(B).c_str(),
                                      print_string(T).c_str());
          }
          else
          {
            mympz_dbgprint_fmt_modinv("A >= T.\nA = %s.\nT = %s.\n",
                                      print_string(A).c_str(),
                                      print_string(T).c_str());
            /* A >= 2*B, 因此 D=2 或者 D=3 */
            M = sub(A, T);
            mympz_dbgprint_fmt_modinv("M = A - T.\nM = %s.\nA = %s.\nT = %s.\n",
                                      print_string(M).c_str(),
                                      print_string(A).c_str(),
                                      print_string(T).c_str());
            D = add(T, B);
            mympz_dbgprint_fmt_modinv("D = T + B.\nD = %s.\nT = %s.\nB = %s.\n",
                                      print_string(D).c_str(),
                                      print_string(T).c_str(),
                                      print_string(B).c_str());
            if (ucmp(A, D) < 0)
            {
              mympz_dbgprint_fmt_modinv("A < D.\nA = %s.\nD = %s.\n",
                                        print_string(A).c_str(),
                                        print_string(D).c_str());
              /* A < 3*B, 因此 D=2 */
              set_word(D, 2);
              mympz_dbgprint_fmt_modinv("D <- 2.\nD = %s.\n",
                                        print_string(D).c_str());
              //
              // M (= A - 2*B) 已经有了正确的值。
              //
            }
            else
            {
              mympz_dbgprint_fmt_modinv("A >= D.\nA = %s.\nD = %s.\n",
                                        print_string(A).c_str(),
                                        print_string(D).c_str());
              /* 仅仅 D=3 剩余 */
              set_word(D, 3);
              mympz_dbgprint_fmt_modinv("D <- 3.\nD = %s.\n",
                                        print_string(D).c_str());
              //
              // 当前 M = A - 2*B，但是我们需要 M = A - 3*B
              //
              M = sub(M, B);
              mympz_dbgprint_fmt_modinv("M = M - B.\nM = %s.\nB = %s.\n",
                                        print_string(M).c_str(),
                                        print_string(B).c_str());
            }
          }
        }
        else
        {
          mympz_dbgprint_fmt_modinv("A bits(%lu), B bits(%lu).\n",
                                    bignum_bits(A), bignum_bits(B));
          division_result_t res = div(A, B);
          D = res.first;
          M = res.second;
          mympz_dbgprint_fmt_modinv("D, M = A / B.\nD = %s.\nM = %s.\nA = %s.\nB = %s.\n",
                                    print_string(D).c_str(),
                                    print_string(M).c_str(),
                                    print_string(A).c_str(),
                                    print_string(B).c_str());
        }

        //
        // 运算到当前
        //      A = D*B + M;
        // 因此，我们有
        //      sign*Y*x  ==  D*B + M   (mod |m|).
        //

        tmp = A; /* 值无关紧要 */

        // (A, B) := (B, A mod B)
        A = B;
        B = M;
        mympz_dbgprint_fmt_modinv("A, B = B, M.\nA = %s.\nB = %s.\n",
                                  print_string(A).c_str(),
                                  print_string(B).c_str());
        // 因此我们再次得到 0 <= B < A

        //
        // 因此旧的 M 是现在的B并且之前的B是现在的A，
        // 也就是说
        //      sign*Y*x  ==  D*A + B    (mod |m|),
        // 既
        //      sign*Y*x - D*A  ==  B    (mod |m|).
        //
        // 因此
        //      sign*Y*x + D*sign*X*x  ==  B  (mod |m|),
        // 既
        //      sign*(Y + D*X)*x  ==  B  (mod |m|).
        //
        // 因此如果我们设 (X, Y, sign) := (Y + D*X, X, -sign)，我们回到
        //      -sign*X*x  ==  B   (mod |m|),
        //      sign*Y*x  ==  A   (mod |m|).
        //
        // 注：X与Y在所有的情况下都保持非负。
        //

        //
        // 大多数情况下 D 是非常小的, 因此我们能优化 tmp = D*X+Y
        //
        if (is_one(D))
        {
          mympz_dbgprint_modinv("D == 1.\n");
          tmp = add(X, Y);
          mympz_dbgprint_fmt_modinv("tmp = X * Y.\ntmp = %s.\nA = %s.\nB = %s.\n",
                                    print_string(tmp).c_str(),
                                    print_string(A).c_str(),
                                    print_string(B).c_str());
        }
        else
        {
          if (is_word(D, 2))
          {
            mympz_dbgprint_modinv("D == 2.\n");
            tmp = lshift1(X);
            mympz_dbgprint_fmt_modinv("tmp = X << 1.\ntmp = %s.\nX = %s.\n",
                                      print_string(tmp).c_str(),
                                      print_string(X).c_str());
          }
          else if (is_word(D, 4))
          {
            mympz_dbgprint_modinv("D == 4.\n");
            tmp = lshift(X, 2);
            mympz_dbgprint_fmt_modinv("tmp = X << 2.\ntmp = %s.\nX = %s.\n",
                                      print_string(tmp).c_str(),
                                      print_string(X).c_str());
          }
          else if (bn_size(D) == 1)
          {
            tmp = mul(X, D.number[0]);
          }
          else
          {
            tmp = mul(D, X);
          }
          tmp = add(tmp, Y);
          mympz_dbgprint_fmt_modinv("tmp = D * X + Y << 2.\ntmp = %s.\nX = %s.\nY = %s.\n",
                                    print_string(tmp).c_str(),
                                    print_string(X).c_str(),
                                    print_string(Y).c_str());
        }

        M = Y;
        Y = X;
        X = tmp;
        sign = -sign;
        mympz_dbgprint_fmt_modinv("M, Y, X = Y, X, tmp.\nM = %s.\nY = %s.\nX = %s.\n",
                                  print_string(M).c_str(),
                                  print_string(Y).c_str(),
                                  print_string(X).c_str());
        mympz_dbgprint_fmt_modinv("sign = %d.\n", sign);
      }
    }

    //
    // 当循环结束（欧几里得算法）
    //      A == gcd(x,m);
    //
    // 我们有
    //      sign*Y*x  ==  A  (mod |m|),
    // Y是非负的。
    //

    if (sign < 0)
    {
      mympz_dbgprint_modinv("sign is negative.\n");
      Y = sub(m, Y);
      mympz_dbgprint_fmt_modinv("Y = m - Y.\nY = %s.\n",
                                print_string(Y).c_str());
    }
    // 现在 Y*x  ==  A  (mod |m|)

    if (is_one(A))
    {
      mympz_dbgprint_modinv("A == 1.\n");
      // Y*x == 1  (mod |m|)
      if (!Y.neg && ucmp(Y, m) < 0)
      {
        mympz_dbgprint_modinv("Y < m.\n");
        R = Y;
        mympz_dbgprint_fmt_modinv("R = %s.\n", print_string(R).c_str());
      }
      else
      {
        mympz_dbgprint_modinv("Y >= m.\n");
        R = __nnmod(Y, m);
        mympz_dbgprint_fmt_modinv("R = %s.\n", print_string(R).c_str());
      }
    }
    else
    {
      // 没有模逆
      mympz_dbgprint_modinv("no modular inversion.\n");
      set_null(R);
    }
    return R;
  }

#if 0

/**
  * @brief         大数的模幂算法
  * @param[in]     x
  * @param[in]     p 指数
  * @param[in]     m 模数
  * @return        x^p mod m
  * @note
  *       对于任意的模数 m = 2^k * m_odd，可以分成两个步骤进行计算：x^p mod m_odd
  *       与 x^p mod 2^k，对于奇数部分使用蒙哥马利算法进行运算，使用近似指数消减并且
  *       通过CRT进行合并结果。
  * 
  *       当且仅当指数是奇数的时候，使用蒙哥马利算法；其余情况使用指数消减进行运算。
  */
bignum_t mod_exp(const bignum_t& x, const bignum_t& p, const bignum_t& m) {

#define RECP_MUL_MOD

  bignum_t y;
  if (is_odd(m)) {
    if ((bn_size(x) == 1) && (is_positive(x))) {
      unit_t w = x.number[0];
      y = mod_exp_mont_unit(w, p, m);
    } else {
      y = mod_exp_mont(x, p, m);
    }
  } else {
#ifdef RECP_MUL_MOD
    y = mod_exp_recp(x, p, m);
#else
    y = mod_exp_simple(x, p, m);
#endif
  }

  return y;
}

/**
  * @brief         蒙哥马利大数的模幂算法
  * @param[in]     w 一个字
  * @param[in]     p 指数
  * @param[in]     m 模数
  * @return        x^p mod m
  * @note 基数为一个字时的模幂算法。
  */
bignum_t mod_exp_mont_unit(const unit_t& w, const bignum_t& p, const bignum_t& m) {
    BN_MONT_CTX *mont = NULL;
    int b, bits, ret = 0;
    int r_is_one;
    BN_ULONG w, next_w;
    BIGNUM *r, *t;
    BIGNUM *swap_tmp;
#define BN_MOD_MUL_WORD(r, w, m)   \
  (BN_mul_word(r, (w)) &&          \
   (/* BN_ucmp(r, (m)) < 0 ? 1 :*/ \
    (BN_mod(t, r, m, ctx) && (swap_tmp = r, r = t, t = swap_tmp, 1))))
    /*
     * BN_MOD_MUL_WORD is only used with 'w' large, so the BN_ucmp test is
     * probably more overhead than always using BN_mod (which uses BN_copy if
     * a similar test returns true).
     */
    /*
     * We can use BN_mod and do not need BN_nnmod because our accumulator is
     * never negative (the result of BN_mod does not depend on the sign of
     * the modulus).
     */
#define BN_TO_MONTGOMERY_WORD(r, w, mont) \
  (BN_set_word(r, (w)) && BN_to_montgomery(r, r, (mont), ctx))

    if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(m, BN_FLG_CONSTTIME) != 0) {
        /* BN_FLG_CONSTTIME only supported by BN_mod_exp_mont() */
        ERR_raise(ERR_LIB_BN, ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return 0;
    }

    bn_check_top(p);
    bn_check_top(m);

    if (!BN_is_odd(m)) {
        ERR_raise(ERR_LIB_BN, BN_R_CALLED_WITH_EVEN_MODULUS);
        return 0;
    }
    if (m->top == 1)
        a %= m->d[0];           /* make sure that 'a' is reduced */

    bits = BN_num_bits(p);
    if (bits == 0) {
        /* x**0 mod 1, or x**0 mod -1 is still zero. */
        if (BN_abs_is_word(m, 1)) {
            ret = 1;
            BN_zero(rr);
        } else {
            ret = BN_one(rr);
        }
        return ret;
    }
    if (a == 0) {
        BN_zero(rr);
        ret = 1;
        return ret;
    }

    BN_CTX_start(ctx);
    r = BN_CTX_get(ctx);
    t = BN_CTX_get(ctx);
    if (t == NULL)
        goto err;

    if (in_mont != NULL)
        mont = in_mont;
    else {
        if ((mont = BN_MONT_CTX_new()) == NULL)
            goto err;
        if (!BN_MONT_CTX_set(mont, m, ctx))
            goto err;
    }

    r_is_one = 1;               /* except for Montgomery factor */

    /* bits-1 >= 0 */

    /* The result is accumulated in the product r*w. */
    w = a;                      /* bit 'bits-1' of 'p' is always set */
    for (b = bits - 2; b >= 0; b--) {
        /* First, square r*w. */
        next_w = w * w;
        if ((next_w / w) != w) { /* overflow */
            if (r_is_one) {
                if (!BN_TO_MONTGOMERY_WORD(r, w, mont))
                    goto err;
                r_is_one = 0;
            } else {
                if (!BN_MOD_MUL_WORD(r, w, m))
                    goto err;
            }
            next_w = 1;
        }
        w = next_w;
        if (!r_is_one) {
            if (!BN_mod_mul_montgomery(r, r, r, mont, ctx))
                goto err;
        }

        /* Second, multiply r*w by 'a' if exponent bit is set. */
        if (BN_is_bit_set(p, b)) {
            next_w = w * a;
            if ((next_w / a) != w) { /* overflow */
                if (r_is_one) {
                    if (!BN_TO_MONTGOMERY_WORD(r, w, mont))
                        goto err;
                    r_is_one = 0;
                } else {
                    if (!BN_MOD_MUL_WORD(r, w, m))
                        goto err;
                }
                next_w = a;
            }
            w = next_w;
        }
    }

    /* Finally, set r:=r*w. */
    if (w != 1) {
        if (r_is_one) {
            if (!BN_TO_MONTGOMERY_WORD(r, w, mont))
                goto err;
            r_is_one = 0;
        } else {
            if (!BN_MOD_MUL_WORD(r, w, m))
                goto err;
        }
    }

    if (r_is_one) {             /* can happen only if a == 1 */
        if (!BN_one(rr))
            goto err;
    } else {
        if (!BN_from_montgomery(rr, r, mont, ctx))
            goto err;
    }
    ret = 1;
 err:
    if (in_mont == NULL)
        BN_MONT_CTX_free(mont);
    BN_CTX_end(ctx);
    bn_check_top(rr);
    return ret;
}

/* The old fallback, simple version :-) */
int BN_mod_exp_simple(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                      const BIGNUM *m, BN_CTX *ctx)
{
    int i, j, bits, ret = 0, wstart, wend, window, wvalue;
    int start = 1;
    BIGNUM *d;
    /* Table of variables obtained from 'ctx' */
    BIGNUM *val[TABLE_SIZE];

    if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(a, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(m, BN_FLG_CONSTTIME) != 0) {
        /* BN_FLG_CONSTTIME only supported by BN_mod_exp_mont() */
        ERR_raise(ERR_LIB_BN, ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return 0;
    }

    bits = BN_num_bits(p);
    if (bits == 0) {
        /* x**0 mod 1, or x**0 mod -1 is still zero. */
        if (BN_abs_is_word(m, 1)) {
            ret = 1;
            BN_zero(r);
        } else {
            ret = BN_one(r);
        }
        return ret;
    }

    BN_CTX_start(ctx);
    d = BN_CTX_get(ctx);
    val[0] = BN_CTX_get(ctx);
    if (val[0] == NULL)
        goto err;

    if (!BN_nnmod(val[0], a, m, ctx))
        goto err;               /* 1 */
    if (BN_is_zero(val[0])) {
        BN_zero(r);
        ret = 1;
        goto err;
    }

    window = BN_window_bits_for_exponent_size(bits);
    if (window > 1) {
        if (!BN_mod_mul(d, val[0], val[0], m, ctx))
            goto err;           /* 2 */
        j = 1 << (window - 1);
        for (i = 1; i < j; i++) {
            if (((val[i] = BN_CTX_get(ctx)) == NULL) ||
                !BN_mod_mul(val[i], val[i - 1], d, m, ctx))
                goto err;
        }
    }

    start = 1;                  /* This is used to avoid multiplication etc
                                 * when there is only the value '1' in the
                                 * buffer. */
    wvalue = 0;                 /* The 'value' of the window */
    wstart = bits - 1;          /* The top bit of the window */
    wend = 0;                   /* The bottom bit of the window */

    if (!BN_one(r))
        goto err;

    for (;;) {
        if (BN_is_bit_set(p, wstart) == 0) {
            if (!start)
                if (!BN_mod_mul(r, r, r, m, ctx))
                    goto err;
            if (wstart == 0)
                break;
            wstart--;
            continue;
        }
        /*
         * We now have wstart on a 'set' bit, we now need to work out how bit
         * a window to do.  To do this we need to scan forward until the last
         * set bit before the end of the window
         */
        wvalue = 1;
        wend = 0;
        for (i = 1; i < window; i++) {
            if (wstart - i < 0)
                break;
            if (BN_is_bit_set(p, wstart - i)) {
                wvalue <<= (i - wend);
                wvalue |= 1;
                wend = i;
            }
        }

        /* wend is the size of the current window */
        j = wend + 1;
        /* add the 'bytes above' */
        if (!start)
            for (i = 0; i < j; i++) {
                if (!BN_mod_mul(r, r, r, m, ctx))
                    goto err;
            }

        /* wvalue will be an odd number < 2^window */
        if (!BN_mod_mul(r, r, val[wvalue >> 1], m, ctx))
            goto err;

        /* move the 'window' down further */
        wstart -= wend + 1;
        wvalue = 0;
        start = 0;
        if (wstart < 0)
            break;
    }
    ret = 1;
 err:
    BN_CTX_end(ctx);
    bn_check_top(r);
    return ret;
}

int BN_mod_exp_recp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                    const BIGNUM *m, BN_CTX *ctx)
{
    int i, j, bits, ret = 0, wstart, wend, window, wvalue;
    int start = 1;
    BIGNUM *aa;
    /* Table of variables obtained from 'ctx' */
    BIGNUM *val[TABLE_SIZE];
    BN_RECP_CTX recp;

    if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(a, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(m, BN_FLG_CONSTTIME) != 0) {
        /* BN_FLG_CONSTTIME only supported by BN_mod_exp_mont() */
        ERR_raise(ERR_LIB_BN, ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return 0;
    }

    bits = BN_num_bits(p);
    if (bits == 0) {
        /* x**0 mod 1, or x**0 mod -1 is still zero. */
        if (BN_abs_is_word(m, 1)) {
            ret = 1;
            BN_zero(r);
        } else {
            ret = BN_one(r);
        }
        return ret;
    }

    BN_RECP_CTX_init(&recp);

    BN_CTX_start(ctx);
    aa = BN_CTX_get(ctx);
    val[0] = BN_CTX_get(ctx);
    if (val[0] == NULL)
        goto err;

    if (m->neg) {
        /* ignore sign of 'm' */
        if (!BN_copy(aa, m))
            goto err;
        aa->neg = 0;
        if (BN_RECP_CTX_set(&recp, aa, ctx) <= 0)
            goto err;
    } else {
        if (BN_RECP_CTX_set(&recp, m, ctx) <= 0)
            goto err;
    }

    if (!BN_nnmod(val[0], a, m, ctx))
        goto err;               /* 1 */
    if (BN_is_zero(val[0])) {
        BN_zero(r);
        ret = 1;
        goto err;
    }

    window = BN_window_bits_for_exponent_size(bits);
    if (window > 1) {
        if (!BN_mod_mul_reciprocal(aa, val[0], val[0], &recp, ctx))
            goto err;           /* 2 */
        j = 1 << (window - 1);
        for (i = 1; i < j; i++) {
            if (((val[i] = BN_CTX_get(ctx)) == NULL) ||
                !BN_mod_mul_reciprocal(val[i], val[i - 1], aa, &recp, ctx))
                goto err;
        }
    }

    start = 1;                  /* This is used to avoid multiplication etc
                                 * when there is only the value '1' in the
                                 * buffer. */
    wvalue = 0;                 /* The 'value' of the window */
    wstart = bits - 1;          /* The top bit of the window */
    wend = 0;                   /* The bottom bit of the window */

    if (!BN_one(r))
        goto err;

    for (;;) {
        if (BN_is_bit_set(p, wstart) == 0) {
            if (!start)
                if (!BN_mod_mul_reciprocal(r, r, r, &recp, ctx))
                    goto err;
            if (wstart == 0)
                break;
            wstart--;
            continue;
        }
        /*
         * We now have wstart on a 'set' bit, we now need to work out how bit
         * a window to do.  To do this we need to scan forward until the last
         * set bit before the end of the window
         */
        wvalue = 1;
        wend = 0;
        for (i = 1; i < window; i++) {
            if (wstart - i < 0)
                break;
            if (BN_is_bit_set(p, wstart - i)) {
                wvalue <<= (i - wend);
                wvalue |= 1;
                wend = i;
            }
        }

        /* wend is the size of the current window */
        j = wend + 1;
        /* add the 'bytes above' */
        if (!start)
            for (i = 0; i < j; i++) {
                if (!BN_mod_mul_reciprocal(r, r, r, &recp, ctx))
                    goto err;
            }

        /* wvalue will be an odd number < 2^window */
        if (!BN_mod_mul_reciprocal(r, r, val[wvalue >> 1], &recp, ctx))
            goto err;

        /* move the 'window' down further */
        wstart -= wend + 1;
        wvalue = 0;
        start = 0;
        if (wstart < 0)
            break;
    }
    ret = 1;
 err:
    BN_CTX_end(ctx);
    BN_RECP_CTX_free(&recp);
    bn_check_top(r);
    return ret;
}

int BN_mod_exp_mont(BIGNUM *rr, const BIGNUM *a, const BIGNUM *p,
                    const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *in_mont)
{
    int i, j, bits, ret = 0, wstart, wend, window, wvalue;
    int start = 1;
    BIGNUM *d, *r;
    const BIGNUM *aa;
    /* Table of variables obtained from 'ctx' */
    BIGNUM *val[TABLE_SIZE];
    BN_MONT_CTX *mont = NULL;

    if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(a, BN_FLG_CONSTTIME) != 0
            || BN_get_flags(m, BN_FLG_CONSTTIME) != 0) {
        return BN_mod_exp_mont_consttime(rr, a, p, m, ctx, in_mont);
    }

    bn_check_top(a);
    bn_check_top(p);
    bn_check_top(m);

    if (!BN_is_odd(m)) {
        ERR_raise(ERR_LIB_BN, BN_R_CALLED_WITH_EVEN_MODULUS);
        return 0;
    }
    bits = BN_num_bits(p);
    if (bits == 0) {
        /* x**0 mod 1, or x**0 mod -1 is still zero. */
        if (BN_abs_is_word(m, 1)) {
            ret = 1;
            BN_zero(rr);
        } else {
            ret = BN_one(rr);
        }
        return ret;
    }

    BN_CTX_start(ctx);
    d = BN_CTX_get(ctx);
    r = BN_CTX_get(ctx);
    val[0] = BN_CTX_get(ctx);
    if (val[0] == NULL)
        goto err;

    /*
     * If this is not done, things will break in the montgomery part
     */

    if (in_mont != NULL)
        mont = in_mont;
    else {
        if ((mont = BN_MONT_CTX_new()) == NULL)
            goto err;
        if (!BN_MONT_CTX_set(mont, m, ctx))
            goto err;
    }

    if (a->neg || BN_ucmp(a, m) >= 0) {
        if (!BN_nnmod(val[0], a, m, ctx))
            goto err;
        aa = val[0];
    } else
        aa = a;
    if (!bn_to_mont_fixed_top(val[0], aa, mont, ctx))
        goto err;               /* 1 */

    window = BN_window_bits_for_exponent_size(bits);
    if (window > 1) {
        if (!bn_mul_mont_fixed_top(d, val[0], val[0], mont, ctx))
            goto err;           /* 2 */
        j = 1 << (window - 1);
        for (i = 1; i < j; i++) {
            if (((val[i] = BN_CTX_get(ctx)) == NULL) ||
                !bn_mul_mont_fixed_top(val[i], val[i - 1], d, mont, ctx))
                goto err;
        }
    }

    start = 1;                  /* This is used to avoid multiplication etc
                                 * when there is only the value '1' in the
                                 * buffer. */
    wvalue = 0;                 /* The 'value' of the window */
    wstart = bits - 1;          /* The top bit of the window */
    wend = 0;                   /* The bottom bit of the window */

#if 1 /* by Shay Gueron's suggestion */
    j = m->top;                 /* borrow j */
    if (m->d[j - 1] & (((BN_ULONG)1) << (BN_BITS2 - 1))) {
        if (bn_wexpand(r, j) == NULL)
            goto err;
        /* 2^(top*BN_BITS2) - m */
        r->d[0] = (0 - m->d[0]) & BN_MASK2;
        for (i = 1; i < j; i++)
            r->d[i] = (~m->d[i]) & BN_MASK2;
        r->top = j;
        r->flags |= BN_FLG_FIXED_TOP;
    } else
#endif
    if (!bn_to_mont_fixed_top(r, BN_value_one(), mont, ctx))
        goto err;
    for (;;) {
        if (BN_is_bit_set(p, wstart) == 0) {
            if (!start) {
                if (!bn_mul_mont_fixed_top(r, r, r, mont, ctx))
                    goto err;
            }
            if (wstart == 0)
                break;
            wstart--;
            continue;
        }
        /*
         * We now have wstart on a 'set' bit, we now need to work out how bit
         * a window to do.  To do this we need to scan forward until the last
         * set bit before the end of the window
         */
        wvalue = 1;
        wend = 0;
        for (i = 1; i < window; i++) {
            if (wstart - i < 0)
                break;
            if (BN_is_bit_set(p, wstart - i)) {
                wvalue <<= (i - wend);
                wvalue |= 1;
                wend = i;
            }
        }

        /* wend is the size of the current window */
        j = wend + 1;
        /* add the 'bytes above' */
        if (!start)
            for (i = 0; i < j; i++) {
                if (!bn_mul_mont_fixed_top(r, r, r, mont, ctx))
                    goto err;
            }

        /* wvalue will be an odd number < 2^window */
        if (!bn_mul_mont_fixed_top(r, r, val[wvalue >> 1], mont, ctx))
            goto err;

        /* move the 'window' down further */
        wstart -= wend + 1;
        wvalue = 0;
        start = 0;
        if (wstart < 0)
            break;
    }
    /*
     * Done with zero-padded intermediate BIGNUMs. Final BN_from_montgomery
     * removes padding [if any] and makes return value suitable for public
     * API consumer.
     */
#if defined(SPARC_T4_MONT)
    if (OPENSSL_sparcv9cap_P[0] & (SPARCV9_VIS3 | SPARCV9_PREFER_FPU)) {
        j = mont->N.top;        /* borrow j */
        val[0]->d[0] = 1;       /* borrow val[0] */
        for (i = 1; i < j; i++)
            val[0]->d[i] = 0;
        val[0]->top = j;
        if (!BN_mod_mul_montgomery(rr, r, val[0], mont, ctx))
            goto err;
    } else
#endif
    if (!BN_from_montgomery(rr, r, mont, ctx))
        goto err;
    ret = 1;
 err:
    if (in_mont == NULL)
        BN_MONT_CTX_free(mont);
    BN_CTX_end(ctx);
    bn_check_top(rr);
    return ret;
}

#endif

} // namespace mympz