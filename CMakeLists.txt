cmake_minimum_required (VERSION 3.5)
project(mympz VERSION 1.0)

# -g allows for gdb debugging
# turn on -03 for best performance
# add_definitions(-std=c++11 -g -O0 -DDEBUG -Wunused-variable)
add_definitions(-std=c++11 -g -O0 -DDEBUG -DDISABLE_OPTIMIZE -Wunused-variable)
# add_definitions(-std=c++11 -O3 -Wunused-variable)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

#
# Sub modules directories
#
set(SRC_DIRECTORY ./src/)

#
# Mympz include path
#
include_directories(
  ./
)

#
# System include path and link path
#
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

#
# Mympz public header file
#
set(MYMPZ_HEADERS
  ${SRC_DIRECTORY}/mympz.h
  ${SRC_DIRECTORY}/exception.h
)

#
# Library Build
#
add_library(${PROJECT_NAME} STATIC
  ${SRC_DIRECTORY}/internal.cc
  ${SRC_DIRECTORY}/create.cc
  ${SRC_DIRECTORY}/print.cc
  ${SRC_DIRECTORY}/add.cc
  ${SRC_DIRECTORY}/mul.cc
  ${SRC_DIRECTORY}/sqr.cc
  ${SRC_DIRECTORY}/div.cc
  ${SRC_DIRECTORY}/mod.cc
  ${SRC_DIRECTORY}/gcd.cc
  ${SRC_DIRECTORY}/shift.cc
  ${SRC_DIRECTORY}/exp.cc
  ${SRC_DIRECTORY}/cmp.cc
  ${SRC_DIRECTORY}/mympz.cc
  ${SRC_DIRECTORY}/debug.cc
)
target_link_libraries(${PROJECT_NAME} my)

#
# Test Build
#
set(BUILD_TEST TRUE)
if(BUILD_TEST)

#
# Main Test
#
add_executable(test_addsub ${SRC_DIRECTORY}/test/test_addsub.cc)
target_link_libraries(test_addsub mympz gtest pthread)
add_executable(test_create ${SRC_DIRECTORY}/test/test_create.cc)
target_link_libraries(test_create mympz gtest pthread)
add_executable(test_mul ${SRC_DIRECTORY}/test/test_mul.cc)
target_link_libraries(test_mul mympz gtest pthread)
add_executable(test_sqr ${SRC_DIRECTORY}/test/test_sqr.cc)
target_link_libraries(test_sqr mympz gtest pthread)
add_executable(test_div ${SRC_DIRECTORY}/test/test_div.cc)
target_link_libraries(test_div mympz gtest pthread)
add_executable(test_mod ${SRC_DIRECTORY}/test/test_mod.cc)
target_link_libraries(test_mod mympz gtest pthread)
add_executable(test_shift ${SRC_DIRECTORY}/test/test_shift.cc)
target_link_libraries(test_shift mympz gtest pthread)
add_executable(test_exp ${SRC_DIRECTORY}/test/test_exp.cc)
target_link_libraries(test_exp mympz gtest pthread)
add_executable(test_gcd ${SRC_DIRECTORY}/test/test_gcd.cc)
target_link_libraries(test_gcd mympz gtest pthread)

#
# Optimize Test
#
add_executable(test_x86_64_add ${SRC_DIRECTORY}/test/opt/test_x86_64_add.cc)
target_link_libraries(test_x86_64_add)
add_executable(test_x86_64_sub ${SRC_DIRECTORY}/test/opt/test_x86_64_sub.cc)
target_link_libraries(test_x86_64_sub)
add_executable(test_x86_64_mul ${SRC_DIRECTORY}/test/opt/test_x86_64_mul.cc)
target_link_libraries(test_x86_64_mul)
add_executable(test_x86_64_div ${SRC_DIRECTORY}/test/opt/test_x86_64_div.cc)
target_link_libraries(test_x86_64_div)
add_executable(test_x86_64_sqr ${SRC_DIRECTORY}/test/opt/test_x86_64_sqr.cc)
target_link_libraries(test_x86_64_sqr)

#
# Misc Test
#
add_executable(test_macro ${SRC_DIRECTORY}/test/misc/test_macro.cc)
target_link_libraries(test_macro mympz)
add_executable(test_bits_word ${SRC_DIRECTORY}/test/misc/test_bits_word.cc)
target_link_libraries(test_bits_word)
add_executable(test_left_align ${SRC_DIRECTORY}/test/misc/test_left_align.cc)
target_link_libraries(test_left_align)
add_executable(test_mul_unit ${SRC_DIRECTORY}/test/misc/test_mul_unit.cc)
target_link_libraries(test_mul_unit)
add_executable(test_div_unit ${SRC_DIRECTORY}/test/misc/test_div_unit.cc)
target_link_libraries(test_div_unit)

endif(BUILD_TEST)

#
# Enable Test
#
enable_testing()
add_test(NAME test_create COMMAND test_create)
set_tests_properties(test_create PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
add_test(NAME test_addsub COMMAND test_addsub)
set_tests_properties(test_addsub PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
add_test(NAME test_mul COMMAND test_mul)
set_tests_properties(test_mul PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
add_test(NAME test_sqr COMMAND test_sqr)
set_tests_properties(test_sqr PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
add_test(NAME test_div COMMAND test_div)
set_tests_properties(test_div PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
add_test(NAME test_mod COMMAND test_mod)
set_tests_properties(test_mod PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
add_test(NAME test_shift COMMAND test_shift)
set_tests_properties(test_shift PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
add_test(NAME test_exp COMMAND test_exp)
set_tests_properties(test_exp PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
add_test(NAME test_gcd COMMAND test_gcd)
set_tests_properties(test_gcd PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")

#
# Install Set
#
INSTALL(TARGETS mympz ARCHIVE DESTINATION lib)
INSTALL(FILES ${MYMPZ_HEADERS} DESTINATION include/mympz)