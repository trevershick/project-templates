#include <wordexp.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "realize_config.h"
#include "realize_defs.h"
#include "realize_io.h"

int validate_opts(realize_options_t *);
void show_opts(realize_options_t *);
void show_stats(realize_options_t *);
int handle_opts(realize_options_t *, int, char **);
int load_opts_defaults(realize_options_t *);

// commands
int do_list_templates(realize_options_t *);
int do_generate_project(realize_options_t *);
int do_print_version(realize_options_t *);
int do_print_usage(realize_options_t*, int, char **);

int main(int argc, char **argv) {
  int ret = 0;
  realize_options_t options;
  memset(&options, 0, sizeof(options));

  ret = load_opts_defaults(&options);
  if (ret)
    exit(ret);

  ret = handle_opts(&options, argc, argv);
  if (ret)
    exit(ret);

  ret = validate_opts(&options);
  if (ret)
    exit(ret);

  show_opts(&options);

  switch (options.command) {
  case cmd_generate_project:
    ret = do_generate_project(&options);
    show_stats(&options);
    break;
  case cmd_list_templates:
    ret = do_list_templates(&options);
    break;
  case cmd_print_usage:
    ret = do_print_usage(&options, argc, argv);
    break;
  case cmd_print_version:
    ret = do_print_version(&options);
    break;
  }
  exit(ret * -1);
}

void show_stats(realize_options_t *opts) {
  const char* fmt = "%-20.20s : %zu %s\n";
  const int ll = LOG_LEVEL_INFO;
  const char* d = opts->dryrun ? "(not really - dryrun)" : "";
  // clang-format off
  rprintf(opts, ll, fmt, "Template Files", opts->stats.template_files, d);
  rprintf(opts, ll, fmt, "Template Directories", opts->stats.template_dirs, d);
  rprintf(opts, ll, fmt, "Directories Created", opts->stats.dirs_added, d);
  rprintf(opts, ll, fmt, "Files Created", opts->stats.files_added, d);
  rprintf(opts, ll, fmt, "Files Replaced", opts->stats.files_replaced, d);
  // clang-format on
}
