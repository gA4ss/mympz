#include <mympz/debug.h>
#include "__internal.h"

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>

namespace mympz {

static uint64_t __enable_dbg_what() {
  uint64_t r = 0;
#ifdef DEBUG_INFO
  r |= (1 << kDbgInfo);
#endif

#ifdef DEBUG_CREATE
  r |= (1 << kDbgCreate);
#endif

#ifdef DEBUG_ADDSUB
  r |= (1 << kDbgAddSub);
#endif

#ifdef DEBUG_MUL
  r |= (1 << kDbgMul);
#endif

#ifdef DEBUG_SQR
  r |= (1 << kDbgSqr);
#endif

#ifdef DEBUG_EXP
  r |= (1 << kDbgExp);
#endif

#ifdef DEBUG_DIV
  r |= (1 << kDbgDiv);
#endif

#ifdef DEBUG_MOD
  r |= (1 << kDbgMod);
#endif

#ifdef DEBUG_MODMUL
  r |= (1 << kDbgModMul);
#endif

#ifdef DEBUG_MODEXP
  r |= (1 << kDbgModExp);
#endif

#ifdef DEBUG_MODINV
  r |= (1 << kDbgModInv);
#endif

#ifdef DEBUG_GCD
  r |= (1 << kDbgGCD);
#endif

#ifdef DEBUG_SHIFT
  r |= (1 << kDbgShift);
#endif

#ifdef DEBUG_RANDOM
  r |= (1 << kDbgRandom);
#endif

#ifdef DEBUG_PRIME
  r |= (1 << kDbgPrime);
#endif

#ifdef DEBUG_RSA
  r |= (1 << kDbgRSA);
#endif
  return r;
}

static const std::string __dbg_title[KDbgWhatMax] = {
  "info",
  "create",
  "addsub",
  "mul",
  "sqr",
  "exp",
  "div",
  "mod",
  "modmul",
  "modexp",
  "modinv",
  "gcd",
  "shift",
  "random",
  "prime",
  "rsa"
};

static const uint64_t __dbg_what = __enable_dbg_what();
void dbgprint(int what, const char* filepath, int lineno, const char* funcname, 
              const char* fmt, ...) {
  va_list args;
  char buffer[2048] = {0};

  //
  // 测试是否需要打印调试信息
  //
  if (!(__dbg_what & (1 << what)))
    return;

  //
  // FIXME : 没校验缓存长度,可能存在溢出
  //
  va_start(args, fmt);
  vsprintf(buffer, fmt, args);
  va_end(args);

  std::string title = "[" + __dbg_title[what] + "]";
  if (filepath) {
    std::string filename = filepath;
    filename = filename.substr(filename.find_last_of('/')+1);
    title += filename;
    title += ":";
    title += std::to_string(lineno);
    title += ":";
  }
  if (funcname) {
    title += funcname;
    title += ": ";
  }
  title += buffer;

  printf("%s", title.c_str());
}

} // namespace mympz