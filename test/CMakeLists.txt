cmake_minimum_required (VERSION 3.5)
project(mympz_test VERSION 1.0)

# -g allows for gdb debugging
# turn on -03 for best performance
# add_definitions(-std=c++11 -g -O0 -Wunused-variable)
add_definitions(-std=c++11 -g -O0 -DDEBUG -Wunused-variable)
# add_definitions(-std=c++11 -g -O0 -DDEBUG -DDISABLE_OPTIMIZE -Wunused-variable)
# add_definitions(-std=c++11 -O3 -Wunused-variable)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

#
# Sub modules directories
#
set(SRC_DIRECTORY .)

#
# Mympz include path
#
include_directories(
  ../src
)

#
# System include path and link path
#
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

#
# Main Test
#
add_executable(test_addsub ${SRC_DIRECTORY}/test_addsub.cc)
target_link_libraries(test_addsub mympz gtest pthread)
add_executable(test_create ${SRC_DIRECTORY}/test_create.cc)
target_link_libraries(test_create mympz gtest pthread)
add_executable(test_mul ${SRC_DIRECTORY}/test_mul.cc)
target_link_libraries(test_mul mympz gtest pthread)
add_executable(test_sqr ${SRC_DIRECTORY}/test_sqr.cc)
target_link_libraries(test_sqr mympz gtest pthread)
add_executable(test_div ${SRC_DIRECTORY}/test_div.cc)
target_link_libraries(test_div mympz gtest pthread)
add_executable(test_mod ${SRC_DIRECTORY}/test_mod.cc)
target_link_libraries(test_mod mympz gtest pthread)
add_executable(test_shift ${SRC_DIRECTORY}/test_shift.cc)
target_link_libraries(test_shift mympz gtest pthread)
add_executable(test_exp ${SRC_DIRECTORY}/test_exp.cc)
target_link_libraries(test_exp mympz gtest pthread)
add_executable(test_gcd ${SRC_DIRECTORY}/test_gcd.cc)
target_link_libraries(test_gcd mympz gtest pthread)

#
# Optimize Test
#
add_executable(test_x86_64_add ${SRC_DIRECTORY}/opt/test_x86_64_add.cc)
target_link_libraries(test_x86_64_add)
add_executable(test_x86_64_sub ${SRC_DIRECTORY}/opt/test_x86_64_sub.cc)
target_link_libraries(test_x86_64_sub)
add_executable(test_x86_64_mul ${SRC_DIRECTORY}/opt/test_x86_64_mul.cc)
target_link_libraries(test_x86_64_mul)
add_executable(test_x86_64_div ${SRC_DIRECTORY}/opt/test_x86_64_div.cc)
target_link_libraries(test_x86_64_div)
add_executable(test_x86_64_sqr ${SRC_DIRECTORY}/opt/test_x86_64_sqr.cc)
target_link_libraries(test_x86_64_sqr)

#
# Misc Test
#
add_executable(test_macro ${SRC_DIRECTORY}/misc/test_macro.cc)
target_link_libraries(test_macro mympz)
add_executable(test_bits_word ${SRC_DIRECTORY}/misc/test_bits_word.cc)
target_link_libraries(test_bits_word)
add_executable(test_left_align ${SRC_DIRECTORY}/misc/test_left_align.cc)
target_link_libraries(test_left_align)
add_executable(test_mul_unit ${SRC_DIRECTORY}/misc/test_mul_unit.cc)
target_link_libraries(test_mul_unit)
add_executable(test_div_unit ${SRC_DIRECTORY}/misc/test_div_unit.cc)
target_link_libraries(test_div_unit)
add_executable(test_montgomery ${SRC_DIRECTORY}/misc/test_montgomery.cc)
target_link_libraries(test_montgomery mympz gtest pthread)