#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <mympz/mympz.h>

using namespace mympz;

TEST(Mympz, Add) {
  bignum_t x = create("123");
  bignum_t y = create("456");

  bignum_t z = add(x, y);
  std::string z_str = print_string(z);
  EXPECT_STREQ(z_str.c_str(), "579") << "z = " << z_str;

  x = create(123578);
  y = create(456);

  z = add(x, y);
  z_str = print_string(z, true);
  std::cout << z_str << std::endl;
  // EXPECT_TRUE(z_str == "124034") << "z = " << z_str;
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}