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
  ${SRC_DIRECTORY}/mympz.h
  ${SRC_DIRECTORY}/exception.h
)

#
# Library Build
#
add_library(${PROJECT_NAME} STATIC
  ${SRC_DIRECTORY}/create.cc
  ${SRC_DIRECTORY}/calc.cc
  ${SRC_DIRECTORY}/add.cc
  ${SRC_DIRECTORY}/cmp.cc
  ${SRC_DIRECTORY}/one.cc
  ${SRC_DIRECTORY}/zero.cc
  ${SRC_DIRECTORY}/print.cc
)
target_link_libraries(${PROJECT_NAME} my)

#
# Test Build
#
set(BUILD_TEST TRUE)
if(BUILD_TEST)
add_executable(test_add ${SRC_DIRECTORY}/test/test_add.cc)
target_link_libraries(test_add mympz gtest pthread)
add_executable(test_overflow ${SRC_DIRECTORY}/test/test_overflow.cc)
target_link_libraries(test_overflow)
add_executable(test_size ${SRC_DIRECTORY}/test/test_size.cc)
target_link_libraries(test_size)
add_executable(test_macro ${SRC_DIRECTORY}/test/test_macro.cc)
target_link_libraries(test_macro mympz)
add_executable(test_compile ${SRC_DIRECTORY}/test/test_compile.cc)
target_link_libraries(test_compile)
add_executable(test_create ${SRC_DIRECTORY}/test/test_create.cc)
target_link_libraries(test_create mympz gtest pthread)
add_executable(test_deque ${SRC_DIRECTORY}/test/test_deque.cc)
target_link_libraries(test_deque)
endif(BUILD_TEST)

#
# Enable Test
#
# enable_testing()
# add_test(NAME test_bignum COMMAND test_bignum)
# set_tests_properties(test_bignum PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")

#
# Install Set
#
INSTALL(TARGETS mympz ARCHIVE DESTINATION lib)
INSTALL(FILES ${MYMPZ_HEADERS} DESTINATION include/mympz)