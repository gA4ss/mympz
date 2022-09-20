#ifndef MYMPZ_COMPILE_H_
#define MYMPZ_COMPILE_H_

namespace mympz
{

#define DEBUG
// #define ARCH_32BITS
#define ARCH_64BITS

  //
  // 架构相关
  //
#ifdef ARCH_64BITS
#undef ARCH_32BITS
#endif

  //
  // 调试信息
  //
#ifdef DEBUG

#define DEBUG_INFO
#define DEBUG_CREATE
#define DEBUG_ADDSUB
#define DEBUG_MUL
#define DEBUG_SQR
#define DEBUG_EXP
#define DEBUG_DIV
#define DEBUG_MOD
#define DEBUG_MODMUL
#define DEBUG_MODEXP
#define DEBUG_MODINV
#define DEBUG_GCD
#define DEBUG_SHIFT
#define DEBUG_RANDOM
#define DEBUG_PRIME
#define DEBUG_RSA

#endif

} // namespace mympz

#endif