#include <iostream>
#include <ctype.h>
#include <mympz/mympz.h>

using namespace mympz;

int main(int argc, char* argv[]) {
  bignum_t x;
  x.number.push_back(123);
  x.number.push_back(456);
  x.number.push_back(0);
  x.number.push_back(0);
  x.number.push_back(0);
  
  clear_head_zero(x);

  std::cout << "x size = " << bn_size(x) << std::endl;
  for (size_t i = 0; i < x.number.size(); i++) {
    std::cout << x.number[i] << std::endl;
  }

  std::cout << std::endl << std::endl;

  x.number.clear();
  x.number.push_back(123);
  x.number.push_back(456);
  
  clear_head_zero(x);

  std::cout << "x size = " << bn_size(x) << std::endl;
  for (size_t i = 0; i < x.number.size(); i++) {
    std::cout << x.number[i] << std::endl;
  }
  return 0;
}