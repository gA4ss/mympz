#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <mympz/mympz.h>

using namespace mympz;

TEST(Mympz, ModInv) {
  bignum_t x,m,xi;
  std::string xi_str;

  x = create(
    "CCDD1122"
  ,true);
  // std::cout << "x = " << print_string(x) << std::endl;
  m = create(
    "E1234567FFFFFFF1"
    // "E1234567FFFFFFFFFFFFFFFFFF1"
  ,true);
  // std::cout << "m = " << print_string(m) << std::endl;
  xi = mod_inverse(x, m);
  xi_str = print_string(xi, true);
  EXPECT_STREQ(xi_str.c_str(), "A9079A630495AE72");

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
  m = create(
    "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223333"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
  ,true);
  // std::cout << "m = " << print_string(m) << std::endl;
  xi = mod_inverse(x, m);
  xi_str = print_string(xi);
  EXPECT_STREQ(xi_str.c_str(), "593907798222081907713519753568228587079077485424183140027845937692085970600993276044519953050220485652630892297068317920189808685646552294391932005896301266928262193839152923181064782576060385683621098621712938601668490927613979911");

  x = create(
    "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223AAF"
  ,true);
  // std::cout << "x = " << print_string(x) << std::endl;
  m = create(
    "FFFFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
  ,true);
  // std::cout << "m = " << print_string(m) << std::endl;
  xi = mod_inverse(x, m);
  xi_str = print_string(xi);
  EXPECT_STREQ(xi_str.c_str(), "");

  x = create("5");
  // std::cout << "x = " << print_string(x) << std::endl;
  m = create("8");
  // std::cout << "m = " << print_string(m) << std::endl;
  xi = mod_inverse(x, m);
  xi_str = print_string(xi);
  EXPECT_STREQ(xi_str.c_str(), "5");

  x = create("13");
  // std::cout << "x = " << print_string(x) << std::endl;
  m = create("7");
  // std::cout << "m = " << print_string(m) << std::endl;
  xi = mod_inverse(x, m);
  xi_str = print_string(xi);
  EXPECT_STREQ(xi_str.c_str(), "6");
}

TEST(Mympz, UnPass2Pass) {
  bignum_t y,r,m;
  std::string r_str;

  y = create(
    "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223333"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
  ,true);
  // std::cout << "y = " << print_string(y) << std::endl;

  m = create(
    "AADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223333"
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "4DDCAADD11223344FFBCDEA1234DDCDDCCAABB1289232CCAABB1289232CCAABB"
  ,true);
  // std::cout << "m = " << print_string(m) << std::endl;

  r = mod(y, m);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "35217974951994302427446093879638409683587491545935526331484845548968084286139");

  // r = mod_lshift1(y, m);
  // r_str = print_string(r);
  // EXPECT_STREQ(r_str.c_str(), "70435949903988604854892187759276819367174983091871052662969691097936168572278");

  // r = mod_lshift(y, 12, m);
  // r_str = print_string(r);
  // EXPECT_STREQ(r_str.c_str(), "392691905299878276836572593367703197882199378793218434439725836202397369155564506441085402488838759974990598521189523745187270818151022947259444646892984256613143124924662063654607166237393299221154795984575293697705653506823186511");
}

TEST(Mympz, Mod) {
  bignum_t x,y,r;
  std::string r_str;

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
  r = mod(y, x);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "21175685170448463646996560190595082856718904083028965863566610256317014862544035701382102883055223177540923991989036759128902518240610842297367761195819164670029291466913243438266560186281775403282937234818179930114533667349685864365124402546062495855264012312718565827275423656959267394759987548526661492308171746510134302056336661280062409908824935482968291135959676788812717106973328653246724939611287117341847309753596337140070095920182141572112632656711855675899680768856480893829913079541928423083306730812074004091389808178105949021705558621463693097067193200855000616963470929426491029267724381565516765238511");
}

TEST(Mympz, ModAdd) {
  bignum_t x,y,m,r;
  std::string r_str;

  m = create(
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
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
  ,true);

  // std::cout << "m = " << print_string(m) << std::endl;

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
  r = mod_add(x, y, m);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "289200035058554460164404010036420314436570403638731954645104133344719059899427687510739141802006379933219681771968230742297550893243867816594324878459721303460863237355723672050853994873385568263168671748165527596304070266699121742317294070476131425010155455837158350300777708527343466706829815612795079765628406127625452108782186153353240021469147129565862428866783461209562778218329494358356506180237226023979720570562265600611540476607044885487983050530541844817894016889068727527402098962704645150995713208916753956508181213981967255438016074663187833255242115952438374590262893247574699102674703041356048027416026000600742688196191213327288040600753098831366991948659511312718562573052590233073334444741240541365237277106594503217650878472421349652715414042900125200");
}

TEST(Mympz, ModSub) {
  bignum_t x,y,m,r;
  std::string r_str;

  m = create(
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
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
  ,true);

  // std::cout << "m = " << print_string(m) << std::endl;

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
  r = mod_sub(y, x, m);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "289200035058554460164404010036420314436570403638731954645104133344719059899427687510739141802006379933219681771968230742297550893243867816594324878459721260321815218520833723958902663179209169681391443145892389922924578950752313209352537898988328877816278101302579036770046258610855773382710950484115318879731239290711267014115128592256094549102879707178362537381908541372347522838764541687372981767681835247865168521753312015167054876593365878494655432762884702827766613385182800993996833922747807970756221794138677157342348863673204080732912200282315230904148261007399581443468460805361635154608721750671366509429927919528767156085483867047360946525388485733566694058254906775619610152480603359020927811734685306891241328613483297049347943917614178892806324134494671274");

  r = mod_sub(x, y, m);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "448988286722405113360035967620791220537257364876716618229267380413625074515460842228063106632124610997850377824962859682083562147132098241431668731194283090463785590573565888069179548777392259080054768314296641123131795921705232779827608003599955767762322049938477681730621951667635144693507473323390321187768620306846416030623317628057351840025921831014899057211108167472722151637448702515374406868498763494256531218922173999830058826387994868344447427938013397673445528882808103447562825897453730011060467676486791611435798965790879061300458126573092251163424829105824621895968778559608714061520549802067455023539488660767729312869261289533748218552568283245047219283255510452072619166674502376915481361695923968348647469752146723379323820375743613287968675360932267530672438313540852845278920550391361384902836299136563194955921479453469061125894117056001118919287512034451843579776280893818982795962483531346374310612789165669674391869971140721025461350195966999554529962308808908578975148654203017");
}

TEST(Mympz, ModMul) {
  bignum_t x,y,m,r;
  std::string r_str;

  m = create(
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
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
  ,true);

  // std::cout << "m = " << print_string(m) << std::endl;

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
  r = mod_mul(x, y, m);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "448405015358480386927035306480444652142961192257563464726292845316114811355218592063572222070034102790580845397997579560474874059643303211625188683461013274500231180329924463706210923172489091315533653284332367317028173037610125234551750823126065837483762881127485562473403177788938832077505707641819200450418788960084829858149866997386491484344608901788648354123806704129337702169859610062540908415133267485393881131649140542737523125889507038174698749079429690355218695782625775768161934754018724487148553248106637511578780670779934841330430741323916529314347521559533283753530848746842643268216675764399594279710850963879315591885128855278298815105305530808827944148888482948207353269811764095468049849023700697679197333395872363830857127343414004852251832177390473618300644859711925924501692519885296496990731944064249196242253800960124241901204774972178101747731136926069495717649943081245501640798345975229990444791117494817475779880422655821260231403743867882502475930847449331533522915994431246");
}

TEST(Mympz, ModLeftShift) {
  bignum_t y,m,r;
  std::string r_str;

  m = create(
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
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
  ,true);
  // std::cout << "m = " << print_string(m) << std::endl;
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
  r = mod_lshift1(y, m);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "578400070117108920328808020072840628873140807277463909290208266689438119798855375021478283604012759866439363543936461484595101786487735633188649756919442563782678455876557396009756658052594737944560114894057917519228649217451434951669831969464460302826433557139737387070823967138199240089540766096910398645359645418336719122897314745609334570572026836744224966248692002581910301057094036045729487947919061271844889092315577615778595353200410763982638483293426547645660630274251528521398932885452453121751935003055431113850530077655171336170928274945503064159390376959837956033731354052936334257283424792027414536845953920129509844281675080374648987126141584564933686006914418088338172725533193592094262256475925848256478605720077800266998822390035528545521738177394796474");

  r = mod_lshift(y, 5, m);
  r_str = print_string(r);
  EXPECT_STREQ(r_str.c_str(), "9254401121873742725260928321165450061970252916439422548643332267031009916781686000343652537664204157863029816702983383753521628583803770131018396110711081020522855294024918336156106528841515807112961838304926680307658387479222959226717311511431364845222936914235798193133183474211187841432652257550566378325754326693387505966357035929749353129152429387907599459979072041310564816913504576731671807166704980349518225477049241852457525651206572223722215732694824762330570084388024456342382926167239249948030960048886897821608481242482741378734852399128049026550246031357407296539701664846981348116534796672438632589535262722072157508506801285994383794018265353038938976110630689413410763608531097473508196103614813572103657691521244804271981158240568456728347810838316743584");
}

TEST(Mympz, ModExp) {
  bignum_t y,m,r;
  std::string r_str;

  m = create(
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
    "44FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA123"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
    "FFAADD11223344FFBCDEA1234DDCAADD11223344FFBCDEA1234DDCAADD112233"
  ,true);
  // std::cout << "m = " << print_string(m) << std::endl;
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
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}