#define _XOPEN_SOURCE 500

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PRJNAME_cli_opts.h"
#include "PRJNAME_config.h"
#include "PRJNAME_types.h"

void print_version();

int main(int argc, char **argv) {

  int ret = 0;
  PRJNAME_options_t options;
  memset(&options, 0, sizeof(options));

  ret = load_opts_defaults(&options);
  if (ret) exit(ret);

  ret = handle_opts(argc, argv, &options);
  if (ret) exit(ret);

  ret = validate_opts(&options);
  if (ret) exit(ret);

  if (options.debug) {
    show_opts(&options);
  }

  switch (options.command) {
  case cmd_do_main_function:
    printf("DO MAIN FUNCTION\n");
    break;
  case cmd_print_usage:
    print_usage(argc, argv);
    break;
  case cmd_print_version:
    print_version();
    break;
  }
  exit(ret);
}

void list_templates(PRJNAME_options_t *opts) {}

void print_version() {
  fprintf(stdout, "%d.%d\n", PRJNAME_VERSION_MAJOR, PRJNAME_VERSION_MINOR);
}
