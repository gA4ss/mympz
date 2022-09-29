import random
import os

head_string = """
  #include <stdio.h>
  #include <stdlib.h>
  #include <string>
  #include <iostream>
  #include <gtest/gtest.h>
  #include <mympz/mympz.h>

  using namespace mympz;
"""

main_string = """
int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
"""

def generate_case(unit_name, case_name):
  return "TEST({}, {})".format(unit_name, case_name)

def generate_bignum_variables(variables):
  names = "bignum_t "
  for i, name in enumerate(variables):
    names += name;
    if i != len(variables)-1:
      names += ','
  names += ';'
  return names

def generate_EXPECT_TRUE(variable, value):
  return 'EXPECT_TRUE({} == "{}");'.format(variable, value)

def generate_EXPECT_STREQ(variable, value):
  return 'EXPECT_STREQ({}.c_str(), "{}");'.format(variable, value)

