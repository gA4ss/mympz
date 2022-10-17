#ifndef MYMPZ_COMPILE_H_
#define MYMPZ_COMPILE_H_

namespace mympz
{

  // #define DEBUG
  // #define DISABLE_OPTIMIZE
  // #define ARCH_64BITS

  //
  // 架构相关
  // 默认64位架构
  //
#define ARCH_64BITS
#if defined(__x86_64) || defined(__x86_64__)
#define CPU_X86_64
#elif (defined(_M_AMD64) || defined(_M_X64))
#define CPU_AMD64
#elif defined(__alpha)
#define CPU_ALPHA
#elif defined(_ARCH_PPC64)
#define CPU_PPC64
#elif defined(__mips)
#define CPU_MIPS
#elif defined(__aarch64__)
#define CPU_AARCH64
#else
#if defined(__i386) || defined(__i386__)
#define CPU_X86
#undef ARCH_64BITS
#endif
#endif

//
// 判断架构位数
//
#if defined(ARCH_64BITS)
#undef ARCH_32BITS
#else
#define ARCH_32BITS
#endif

//
// 编译器相关
//
#if defined(__GNUC__) && __GNUC__ >= 2
#define COMPILE_GNU
#elif defined(_MSC_VER) && _MSC_VER >= 1400
#define COMPILE_MS
#else
// 输出警告
#endif

//
// 优化相关
// gcc -E -dM - </dev/null | grep "STDC_VERSION"
// 优先启用汇编优化，在没有汇编优化前提下开启128位辅助优化
// 如果没有128位运算支持则无优化。
//

// 目前其他体系还不支持汇编优化
#if !defined(CPU_X86_64)
#define DISABLE_OPTIMIZE
#endif

#if !defined(DISABLE_OPTIMIZE)

#define CALC_ASM_OPTIMIZE
#if !defined(COMPILE_GNU)
#undef CALC_ASM_OPTIMIZE
#endif

#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16
#define CALC_INT128_OPTIMIZE
#endif

// 开启汇编优化正确前提下则关闭128运算优化
#if defined(CALC_ASM_OPTIMIZE)
#undef CALC_INT128_OPTIMIZE
#endif

#endif

//
// 除法计算的商与余数是上取整还是下取整
// 默认是上去整
//
// #define DIVISION_QUOTIENT_FLOOR

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
// #define DEBUG_DIV
#define DEBUG_MOD
#define DEBUG_MODMUL
#define DEBUG_MODEXP
// #define DEBUG_MODINV
// #define DEBUG_GCD
#define DEBUG_SHIFT
#define DEBUG_RANDOM
#define DEBUG_PRIME
#define DEBUG_RSA

#endif

} // namespace mympz

#endif