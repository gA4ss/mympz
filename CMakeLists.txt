cmake_minimum_required (VERSION 3.5)
project(mympz VERSION 1.0)

# -g allows for gdb debugging
# turn on -03 for best performance
add_definitions(-std=c++11 -g -O0 -Wunused-variable)
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
  ${SRC_DIRECTORY}/exception.h
  ${SRC_DIRECTORY}/bignum.h
)

#
# Test Build
#
set(BUILD_TEST TRUE)
if(BUILD_TEST)
add_executable(test_bignum ${SRC_DIRECTORY}/test/test_bignum.cc)
target_link_libraries(test_bignum mympz gtest pthread)
add_executable(test_size ${SRC_DIRECTORY}/test/test_size.cc)
target_link_libraries(test_size)
endif(BUILD_TEST)

#
# Library Build
#
add_library(${PROJECT_NAME} STATIC
  ${SRC_DIRECTORY}/bignum/internal/shrink_zero.cc
  ${SRC_DIRECTORY}/bignum/internal/string_to_bignum.cc
  ${SRC_DIRECTORY}/bignum/internal/bignum_to_string.cc
  ${SRC_DIRECTORY}/bignum/internal/nan.cc
  ${SRC_DIRECTORY}/bignum/internal/zero.cc
  ${SRC_DIRECTORY}/bignum/internal/is_nan.cc
  ${SRC_DIRECTORY}/bignum/internal/is_zero.cc
  ${SRC_DIRECTORY}/bignum/internal/cmp.cc
  ${SRC_DIRECTORY}/bignum/internal/add.cc
  ${SRC_DIRECTORY}/bignum/internal/add2.cc
  ${SRC_DIRECTORY}/bignum/internal/sub.cc
  ${SRC_DIRECTORY}/bignum/internal/sub2.cc
  ${SRC_DIRECTORY}/bignum/internal/mul.cc
  ${SRC_DIRECTORY}/bignum/internal/div.cc
  ${SRC_DIRECTORY}/bignum/internal/div2.cc
  ${SRC_DIRECTORY}/bignum/shrink_zero.cc
  ${SRC_DIRECTORY}/bignum/string_to_bignum.cc
  ${SRC_DIRECTORY}/bignum/bignum_to_string.cc
  ${SRC_DIRECTORY}/bignum/nan.cc
  ${SRC_DIRECTORY}/bignum/zero.cc
  ${SRC_DIRECTORY}/bignum/is_nan.cc
  ${SRC_DIRECTORY}/bignum/is_zero.cc
  ${SRC_DIRECTORY}/bignum/cmp.cc
  ${SRC_DIRECTORY}/bignum/add.cc
  ${SRC_DIRECTORY}/bignum/add2.cc
  ${SRC_DIRECTORY}/bignum/sub.cc
  ${SRC_DIRECTORY}/bignum/sub2.cc
  ${SRC_DIRECTORY}/bignum/mul.cc
  ${SRC_DIRECTORY}/bignum/div.cc
  ${SRC_DIRECTORY}/bignum/div2.cc
)
target_link_libraries(${PROJECT_NAME} my)

#
# Enable Test
#
enable_testing()
add_test(NAME test_bignum COMMAND test_bignum)
set_tests_properties(test_bignum PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")

#
# Install Set
#
INSTALL(TARGETS mympz ARCHIVE DESTINATION lib)
INSTALL(FILES ${MYMPZ_HEADERS} DESTINATION include/mympz)