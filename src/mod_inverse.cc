#include <mympz/mympz.h>
#include "__internal.h"

namespace mympz
{

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
      B = nnmod(B, A);
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
        R = nnmod(Y, m);
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

} // namespace mympz