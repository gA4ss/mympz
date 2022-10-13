#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <mympz/mympz.h>

using namespace mympz;

TEST(Mympz, Gcd) {
  bignum_t x,y,r;
  std::string r_str;

  x = create("5");
  // std::cout << "x = " << print_string(x) << std::endl;
  y = create("15");
  // std::cout << "y = " << print_string(y) << std::endl;
  r = gcd(x, y);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "5");

  x = create(
    "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223333"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
  ,true);
  // std::cout << "x = " << print_string(x) << std::endl;
  y = create(
    "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223333"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
  ,true);
  // std::cout << "y = " << print_string(y) << std::endl;
  r = gcd(x, y);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "1");

  x = create("1243544315424321542431244243124312");
  // std::cout << "x = " << print_string(x) << std::endl;
  y = create("2");
  // std::cout << "y = " << print_string(y) << std::endl;
  r = gcd(x, y);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "2");

  x = create("121");
  // std::cout << "x = " << print_string(x) << std::endl;
  y = create("11");
  // std::cout << "y = " << print_string(y) << std::endl;
  r = gcd(x, y);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "11");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}