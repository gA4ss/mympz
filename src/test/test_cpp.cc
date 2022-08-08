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

void test3() {
  std::deque<int> l;
  l.resize(10);
  for (size_t i = 0; i < 10; i++) {
    l[i] = i;
    // std::cout << l[i] << " " << std::endl;
  }
  std::deque<int>::iterator it = l.begin();

  for (size_t i = 0; i < l.size(); i++) {
    std::cout << it[i] << " " << std::endl;
  }

  std::cout << std::endl;

  it[5] = 9;
  it[1] = 2;
  it[7] = 3;

  for (size_t i = 0; i < l.size(); i++) {
    std::cout << it[i] << " " << std::endl;
  }
}

int main(int argc, char* argv[]) {
  // test1();
  test3();
  return 0;
}