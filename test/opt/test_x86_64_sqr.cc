#include <iostream>
#include <ctype.h>

#define sqr(r0, r1, a)     \
  asm("mulq %2"            \
      : "=a"(r0), "=d"(r1) \
      : "a"(a)             \
      : "cc");

int main(int argc, char *argv[])
{
  uint64_t x = 0xFFAABBCC11223344;
  uint64_t y = 0x11223344ABCDEFBC;
  uint64_t z = 0;

  sqr(x, z, y);

  // 0x111c7e591695cc2688e5288cfc6e21f0
  std::cout << std::hex
            << "x = " << x << " "
            << "y = " << y << " "
            << "z = " << z << std::endl;
  return 0;
}