cmake_minimum_required (VERSION 3.5)
project(mympz VERSION 1.0)

# -g allows for gdb debugging
# turn on -03 for best performance
# add_definitions(-std=c++11 -g -O0 -Wunused-variable)
# add_definitions(-std=c++11 -g -O0 -DDEBUG -Wunused-variable)
# add_definitions(-std=c++11 -g -O0 -DDEBUG -DDISABLE_OPTIMIZE -Wunused-variable)
add_definitions(-std=c++11 -O3 -Wunused-variable)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

#
# Sub modules directories
#
set(SRC_DIRECTORY ./src)

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
  ${SRC_DIRECTORY}/config.h
  ${SRC_DIRECTORY}/compile.h
  ${SRC_DIRECTORY}/exception.h
  ${SRC_DIRECTORY}/debug.h
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
  ${SRC_DIRECTORY}/gcd.cc
  ${SRC_DIRECTORY}/mod.cc
  ${SRC_DIRECTORY}/mod_inverse.cc
  ${SRC_DIRECTORY}/montgomery_mod.cc
  ${SRC_DIRECTORY}/reciprocal_mod.cc
  ${SRC_DIRECTORY}/shift.cc
  ${SRC_DIRECTORY}/exp.cc
  ${SRC_DIRECTORY}/cmp.cc
  ${SRC_DIRECTORY}/mympz.cc
  ${SRC_DIRECTORY}/const.cc
  ${SRC_DIRECTORY}/debug.cc
)
target_link_libraries(${PROJECT_NAME} my)

#
# Test Build
#
set(BUILD_TEST TRUE)
if(BUILD_TEST)

#
# Add SubDirectory
#
add_subdirectory(test)

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
# Auto Test
#
set(BUILD_AUTOTEST FALSE)
if(BUILD_AUTOTEST)
endif(BUILD_AUTOTEST)

endif(BUILD_TEST)

#
# Install Set
#
INSTALL(TARGETS mympz ARCHIVE DESTINATION lib)
INSTALL(FILES ${MYMPZ_HEADERS} DESTINATION include/mympz)