#include "realize_defs.h"

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <wordexp.h>

#include "ezfs.h"
#include "ezregex.h"
#include "realize_io.h"

int load_opts_defaults(realize_options_t *opts) {
  strncpy(opts->proj_tmpls_root_path, DEFAULT_PROJ_TMPLS_ROOT_PATH,
          sizeof(opts->proj_tmpls_root_path));
  ioctl(1, TIOCGWINSZ, &opts->window_size);
  strcpy(opts->project_name, NOT_SET_STRING);
  strcpy(opts->template_name, NOT_SET_STRING);
  return 0;
}

#define OPTS_LINE_FMT "  %-25s: %s\n"
void show_opts(realize_options_t *opts) {
  // log at debug level
  rprintf(opts, LOG_LEVEL_DEBUG, "Selected Options:\n");
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Templates Root Path",
          opts->proj_tmpls_root_path);
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Template Name",
          opts->template_name);
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Project Name",
          opts->project_name);
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Force",
          opts->force ? "Y" : "N");
  rprintf(opts, LOG_LEVEL_DEBUG, OPTS_LINE_FMT, "Debug",
          (opts->log_level == LOG_LEVEL_DEBUG) ? "Y" : "N");
  rprintf(opts, LOG_LEVEL_DEBUG, "\n");
}

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

int handle_opts(realize_options_t *opts, int argc, char **argv) {
  opts->command = cmd_generate_project;

  int c;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                           {"force", no_argument, 0, 'f'},
                                           {"debug", no_argument, 0, 'd'},
                                           {"quiet", no_argument, 0, 'q'},
                                           {"version", no_argument, 0, 'v'},
                                           {"list", no_argument, 0, 'l'},
                                           {0, 0, 0, 0}};
    c = getopt_long(argc, argv, "hqfvl?d", long_options, &option_index);
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
          sizeof(opts->proj_tmpls_root_path));

  if (opts->command == cmd_generate_project) {
    // we need two arguments here
    if (argc - optind != 2) {
      perror("exactly two arguments are required");
      rprintf(opts, LOG_LEVEL_ERROR,
              "The command requires exactly two arguments: template "
              "and project name\n");
      return -1;
    }

    strncpy(opts->template_name, argv[optind], sizeof(opts->template_name));
    if (strlen(argv[optind]) > TEMPLATE_NAME_MAX) {
      rprintf(opts, LOG_LEVEL_ERROR,
              "Template name cannot be > %d characters in length.\n",
              TEMPLATE_NAME_MAX);
      return -1;
    }

    strncpy(opts->project_name, argv[optind + 1], sizeof(opts->project_name));
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

void do_print_usage(realize_options_t *opts, int arc, char **argv) {
  // clang-format: off
  rprintf(opts, LOG_LEVEL_ERROR, "Usage:\n");
  rprintf(opts, LOG_LEVEL_ERROR,
          "  %s (--list|-l)                   List available templates.\n",
          argv[0]);
  rprintf(opts, LOG_LEVEL_ERROR,
          "  %s [options] <template> <name>   Generate a project from a "
          "template.\n",
          argv[0]);
  rprintf(opts, LOG_LEVEL_ERROR, "\n");
  rprintf(opts, LOG_LEVEL_ERROR, "Options:\n");
  rprintf(opts, LOG_LEVEL_ERROR,
          "  -h, --help            Print this help message\n");
  rprintf(opts, LOG_LEVEL_ERROR,
          "  -v, --version         Print version information\n");
  rprintf(
      opts, LOG_LEVEL_ERROR,
      "  -d, --debug           Print extra debugging info during processing\n");
  // clang-format: on
}
