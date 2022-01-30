#include <stdio.h>

#include "PRJNAME_defs.h"
#include "PRJNAME_io.h"

const char *const kLogLevels[] = {
    "Error", "Quiet", "Info", "Debug", "Trace",
};

static const char *ll_str(PRJNAME_options_t *o) {
  int i = o->log_level + LOG_LEVEL_ERROR;
  if (i < 0 || i > (int) sizeof(kLogLevels) - 1) {
    return "???";
  }
  return kLogLevels[i];
}

#define Y_OR_N(i) (i ? "Y" : "N")
#define OPTS_LINE_FMT "  %-25s: %s\n"
void show_opts(PRJNAME_options_t *opts) {
  // log at debug level
  // clang-format off
  rprintf(opts, LOG_LEVEL_DEBUG, "Selected Options:\n");
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Force", Y_OR_N(opts->force));
  // clang-format on

  char buffer[8];
  snprintf(buffer, sizeof(buffer), "%d", opts->log_level);
  // clang-format off
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Log Level", ll_str(opts));
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Dry Run", Y_OR_N(opts->dryrun));
  rprintf(opts, LOG_LEVEL_DEBUG, "\n");
  // clang-format on
}
