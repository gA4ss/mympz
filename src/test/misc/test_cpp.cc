#include <iostream>
#include <deque>
#include <sstream>

typedef std::deque<int>::iterator dptr_t;

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

void test4_foo(const dptr_t& z, const dptr_t& x, const dptr_t& y) {
  dptr_t& _x = const_cast<dptr_t&>(x);
  dptr_t& _y = const_cast<dptr_t&>(y);
  _x++;
  _y++;
  *z = *_x + *_y;
}

void test4() {
  std::deque<int> x,y,z;
  x.resize(5); y.resize(5); z.resize(5);
  x[0] = 5; x[1] = 3;
  y[0] = 4; y[1] = 6;
  test4_foo(z.begin(), x.begin(), y.begin());
  for (size_t i = 0; i < 2; i++) {
    std::cout << "x[" << i << "] = " << x[i] << std::endl;
    std::cout << "y[" << i << "] = " << y[i] << std::endl;
    std::cout << "z[" << i << "] = " << z[i] << std::endl;
  }
}

void test5() {
  std::deque<int> x,y;
  x.resize(2); y.resize(2);
  x[0] = 5; x[1] = 3;
  y[0] = 4; y[1] = 6;

  std::deque<int>::iterator x_ptr;
  std::deque<int>::iterator y_ptr;

  x_ptr = x.begin();
  y_ptr = y.begin();

  for (size_t i = 0; i < 2; i++) {
    std::cout << "x[" << i << "] = " << x_ptr[i] << std::endl;
    std::cout << "y[" << i << "] = " << y_ptr[i] << std::endl;
  }

}

void test6() {
  std::deque<int> x,y;
  x.resize(5);
  for (int i = 0; i < 5; i++)
    x[i] = i;

  std::deque<int>::iterator x_ptr1, x_ptr2;
  x_ptr1= x.begin() + 3;
  x_ptr2= x.begin() + 3;
  if (x_ptr1 == x_ptr2)
    std::cout << "same" << std::endl;
  else
    std::cout << "not same" << std::endl;
}

void test7() {
  signed short i = 0x8000;
  signed short j = 0;
  std::cout << "i = " << i << std::endl;
  if (i == j)
    std::cout << "yes" << std::endl;
  else
    std::cout << "no"  << std::endl;
}

int main(int argc, char* argv[]) {
  // test1();
  test7();
  return 0;
}