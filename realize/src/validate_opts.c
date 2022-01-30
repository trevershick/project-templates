#include "ezfs.h"
#include "ezregex.h"
#include "realize_defs.h"
#include "realize_io.h"

int validate_opts(realize_options_t *opts) {
  if (!ez_is_dir(opts->proj_tmpls_root_path)) {
    rprintf(opts, LOG_LEVEL_ERROR, "Template Root Path (%s) does not exist\n",
            opts->proj_tmpls_root_path);
    return -1;
  }

  if (opts->command == cmd_generate_project) {
    if (ez_matches(PNAME_PATTERN, opts->project_name) == EZREGEX_NOMATCH) {
      rprintf(opts, LOG_LEVEL_ERROR,
              "Project name must match " PNAME_PATTERN "\n");
      return -1;
    }

    // validate the specified template exists in the root
    if (!ez_is_dir(opts->proj_tmpl_path)) {
      rprintf(opts, LOG_LEVEL_ERROR,
              "Project Template Directory (%s) does not exist\n",
              opts->proj_tmpl_path);
      return -1;
    }
  }

  return 0;
}
