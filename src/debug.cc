#include <mympz/mympz.h>
#include "__internal.h"

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>

namespace mympz {

void dbgprint(const char* filepath, int lineno, const char* funcname, const char* fmt, ...) {
  va_list args;
  char buffer[2048] = {0};

  //
  // FIXME : 没校验缓存长度,可能存在溢出
  //
  va_start(args, fmt);
  vsprintf(buffer, fmt, args);
  va_end(args);

  std::string title = "";
  if (filepath) {
    std::string filename = filepath;
    filename = filename.substr(filename.find_last_of('/')+1);
    title += filename;
    title += ":";
    title += std::to_string(lineno);
    title += ":";
  }
  if (funcname) {
    title += funcname;
    title += ": ";
  }
  title += buffer;

  printf("%s", title.c_str());
}

} // namespace mympz