#ifndef MYMPZ_EXCEPTION_H_
#define MYMPZ_EXCEPTION_H_

#include <ctype.h>
#include <my/my_common.h>

namespace mympz
{
#define mympz_exception(format, ...)                                              \
  {                                                                               \
    throw my::MyException("<mympz>",                                              \
                          __FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__); \
  }
#define mympz_divide_by_zero(x)                                                              \
  {                                                                                          \
    throw my::MyException("<mympz>",                                                         \
                          __FILE__, __LINE__, __FUNCTION__, "%s", "divide " #x " is zero."); \
  }
#define mympz_out_of_range(x)                                                              \
  {                                                                                        \
    throw my::MyException("<mympz>",                                                       \
                          __FILE__, __LINE__, __FUNCTION__, "%s", #x " is out of range."); \
  }
#define mympz_null_address(x)                                                                 \
  {                                                                                           \
    throw my::MyException("<mympz>",                                                          \
                          __FILE__, __LINE__, __FUNCTION__, "%s", "pointer " #x " is null."); \
  }
#define mympz_invalid_argument(x)                                                                  \
  {                                                                                                \
    throw my::MyException("<mympz>",                                                               \
                          __FILE__, __LINE__, __FUNCTION__, "%s", "invalid argument \'" #x "\'."); \
  }
#define mympz_mod_mul_montgomery_invalid_size(xl, yl, nl)          \
  {                                                                \
    throw my::MyException("<mympz>",                               \
                          __FILE__, __LINE__, __FUNCTION__,        \
                          "xl(%d) + yl(%d) > nl(%d)", xl, yl, nl); \
  }
#define unknown_mympz_exception()                                                                   \
  {                                                                                                 \
    throw my::MyException("<mympz> Unknown Exception", __FILE__, __LINE__, __FUNCTION__, "%s", ""); \
  }
#define mympz_check_number_size(x) \
  {                                \
    if ((x).empty())               \
    {                              \
      mympz_invalid_argument(x);   \
    }                              \
  }
} // namespace mympz

#endif // MYMPZ_EXCEPTION_H_
