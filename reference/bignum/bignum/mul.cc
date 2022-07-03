#include <mympz/bignum.h>

namespace mympz {

/* 输入 : B进制n位多精度整数a,b; n是2的次幂。
 * 输出 : 乘积 c = a * b
 * if n == 0 then return a * b
 * else
 *    c = sgn(a) * sgn(b)
 *    a1 = |a|的前n/2位数值；
 *    a2 = |a|的后n/2位数值；
 *    b1 = |b|的前n/2位数值；
 *    b2 = |b|的后n/2位数值；
 *    m1 = Karatsuba(a1, b1, n/2);
 *    m2 = Karatsuba(a2, b2, n/2);
 *    m3 = Karatsuba(a2-a1, b2-b2, n/2);
 *    c = c * (m2 * B^n + (m1+m2+m3)B^{n/2}+m1);
 *    return c
 * endif
 */
// static bignum_t karatsuba_mul(const bignum_t& a, const bignum_t& b) {
//   bignum_t c;
//   return c; 
// }

// FIXME: 使用 Karatsuba 算法改写增加速度。
bignum_t mul(const bignum_t& a, const bignum_t& b) {
  if (a.empty() || b.empty()) {
    mympz_exception(
      "%s", "operand a or b is nan."
    );
  }

  bignum_t x, y, z;
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
  return z;
}

} // namespace mympz