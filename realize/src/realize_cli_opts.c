#include "realize_cli_opts.h"

#include "realize_types.h"

#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <wordexp.h>

#include "ezregex.h"
#include "ezfs.h"


int load_opts_defaults(realize_options_t *opts) {
  strncpy(opts->proj_tmpls_root_path, DEFAULT_PROJ_TMPLS_ROOT_PATH,
          sizeof(opts->proj_tmpls_root_path));
  ioctl(1, TIOCGWINSZ, &opts->window_size);
  return 0;
}

void show_opts(const realize_options_t *opts) {
  fprintf(stderr, "Selected Options:\n");
  fprintf(stderr, "  %-25s: %s\n", "Templates Root Path",
          opts->proj_tmpls_root_path);
  fprintf(stderr, "  %-25s: %s\n", "Template Name", opts->template_name);
  fprintf(stderr, "  %-25s: %s\n", "Project Name", opts->project_name);
  fprintf(stderr, "  %-25s: %s\n", "Force", opts->force ? "Y" : "N");
  fprintf(stderr, "  %-25s: %s\n", "Debug", opts->debug ? "Y" : "N");
  fprintf(stderr, "\n");
}

int validate_opts(const realize_options_t *opts) {

  if (!ez_is_dir(opts->proj_tmpls_root_path)) {
    fprintf(stderr, "Template Root Path (%s) does not exist\n", opts->proj_tmpls_root_path);
    return -1;
  }

  #define PNAME_PATTERN "^[a-zA-Z0-9]+$"
  if (ez_matches(PNAME_PATTERN, opts->project_name) == EZREGEX_NOMATCH) {
      fprintf(stderr, "Project name must match " PNAME_PATTERN "\n");
      return -1;
  }

  // validate the specified template exists in the root
  if (!ez_is_dir(opts->proj_tmpl_path)) {
    fprintf(stderr, "Project Template Directory (%s) does not exist\n", opts->proj_tmpl_path);
    return -1;
  }

  return 0;
}

int handle_opts(int argc, char **argv, realize_options_t *opts) {
  opts->command = cmd_generate_project;

  int c;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"force", no_argument, 0, 'f'},
        {"debug", no_argument, 0, 'd'},
        //{"delete",  required_argument, 0,  0 },
        {"version", no_argument, 0, 'v'},
        {"list", no_argument, 0, 'l'},
        {0, 0, 0, 0}};
    c = getopt_long(argc, argv, "hfdvl?", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
    case 'f':
      opts->force = true;
      break;
    case 'd':
      opts->debug = true;
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
      fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
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
      fprintf(stderr, "The command requires exactly two arguments: template "
                      "and project name\n");
      return -1;
    }

    strncpy(opts->template_name, argv[optind], sizeof(opts->template_name));
    if (strlen(argv[optind]) > TEMPLATE_NAME_MAX) {
      fprintf(stderr, "Template name cannot be > %d characters in length.\n",
              TEMPLATE_NAME_MAX);
      return -1;
    }

    strncpy(opts->project_name, argv[optind + 1], sizeof(opts->project_name));
    if (strlen(argv[optind + 1]) > PROJECT_NAME_MAX) {
      fprintf(stderr, "Template name cannot be > %d characters in length.\n",
              TEMPLATE_NAME_MAX);
      return -1;
    }
  snprintf(
    opts->proj_tmpl_path,
    sizeof(opts->proj_tmpl_path),
     TEMPLATE_FOLDER_FMT, opts->proj_tmpls_root_path, opts->template_name);
  }
  // we only list projects once we have our options filledin
  // since we need to
  return 0;
}

void print_usage(int arc, char **argv) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr,
          "  %s (--list|-l)                   List available templates.\n",
          argv[0]);
  fprintf(stderr,
          "  %s [options] <template> <name>   Generate a project from a "
          "template.\n",
          argv[0]);
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -h, --help            Print this help message\n");
  fprintf(stderr, "  -v, --version         Print version information\n");
  fprintf(
      stderr,
      "  -d, --debug           Print extra debugging info during processing\n");
}
