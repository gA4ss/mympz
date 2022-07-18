#include <iostream>
#include <deque>

int main(int argc, char* argv[]) {
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
  return 0;
}