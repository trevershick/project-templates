#include <getopt.h>
#include <wordexp.h>

#include "PRJNAME_defs.h"
#include "PRJNAME_io.h"

int handle_opts(PRJNAME_options_t *opts, int argc, char **argv) {
  opts->command = cmd_do_main_function;

  int c;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
        // clang-format off
        {"help", no_argument, 0, 'h'},
        {"force", no_argument, 0, 'f'},
        {"debug", no_argument, 0, 'd'},
        {"quiet", no_argument, 0, 'q'},
        {"dryrun", no_argument, 0, 'n'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
        // clang-format on
    };
    c = getopt_long(argc, argv, "vh?fqdn", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
    case 'f':
      opts->force = true;
      break;
    case 'q':
      opts->log_level = LOG_LEVEL_QUIET;
      break;
    case 'd':
      opts->log_level = LOG_LEVEL_DEBUG;
      break;
    case 'n':
      opts->dryrun = true;
      break;
    case 'v':
      opts->command = cmd_print_version;
      break;
    case 'h':
    case '?':
      opts->command = cmd_print_usage;
      break;
    default:
      rprintf(opts, LOG_LEVEL_ERROR,
              "?? getopt returned character code 0%o ??\n", c);
      break;
    }
  }

  if (opts->command == cmd_do_main_function) {
    // we need two arguments here
    // if (argc - optind != 2) {
    //   fprintf(stderr, "The command requires exactly two arguments: template "
    //                   "and project name");
    //   return -1;
    // }

    // strncpy(opts->template_name, argv[optind], sizeof(opts->template_name));
    // strncpy(opts->project_name, argv[optind + 1],
    // sizeof(opts->project_name));
  }
  return 0;
}
