#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <mympz/mympz.h>

using namespace mympz;

// TEST(Mympz, UnPass) {
//   bignum_t x,y,r;
//   std::string r_str;

//   x = create(
//     "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223333"
//     "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
//     "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
//     "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
//   ,true);
//   std::cout << "x = " << print_string(x) << std::endl;
//   y = create(
//     "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223333"
//     "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
//     "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
//   ,true);
//   std::cout << "y = " << print_string(y) << std::endl;
//   r = gcd(x, y);
//   r_str = print_string(r);
//   EXPECT_STREQ(r_str.c_str(), "1");
// }

TEST(Mympz, Gcd) {
  bignum_t x,y,r;
  std::string r_str;

  x = create("15");
  // std::cout << "x = " << print_string(x) << std::endl;
  y = create("5");
  // std::cout << "y = " << print_string(y) << std::endl;
  r = gcd(x, y);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "5");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}