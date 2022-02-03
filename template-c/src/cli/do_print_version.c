#include "PRJNAME_config.h"
#include "PRJNAME_io.h"

void do_print_version(PRJNAME_options_t *opts) {
  rprintf(opts, LOG_LEVEL_INFO, "%d.%d\n", PRJNAME_VERSION_MAJOR,
          PRJNAME_VERSION_MINOR);
}
