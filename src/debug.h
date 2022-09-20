#ifndef MYMPZ_DEBUG_H_
#define MYMPZ_DEBUG_H_

#include <mympz/mympz.h>

namespace mympz
{

  //
  // 只有kDbgInfo是用于通用的打印信息，其余的都在各个主要运算中进行调用。
  //
  enum
  {
    kDbgInfo,
    kDbgCreate,
    kDbgAddSub,
    kDbgMul,
    kDbgSqr,
    kDbgExp,
    kDbgDiv,
    kDbgMod,
    kDbgModMul,
    kDbgModExp,
    kDbgModInv,
    kDbgGCD,
    kDbgShift,
    kDbgRandom,
    kDbgPrime,
    kDbgRSA,
    KDbgWhatMax
  };

  void dbgprint(int what, const char *filepath, int lineno, const char *funcname,
                const char *fmt, ...);

#ifdef DEBUG
#define mympz_dbgprint(w, s) dbgprint((w), "%s", (s))
#define mympz_dbgprint_fmt(w, format, ...) dbgprint((w), format, __VA_ARGS__)

#define mympz_dbgprint_info(s) dbgprint(kDbgInfo, "%s", (s))
#define mympz_dbgprint_fmt_info(format, ...) dbgprint(kDbgInfo, format, __VA_ARGS__)

#define mympz_dbgprint_create(s) dbgprint(kDbgCreate, "%s", (s))
#define mympz_dbgprint_fmt_create(format, ...) dbgprint(kDbgCreate, format, __VA_ARGS__)

#define mympz_dbgprint_addsub(s) dbgprint(kDbgAddSub, "%s", (s))
#define mympz_dbgprint_fmt_addsub(format, ...) dbgprint(kDbgAddSub, format, __VA_ARGS__)

#define mympz_dbgprint_mul(s) dbgprint(kDbgMul, "%s", (s))
#define mympz_dbgprint_fmt_mul(format, ...) dbgprint(kDbgMul, format, __VA_ARGS__)

#define mympz_dbgprint_sqr(s) dbgprint(kDbgSqr, "%s", (s))
#define mympz_dbgprint_fmt_sqr(format, ...) dbgprint(kDbgSqr, format, __VA_ARGS__)

#define mympz_dbgprint_exp(s) dbgprint(kDbgExp, "%s", (s))
#define mympz_dbgprint_fmt_exp(format, ...) dbgprint(kDbgExp, format, __VA_ARGS__)

#define mympz_dbgprint_div(s) dbgprint(kDbgDiv, "%s", (s))
#define mympz_dbgprint_fmt_div(format, ...) dbgprint(kDbgDiv, format, __VA_ARGS__)

#define mympz_dbgprint_mod(s) dbgprint(kDbgMod, "%s", (s))
#define mympz_dbgprint_fmt_mod(format, ...) dbgprint(kDbgMod, format, __VA_ARGS__)

#define mympz_dbgprint_modmul(s) dbgprint(kDbgModMul, "%s", (s))
#define mympz_dbgprint_fmt_modmul(format, ...) dbgprint(kDbgModMul, format, __VA_ARGS__)

#define mympz_dbgprint_modexp(s) dbgprint(kDbgMulExp, "%s", (s))
#define mympz_dbgprint_fmt_modexp(format, ...) dbgprint(kDbgMulExp, format, __VA_ARGS__)

#define mympz_dbgprint_modinv(s) dbgprint(kDbgModInv, "%s", (s))
#define mympz_dbgprint_fmt_modinv(format, ...) dbgprint(kDbgModInv, format, __VA_ARGS__)

#define mympz_dbgprint_gcd(s) dbgprint(kDbgGCD, "%s", (s))
#define mympz_dbgprint_fmt_gcd(format, ...) dbgprint(kDbgGCD, format, __VA_ARGS__)

#define mympz_dbgprint_shift(s) dbgprint(kDbgShift, "%s", (s))
#define mympz_dbgprint_fmt_shift(format, ...) dbgprint(kDbgShift, format, __VA_ARGS__)

#define mympz_dbgprint_random(s) dbgprint(kDbgRandom, "%s", (s))
#define mympz_dbgprint_fmt_random(format, ...) dbgprint(kDbgRandom, format, __VA_ARGS__)

#define mympz_dbgprint_prime(s) dbgprint(kDbgPrime, "%s", (s))
#define mympz_dbgprint_fmt_prime(format, ...) dbgprint(kDbgPrime, format, __VA_ARGS__)

#define mympz_dbgprint_rsa(s) dbgprint(kDbgRSA, "%s", (s))
#define mympz_dbgprint_fmt_rsa(format, ...) dbgprint(kDbgRSA, format, __VA_ARGS__)

#else

#define mympz_dbgprint(w, s)
#define mympz_dbgprint_fmt(w, format, ...)

#define mympz_dbgprint_info(s)
#define mympz_dbgprint_fmt_info(format, ...)

#define mympz_dbgprint_create(s)
#define mympz_dbgprint_fmt_create(format, ...)

#define mympz_dbgprint_addsub(s)
#define mympz_dbgprint_fmt_addsub(format, ...)

#define mympz_dbgprint_mul(s)
#define mympz_dbgprint_fmt_mul(format, ...)

#define mympz_dbgprint_sqr(s)
#define mympz_dbgprint_fmt_sqr(format, ...)

#define mympz_dbgprint_exp(s)
#define mympz_dbgprint_fmt_exp(format, ...)

#define mympz_dbgprint_div(s)
#define mympz_dbgprint_fmt_div(format, ...)

#define mympz_dbgprint_mod(s)
#define mympz_dbgprint_fmt_mod(format, ...)

#define mympz_dbgprint_modmul(s)
#define mympz_dbgprint_fmt_modmul(format, ...)

#define mympz_dbgprint_modexp(s)
#define mympz_dbgprint_fmt_modexp(format, ...)

#define mympz_dbgprint_modinv(s)
#define mympz_dbgprint_fmt_modinv(format, ...)

#define mympz_dbgprint_gcd(s)
#define mympz_dbgprint_fmt_gcd(format, ...)

#define mympz_dbgprint_shift(s)
#define mympz_dbgprint_fmt_shift(format, ...)

#define mympz_dbgprint_random(s)
#define mympz_dbgprint_fmt_random(format, ...)

#define mympz_dbgprint_prime(s)
#define mympz_dbgprint_fmt_prime(format, ...)

#define mympz_dbgprint_rsa(s)
#define mympz_dbgprint_fmt_rsa(format, ...)

#endif

} // namespace mympz

#endif // MYMPZ_DEBUG_H_