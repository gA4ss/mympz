#include <iostream>
#include <ctype.h>

#define sqr(r0, r1, a)     \
  asm("mulq %2"            \
      : "=a"(r0), "=d"(r1) \
      : "a"(a)             \
      : "cc");

int main(int argc, char *argv[])
{
  uint64_t *x = new uint64_t[4];
  uint64_t *y = new uint64_t[4];
  x[0] = 0xFFFFFFFFEEEEEEEE;
  x[1] = 0x11111111AAAAAAAA;
  x[2] = 0;
  x[3] = 0;

  y[0] = 0;
  y[1] = 0;
  y[2] = 0;
  y[3] = 0;

  
  std::cout << "z[0] = " << std::hex << z[0] << " "
            << "z[1] = " << z[1] << std::endl;
  std::cout << "c = " << std::hex << c << std::endl;
  return 0;
}