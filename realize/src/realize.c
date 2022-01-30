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
int do_print_version();
int do_print_usage(int, char **);

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
    ret = do_print_usage(argc, argv);
    break;
  case cmd_print_version:
    ret = do_print_version();
    break;
  }
  exit(ret * -1);
}

#define STAT_LINE_FORMAT "%-20.20s : %zu\n"
void show_stats(realize_options_t *opts) {
  rprintf(opts, LOG_LEVEL_INFO, STAT_LINE_FORMAT, "Template Files",
          opts->stats.template_files);
  rprintf(opts, LOG_LEVEL_INFO, STAT_LINE_FORMAT, "Template Directories",
          opts->stats.template_dirs);
  rprintf(opts, LOG_LEVEL_INFO, STAT_LINE_FORMAT, "Directories Created",
          opts->stats.dirs_added);
  rprintf(opts, LOG_LEVEL_INFO, STAT_LINE_FORMAT, "Files Created",
          opts->stats.files_added);
  rprintf(opts, LOG_LEVEL_INFO, STAT_LINE_FORMAT, "Files Replaced",
          opts->stats.files_replaced);
}
