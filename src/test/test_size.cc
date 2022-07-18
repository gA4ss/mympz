#include <iostream>
#include <ctype.h>

int main(int argc, char* argv[]) {
  std::cout << "size = " << sizeof(unsigned int) << std::endl;
  std::cout << "size = " << sizeof(unsigned long) << std::endl;
  std::cout << "size = " << sizeof(unsigned long long) << std::endl;
  std::cout << "8 bytes = " << 0xFF << std::endl;
  std::cout << "16 bytes = " << 0xFFFF << std::endl;
  std::cout << "32 bytes = " << 0xFFFFFFFF << std::endl;
  std::cout << "64 bytes = " << 0xFFFFFFFFFFFFFFFF << std::endl;
  std::cout << std::endl;

  uint16_t a = 0xFF;
  uint32_t b = 0xFFFF;
  uint64_t c = 0xFFFFFFFF;
  std::cout << "0xFF * 0xFF = " << a * a << " " << std::hex << a * a << std::endl;
  std::cout << "0xFFFF * 0xFFFF = " << b * b << " " << std::hex << b * b << std::endl;
  std::cout << "0xFFFFFFFF * 0xFFFFFFFF = " << c * c << " " << std::hex << c * c << std::endl;
  std::cout << std::endl;

  std::cout << !(0x81 & 0x80) << std::endl;
  std::cout << !!(0x81 & 0x80) << std::endl;
  std::cout << !(0x79 & 0x80) << std::endl;
  std::cout << !!(0x79 & 0x80) << std::endl;
  return 0;
}