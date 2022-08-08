#include <iostream>
#include <ctype.h>

#define __mul_unit(r, a, word, carry) \
  do                                  \
  {                                   \
    register uint64_t high, low;      \
    asm("mulq %3"                     \
        : "=a"(low), "=d"(high)       \
        : "a"(word), "g"(a)           \
        : "cc");                      \
    asm("addq %2,%0; adcq %3,%1"      \
        : "+r"(carry), "+d"(high)     \
        : "a"(low), "g"(0)            \
        : "cc");                      \
    (r) = carry, carry = high;        \
  } while (0)

int main(int argc, char *argv[]) {
  uint64_t a = 0xFFEEDDCC11223344, b = 0x55667788AABBCCDD;
  uint64_t c = 0, r = 0;
  __mul_unit(r, a, b, c);
  std::cout << "r = " << std::hex << r << " "
            << "c = " << c << std::endl;
  return 0;
}