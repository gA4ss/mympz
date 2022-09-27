#include <mympz/mympz.h>
#include "__internal.h"
namespace mympz
{

//
// number与缓冲区辅助宏
//
#define __alloc_unit_buffer(n) (unit_t *)malloc(sizeof(unit_t) * n)
#define __free_unit_buffer(buf) \
  {                             \
    if (buf)                    \
      free(buf);                \
    (buf) = nullptr;            \
  }

#define __copy_number_to_buf(num, buf, n)                       \
  {                                                             \
    for (size_t __cntb_i__ = 0; __cntb_i__ < (n); __cntb_i__++) \
      (buf)[__cntb_i__] = (num)[__cntb_i__];                    \
  }

#define __copy_buf_to_number(buf, num, n)                       \
  {                                                             \
    for (size_t __cntb_i__ = 0; __cntb_i__ < (n); __cntb_i__++) \
      (num)[__cntb_i__] = (buf)[__cntb_i__];                    \
  }

#if !defined(DISABLE_OPTIMIZE)
#include "./internal/optimize/__optimize.cc"
#endif

#include "./internal/__internal.cc"
#include "./internal/__internal_bn.cc"
#include "./internal/__print.cc"
#include "./internal/__create.cc"
#include "./internal/__cmp.cc"
#include "./internal/__addsub.cc"
#include "./internal/__mul.cc"
#include "./internal/__sqr.cc"  // 需要放置在__mul.cc之后
#include "./internal/__div.cc"
#include "./internal/__mod.cc"
#include "./internal/__exp.cc"

} // namespace mympz