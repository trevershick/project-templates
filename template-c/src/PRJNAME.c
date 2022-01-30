#define _XOPEN_SOURCE 500

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PRJNAME_config.h"
#include "PRJNAME_defs.h"

int validate_opts(PRJNAME_options_t *);
void show_opts(PRJNAME_options_t *);
void show_stats(PRJNAME_options_t *);
int handle_opts(PRJNAME_options_t *, int, char **);
int load_opts_defaults(PRJNAME_options_t *);

// commands
int do_main_function(PRJNAME_options_t*);
int do_generate_project(PRJNAME_options_t *);
int do_print_version(PRJNAME_options_t *);
int do_print_usage(PRJNAME_options_t *, int, char **);

int main(int argc, char **argv) {
  int ret = 0;
  PRJNAME_options_t options;
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
  case cmd_do_main_function:
    ret = do_main_function(&options);
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
