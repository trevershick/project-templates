#include "PRJNAME_io.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

void rprintf(PRJNAME_options_t *opts, int level, const char *fmt, ...) {
  assert(opts);
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
