#include <iostream>
#include <ctype.h>

uint64_t q,rem,n1,n0,d0;

#if defined(__GNUC__) && __GNUC__ >= 2
#if defined(__i386) || defined(__i386__)
/*-
 * There were two reasons for implementing this template:
 * - GNU C generates a call to a function (__udivdi3 to be exact)
 *   in reply to ((((BN_ULLONG)n0)<<BN_BITS2)|n1)/d0 (I fail to
 *   understand why...);
 * - divl doesn't only calculate quotient, but also leaves
 *   remainder in %edx which we can definitely use here:-)
 */
#define __div_unit(n0, n1, d0)      \
  ({                                \
    asm volatile(                   \
        "divl   %4"                 \
        : "=a"(q), "=d"(rem)        \
        : "a"(n1), "d"(n0), "r"(d0) \
        : "cc");                    \
    q;                              \
  })
#elif defined(__x86_64)
/*
 * Same story here, but it's 128-bit by 64-bit division. Wow!
 */
#define __div_unit(n0, n1, d0)      \
  ({                                \
    asm volatile(                   \
        "divq   %4"                 \
        : "=a"(q), "=d"(rem)        \
        : "a"(n1), "d"(n0), "r"(d0) \
        : "cc");                    \
    q;                              \
  })
#endif
#endif

int main(int argc, char *argv[])
{
  uint64_t xl = 0xFFEEDDCC11223344, xh = 0x55667788AABBCCDD;
  uint64_t y = 0xEEFFCCDDBBAA1122;
  uint64_t q = __div_unit(xh, xl, y);
  std::cout << "q = " << std::hex << q << std::endl;
  return 0;
}