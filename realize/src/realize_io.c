#include "realize_io.h"

#include <stdarg.h>
#include <stdio.h>

void rprintf(realize_options_t *opts, int level, const char *fmt, ...) {
  if (level > opts->log_level) {
    return;
  }
  FILE *out = stdout;
  if (level < LOG_LEVEL_INFO || level == LOG_LEVEL_ERROR) {
    out = stderr;
  }
  char buffer[1024];
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buffer, fmt, ap);
  fprintf(out, "%s", buffer);
  va_end(ap);
}
