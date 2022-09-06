#ifndef MYMPZ_DEBUG_H_
#define MYMPZ_DEBUG_H_

#include <mympz/mympz.h>

namespace mympz {

void dbgprint(const char* filepath, int lineno, const char* funcname, const char* fmt, ...);

#ifdef DEBUG
#define mympz_dbgprint(s)                    dbgprint("%s", (s))
#define mympz_dbgprint_fmt(format, ...)      dbgprint(format, __VA_ARGS__)
#else
#define mympz_dbgprint(s)
#define mympz_dbgprint_fmt(format, ...)
#endif

} // namespace mympz

#endif // MYMPZ_DEBUG_H_