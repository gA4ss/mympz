#ifndef MYMPZ_EXCEPTION_H_
#define MYMPZ_EXCEPTION_H_

#include <ctype.h>
#include <my/my_exception.h>

namespace mympz {

#define unknown_mympz_exception() { throw my::MyException("<mympz> Unknown Exception", __FILE__, __LINE__, __FUNCTION__, "%s", ""); }
#define mympz_exception(format, ...) { \
  throw my::MyException("<mympz> Exception", \
    __FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__); \
}

} // namespace mympz

#endif // MYMPZ_EXCEPTION_H_
