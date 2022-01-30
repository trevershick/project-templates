#include <getopt.h>
#include <wordexp.h>
#include <stdio.h>
#include <string.h>

#include "realize_defs.h"
#include "realize_io.h"

int handle_opts(realize_options_t *opts, int argc, char **argv) {
  opts->command = cmd_generate_project;

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
        {"list", no_argument, 0, 'l'},
        {0, 0, 0, 0}
        // clang-format on
    };
    c = getopt_long(argc, argv, "vh?fqldn", long_options, &option_index);
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
    case 'l':
      opts->command = cmd_list_templates;
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

  // expand any paths
  wordexp_t expansion;
  int ret = wordexp(opts->proj_tmpls_root_path, &expansion, 0);

  if (ret) {
    perror("template path expansion failed");
    wordfree(&expansion);
    return -1;
  }

  // the project path is set as a default value but can be
  // overridden, so we need to expand it AFTER any overrides
  if (expansion.we_wordc > 1) {
    perror("template path expansion yielded > 1 value");
    wordfree(&expansion);
    return -1;
  }
  strncpy(opts->proj_tmpls_root_path, expansion.we_wordv[0],
          sizeof(opts->proj_tmpls_root_path)-1);

  if (opts->command == cmd_generate_project) {
    // we need two arguments here
    if (argc - optind != 2) {
      perror("exactly two arguments are required");
      rprintf(opts, LOG_LEVEL_ERROR,
              "The command requires exactly two arguments: template "
              "and project name\n");
      return -1;
    }

    strncpy(opts->template_name, argv[optind], sizeof(opts->template_name)-1);
    if (strlen(argv[optind]) > TEMPLATE_NAME_MAX) {
      rprintf(opts, LOG_LEVEL_ERROR,
              "Template name cannot be > %d characters in length.\n",
              TEMPLATE_NAME_MAX);
      return -1;
    }

    strncpy(opts->project_name, argv[optind + 1], sizeof(opts->project_name)-1);
    if (strlen(argv[optind + 1]) > PROJECT_NAME_MAX) {
      rprintf(opts, LOG_LEVEL_ERROR,
              "Template name cannot be > %d characters in length.\n",
              TEMPLATE_NAME_MAX);
      return -1;
    }
    snprintf(opts->proj_tmpl_path, sizeof(opts->proj_tmpl_path),
             TEMPLATE_FOLDER_FMT, opts->proj_tmpls_root_path,
             opts->template_name);
  }
  // we only list projects once we have our options filledin
  // since we need to
  return 0;
}
