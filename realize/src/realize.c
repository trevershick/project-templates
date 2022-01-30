#define _XOPEN_SOURCE 500

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "realize_cli_opts.h"
#include "realize_config.h"
#include "realize_types.h"

void list_templates();
void print_version();

int main(int argc, char **argv) {

  int ret = 0;
  realize_options_t options;
  memset(&options, 0, sizeof(options));
  ret = load_opts_defaults(&options);
  ret = handle_opts(argc, argv, &options);
  if (ret) exit(ret);

  ret = validate_opts(&options);
  if (ret) exit(ret);
  if (options.debug) {
    show_opts(&options);
  }

  switch (options.command) {
  case cmd_generate_project:
    // generate_project(&options);
    break;
  case cmd_list_templates:
    list_templates(&options);
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

void list_templates(realize_options_t *opts) {}

void print_version() {
  fprintf(stdout, "%d.%d\n", realize_VERSION_MAJOR, realize_VERSION_MINOR);
}
