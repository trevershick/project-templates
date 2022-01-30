#include "realize_config.h"
#include "realize_io.h"

void do_print_version(realize_options_t *opts) {
  rprintf(opts, LOG_LEVEL_INFO, "%d.%d\n", realize_VERSION_MAJOR,
          realize_VERSION_MINOR);
}
