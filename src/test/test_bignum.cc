#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <mynum/bignum.h>

using namespace mynum;

TEST(BigNum, Create) {
  bignum_t a = string_to_bignum("123456789");
  std::string a_str = bignum_to_string(a);
  EXPECT_TRUE(a_str == "123456789") << "a = " << a_str;

  a = string_to_bignum("4312330957643718845789193755402187590185748902385429394");
  a_str = bignum_to_string(a);
  EXPECT_TRUE(a_str == "4312330957643718845789193755402187590185748902385429394") << "a = " << a_str;
}

TEST(BigNum, Mul) {
  bignum_t a = string_to_bignum("1");
  bignum_t b = string_to_bignum("1234543");
  bignum_t c = string_to_bignum("0");
  c = mul(a,b);
  shrink_zero(c, true);
  EXPECT_TRUE(bignum_to_string(c) == "1234543") << "c = " << bignum_to_string(c);

  a = string_to_bignum("12451246785432787548296578491");
  b = string_to_bignum("43246397325423541798654243965460321");
  c = string_to_bignum("0");
  c = mul(a,b);
  shrink_zero(c, true);
  EXPECT_TRUE(bignum_to_string(c) == "538471565679728975854936536478019102804443655522904986122555611")
              << "c = " << bignum_to_string(c);
}

TEST(BigNum, Add) {
  bignum_t a = string_to_bignum("1");
  bignum_t b = string_to_bignum("1234543");
  bignum_t c = add(a,b);
  EXPECT_TRUE(bignum_to_string(c) == "1234544") 
              << "c = " << bignum_to_string(c);

  a = string_to_bignum("12451246785432787548296578491");
  b = string_to_bignum("43246397325423541798654243965460321");
  c = add(a,b);
  EXPECT_TRUE(bignum_to_string(c) == "43246409776670327231441792262038812") 
              << "c = " << bignum_to_string(c);

  a = string_to_bignum("5428748139873654826358472954739825674826584264783156486478316483");
  b = string_to_bignum("4314315432543265743865472864371453716483657482748731483974983217");
  c = add(a,b);
  EXPECT_TRUE(bignum_to_string(c) == "9743063572416920570223945819111279391310241747531887970453299700") 
              << "c = " << bignum_to_string(c);
}

TEST(BigNum, Sub) {
  bignum_t a = string_to_bignum("1");
  bignum_t b = string_to_bignum("1234543");
  bignum_t c = sub(a,b);
  EXPECT_TRUE(bignum_to_string(c) == "1234542") 
              << "c = " << bignum_to_string(c);

  a = string_to_bignum("12451246785432787548296578491");
  b = string_to_bignum("43246397325423541798654243965460321");
  c = sub(a,b);
  EXPECT_TRUE(bignum_to_string(c) == "43246384874176756365866695668881830") 
              << "c = " << bignum_to_string(c);

  a = string_to_bignum("43246397325423541798654243965460321");
  b = string_to_bignum("12451246785432787548296578491");
  c = sub(a,b);
  EXPECT_TRUE(bignum_to_string(c) == "43246384874176756365866695668881830") 
              << "c = " << bignum_to_string(c);
}

TEST(BigNum, Div) {
  bignum_t a = string_to_bignum("1");
  bignum_t b = string_to_bignum("1234543");
  division_result_t res = div(a,b);
  EXPECT_TRUE(bignum_to_string(res.first) == "0") 
              << "q = " << bignum_to_string(res.first);
  EXPECT_TRUE(bignum_to_string(res.second) == "1") 
              << "r = " << bignum_to_string(res.second);

  a = string_to_bignum("12451246785432787548296578491");
  b = string_to_bignum("43246397325423541798654243965460321");
  res = div(a,b);
  shrink_zero(res.first, true);shrink_zero(res.second, true);
  EXPECT_TRUE(bignum_to_string(res.first) == "0") 
              << "q = " << bignum_to_string(res.first);
  EXPECT_TRUE(bignum_to_string(res.second) == "12451246785432787548296578491") 
              << "r = " << bignum_to_string(res.second);

  a = string_to_bignum("43246397325423541798654243965460321");
  b = string_to_bignum("12451246785432787548296578491");
  res = div(a,b);
  shrink_zero(res.first, true);shrink_zero(res.second, true);
  EXPECT_TRUE(bignum_to_string(res.first) == "3473258") 
              << "q = " << bignum_to_string(res.first);
  EXPECT_TRUE(bignum_to_string(res.second) == "4817944828984232766348966643") 
              << "r = " << bignum_to_string(res.second);


  a = string_to_bignum("1");
  b = string_to_bignum("3");
  res = div2(a,b);
  shrink_zero(res.first, true);shrink_zero(res.second, true);
  EXPECT_TRUE(bignum_to_string(res.first) == "0") 
              << "q = " << bignum_to_string(res.first);
  EXPECT_TRUE(bignum_to_string(res.second) == "3333333333333333")
              << "r = " << bignum_to_string(res.second);

  a = string_to_bignum("10");
  b = string_to_bignum("20");
  res = div2(a,b);
  shrink_zero(res.first, true);shrink_zero(res.second, true);
  EXPECT_TRUE(bignum_to_string(res.first) == "0") 
              << "q = " << bignum_to_string(res.first);
  EXPECT_TRUE(bignum_to_string(res.second) == "5")
              << "r = " << bignum_to_string(res.second);

  a = string_to_bignum("43246397325423541798654243965460321");
  b = string_to_bignum("12451246785432787548296578491");
  res = div2(a,b,10);
  shrink_zero(res.first, true);shrink_zero(res.second, true);
  EXPECT_TRUE(bignum_to_string(res.first) == "3473258")
              << "q = " << bignum_to_string(res.first);
  EXPECT_TRUE(bignum_to_string(res.second) == "38694477043")
              << "r = " << bignum_to_string(res.second);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}