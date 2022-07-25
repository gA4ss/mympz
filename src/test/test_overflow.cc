#include <iostream>
#include <ctype.h>
#include <mympz/mympz.h>

using namespace mympz;

int main(int argc, char* argv[]) {
  unsigned int x = 0xFFFF1234;
  unsigned int y = 0xFFFF;
  unsigned int z = x + y;
  std::cout << "z = 0x" << std::hex << z << std::endl;
  std::cout << std::endl;

  return 0;
}