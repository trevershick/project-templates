#include "realize_defs.h"
#include "realize_io.h"

#include <stdio.h>

static const char *ll_str(realize_options_t *o)
{
    switch (o->log_level)
    {
    case LOG_LEVEL_TRACE:
        return "trace";
    case LOG_LEVEL_DEBUG:
        return "debug";
    case LOG_LEVEL_INFO:
        return "info";
    case LOG_LEVEL_QUIET:
        return "quiet";
    case LOG_LEVEL_ERROR:
        return "error";
    default:
        return "???";
    }
}

#define OPTS_LINE_FMT "  %-25s: %s\n"
void show_opts(realize_options_t *opts)
{
    // log at debug level
    // clang-format off
  rprintf(opts, LOG_LEVEL_DEBUG, "Selected Options:\n");
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Config File", opts->configuration_file);
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Templates Root Path", opts->proj_tmpls_root_path);
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Template Name", opts->template_name);
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Project Name", opts->project_name);
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
