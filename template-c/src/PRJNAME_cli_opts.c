#include "PRJNAME_cli_opts.h"

#include "PRJNAME_types.h"

#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <wordexp.h>



int load_opts_defaults(PRJNAME_options_t *opts) {
  // populate any default values
  return 0;
}

void show_opts(const PRJNAME_options_t *opts) {
  fprintf(stderr, "Selected Options:\n");
  //fprintf(stderr, "  %-25s: %s\n", "Project Name", opts->project_name);
  fprintf(stderr, "\n");
}

int validate_opts(const PRJNAME_options_t *opts) {
  return 0;
}

int handle_opts(int argc, char **argv, PRJNAME_options_t *opts) {
  int c;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"debug", no_argument, 0, 'd'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}};
    c = getopt_long(argc, argv, "hdvl?", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
    case 'd':
      opts->debug = true;
      break;
    case 'v':
      opts->command = cmd_print_version;
      break;
    case 'h':
    case '?':
      opts->command = cmd_print_usage;
      break;
    default:
      fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
      break;
    }
  }

  if (opts->command == cmd_do_main_function) {
    // we need two arguments here
    // if (argc - optind != 2) {
    //   perror("exactly two arguments are required");
    //   fprintf(stderr, "The command requires exactly two arguments: template "
    //                   "and project name");
    //   return -1;
    // }

    // strncpy(opts->template_name, argv[optind], sizeof(opts->template_name));
    // strncpy(opts->project_name, argv[optind + 1], sizeof(opts->project_name));
  }
  return 0;
}

void print_usage(int arc, char **argv) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr,
          "  %s                               Do Something.\n",
          argv[0]);
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -h, --help            Print this help message\n");
  fprintf(stderr, "  -v, --version         Print version information\n");
  fprintf(
      stderr,
      "  -d, --debug           Print extra debugging info during processing\n");
}
