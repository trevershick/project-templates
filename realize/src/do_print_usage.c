#include "realize_defs.h"
#include "realize_io.h"

void do_print_usage(realize_options_t *opts, int arc, char **argv) {
  // clang-format off
  rprintf(opts, LOG_LEVEL_USAGE, "Usage:\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  %s (--list | -l)       List available templates.\n", argv[0]);
  rprintf(opts, LOG_LEVEL_USAGE, "  %s (--version | -v)    Print version information\n", argv[0]);
  rprintf(opts, LOG_LEVEL_USAGE, "  %s (--help | -h)       Print this help\n", argv[0]);
  rprintf(opts, LOG_LEVEL_USAGE, "  %s [options] <template> <name>   Generate a project from a " "template.\n", argv[0]);
  rprintf(opts, LOG_LEVEL_USAGE, "\n");
  rprintf(opts, LOG_LEVEL_USAGE, "Options:\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  -n, --dryrun          Don't actually change anything\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  -f, --force           Replace any existing files\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  -q, --quiet           Print less output\n");
  rprintf(opts, LOG_LEVEL_USAGE, "  -d, --debug           Print extra debugging info\n");
  // clang-format on
}
