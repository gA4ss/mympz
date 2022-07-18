#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <mympz/mympz.h>

using namespace mympz;

TEST(Mympz, Create) {
  bignum_t x = create("123456789");
  std::string x_str = print_string(x);
  EXPECT_STREQ(x_str.c_str(), "123456789") << "x = " << x_str;

  x = create("-98125467284318675842");
  x_str = print_string(x);
  EXPECT_STREQ(x_str.c_str(), "-98125467284318675842") << "x = " << x_str;
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}