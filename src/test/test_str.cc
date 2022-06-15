#include <iostream>
#include <ctype.h>
#include <string>
#include <deque>

typedef uint32_t unit_t;
typedef std::deque<unit_t> bignum_t;
static const char* bignum_string = "4294967295";

bignum_t input_bignum(std::string str) {
  bignum_t num;
  if (str.size() < strlen(bignum_string)) {
    unit_t u = std::stoul(str);
    std::cout << u << std::endl;
  } else if (str.size() == strlen(bignum_string)) {
    
  } else if (str.size() > strlen(bignum_string)) {
    
  }
}

int main(int argc, char* argv[]) {
  std::string s;
  std::cin >> s;
  std::cout << s << std::endl;

  for (int i = s.size()-1; i >= 0; i--) {
    char c = s[i];
    std::cout << c;
  }
  std::cout << std::endl;

  return 0;
}