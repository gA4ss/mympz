#include <iostream>
#include <deque>
#include <sstream>

void test1() {
  std::deque<int> l;
  l.resize(10);
  for (size_t i = 0; i < 10; i++) {
    l[i] = i;
    std::cout << l[i] << " " << std::endl;
  }

  std::cout << std::endl;

  l.resize(3);

  for (size_t i = 0; i < l.size(); i++) {
    std::cout << l[i] << " " << std::endl;
  }
}

void test2() {
  std::ostringstream ostr;
  std::string s;

  ostr << 0xFFFFFFFFFFFFFFFF;
  s = ostr.str();
  std::cout << s << std::endl;

  ostr.str("");
  ostr << 0xFFFFFFFF;
  s = ostr.str();
  std::cout << s << std::endl;
}

int main(int argc, char* argv[]) {
  // test1();
  test2();
  return 0;
}