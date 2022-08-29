#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <mympz/mympz.h>

using namespace mympz;

TEST(Mympz, Div) {
  bignum_t x,y;
  division_result_t z;
  std::string d_str, r_str;

  x = create("56789");
  y = create("123");
  z = div(x, y);
  d_str = print_string(z.first);
  r_str = print_string(z.second);
  EXPECT_STREQ(d_str.c_str(), "461");
  EXPECT_STREQ(r_str.c_str(), "86");

  x = create("5678932145424313244313232143542432");
  y = create("12341235465432547623431431");
  z = div(x, y);
  d_str = print_string(z.first);
  r_str = print_string(z.second);
  EXPECT_STREQ(d_str.c_str(), "460159127");
  EXPECT_STREQ(r_str.c_str(), "7549433452529100110221695");

  x = create(
    "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223AAF"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
  ,true);
  // std::cout << "x = " << print_string(x) << std::endl;
  y = create(
    "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223333"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFDDCCBB12345784365482785490910375482749031560427490368427489FFF"
    "19DD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233DD"
  ,true);
  // std::cout << "y = " << print_string(y) << std::endl;
  z = div(y, x);
  d_str = print_string(z.first);
  r_str = print_string(z.second);
  EXPECT_STREQ(d_str.c_str(), "13407807929942597099574024998205846127479365820592393377723561443721764029741143962624141838183490357554335959547134604119870891686790233339828254276256602");
  EXPECT_STREQ(r_str.c_str(), "21175685170448463646996560190595082856718904083028965863566610256317014862544035701382102883055223177540923991989036759128902518240610842297367761195819164670029291466913243438266560186281775403282937234818179930114533667349685864365124402546062495855264012312718565827275423656959267394759987548526661492308171746510134302056336661280062409908824935482968291135959676788812717106973328653246724939611287117341847309753596337140070095920182141572112632656711855675899680768856480893829913079541928423083306730812074004091389808178105949021705558621463693097067193200855000616963470929426491029267724381565516765238511");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}