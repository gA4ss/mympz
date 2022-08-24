#include <iostream>
#include <ctype.h>
#include <mympz/mympz.h>

using namespace mympz;

int main(int argc, char* argv[]) {
  unsigned int y = 1, z = 0;
  unsigned int x = 0xFFFFFFFF;
  unsigned long zz = 0;
  for (int i = 0; i < 10; i++) {
    z = x + y;
    zz = static_cast<unsigned long>(x) + static_cast<unsigned long>(y);
    std::cout << "z = 0x" << std::hex << z << " " << "zz = 0x" << zz << std::endl;
    y++;
  }
  return 0;
}