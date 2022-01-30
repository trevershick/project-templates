#include "PRJNAME_defs.h"
#include "PRJNAME_io.h"

void do_print_usage(PRJNAME_options_t *opts, int arc, char **argv) {
  // clang-format off
  rprintf(opts, LOG_LEVEL_USAGE, "Usage:\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  %s (--version | -v)    Print version information\n", argv[0]);
  rprintf(opts, LOG_LEVEL_USAGE, "  %s (--help | -h)       Print this help\n", argv[0]);
  rprintf(opts, LOG_LEVEL_USAGE, "\n");
  rprintf(opts, LOG_LEVEL_USAGE, "Options:\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  -n, --dryrun          Don't actually change anything\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  -f, --force           Replace any existing files\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  -q, --quiet           Print less output\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  -d, --debug           Print extra debugging info\n");
  // clang-format on
}
