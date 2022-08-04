#include <iostream>
#include <ctype.h>

uint64_t __add_units(uint64_t *rp, const uint64_t *ap, const uint64_t *bp, int n) {
  uint64_t ret;
  size_t i = 0;

  if (n <= 0)
    return 0;

  asm volatile ("       subq    %0,%0           \n" /* clear carry */
                "       jmp     1f              \n"
                ".p2align 4                     \n"
                "1:     movq    (%4,%2,8),%0    \n"
                "       adcq    (%5,%2,8),%0    \n"
                "       movq    %0,(%3,%2,8)    \n"
                "       lea     1(%2),%2        \n"
                "       dec     %1              \n"
                "       jnz     1b              \n"
                "       sbbq    %0,%0           \n"
                :"=&r" (ret), "+c"(n), "+r"(i)
                :"r"(rp), "r"(ap), "r"(bp)
                :"cc", "memory");

  return ret & 1;
}

int main(int argc, char* argv[]) {
  uint64_t* x = new uint64_t [2];
  uint64_t* y = new uint64_t [2];
  uint64_t* z = new uint64_t [2];
  x[0] = 0xFFFFFFFFFFFFFFFF; x[1] = 0;
  y[0] = 3; y[1] = 0;
  z[0] = 0, z[1] = 0;

  uint64_t c = __add_units(z,x,y,2);
  std::cout << "z[0] = " << std::hex << z[0] << " " << "z[1] = " << z[1] << std::endl;
  std::cout << "c = " << std::hex << c << std::endl;
  return 0;
}