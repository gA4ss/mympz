#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <mympz/mympz.h>

using namespace mympz;

TEST(Mympz, Shift) {
  bignum_t x,y;
  std::string x_str, y_str;

  x = create("FFFFFFFF00000000", true);
  y = lshift1(x);
  // std::cout << "y = " << print_string(y, true) << std::endl;
  y_str = print_string(y);
  EXPECT_TRUE(y_str == "36893488138829168640");

  x = create("FFFFFFFF00000000", true);
  y = lshift(x, 63);
  // std::cout << "y = " << print_string(y, true) << std::endl;
  y_str = print_string(y);
  EXPECT_TRUE(y_str == "170141183420855150474555134919112130560");

  x = create("FFFFFFFF00000000", true);
  y = rshift1(x);
  // std::cout << "y = " << print_string(y, true) << std::endl;
  y_str = print_string(y);
  EXPECT_TRUE(y_str == "9223372034707292160") << "y = " << y_str;

  x = create("FFFFFFFF00000000", true);
  y = rshift(x, 128);
  // std::cout << "y = " << print_string(y, true) << std::endl;
  y_str = print_string(y);
  EXPECT_TRUE(y_str == "0");
}

TEST(Mympz, Shift1) {
  bignum_t x,y;
  std::string x_str, y_str;

  x = create("FFFFFFFF00000000FFFFFFFFBBBBCCCC", true);
  y = lshift1(x);
  // std::cout << "y = " << print_string(y, true) << std::endl;
  y_str = print_string(y);
  EXPECT_TRUE(y_str == "680564733683420601935114027821576984984");

  y = lshift(x, 125);
  // std::cout << "y = " << print_string(y, true) << std::endl;
  y_str = print_string(y);
  EXPECT_TRUE(y_str == "14474011151294531095337180868627230155821919580691028592821526248747165548544");

  y = rshift1(x);
  // std::cout << "y = " << print_string(y, true) << std::endl;
  y_str = print_string(y);
  EXPECT_TRUE(y_str == "170141183420855150483778506955394246246") << "y = " << y_str;

  y = rshift(x, 57);
  // std::cout << "y = " << print_string(y, true) << std::endl;
  y_str = print_string(y);
  EXPECT_TRUE(y_str == "2361183240885066793087");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}