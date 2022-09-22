#ifndef MYMPZ_EXCEPTION_H_
#define MYMPZ_EXCEPTION_H_

#include <ctype.h>
#include <my/my_common.h>

namespace mympz
{

#define unknown_mympz_exception()                                                                   \
  {                                                                                                 \
    throw my::MyException("<mympz> Unknown Exception", __FILE__, __LINE__, __FUNCTION__, "%s", ""); \
  }
#define mympz_exception(format, ...)                                              \
  {                                                                               \
    throw my::MyException("<mympz>",                                              \
                          __FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__); \
  }
#define mympz_null_address(x)                                                                 \
  {                                                                                           \
    throw my::MyException("<mympz>",                                                          \
                          __FILE__, __LINE__, __FUNCTION__, "%s", "pointer " #x " is null."); \
  }
#define mympz_check_size(x)                                 \
  {                                                         \
    my_assert(!x.number.empty(), "%s", "bignum is empty."); \
  }
#define mympz_return_error(x)                                \
  {                                                          \
    my_assert(!is_error(x), "%s", "function return error."); \
  }

} // namespace mympz

#endif // MYMPZ_EXCEPTION_H_
