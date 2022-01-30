#include "realize_cli_opts.h"

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <wordexp.h>

int load_opts_defaults(realize_options_t *opts) {
  strncpy(opts->proj_tmpls_root_path, DEFAULT_PROJ_TMPLS_ROOT_PATH,
          sizeof(opts->proj_tmpls_root_path));
  return 0;
}

void show_opts(const realize_options_t *opts) {
  fprintf(stderr, "Selected Options:\n");
  fprintf(stderr, "  Templates Root Path: %s\n", opts->proj_tmpls_root_path);
  fprintf(stderr, "\n");
}

int validate_opts(const realize_options_t *opts) {
  struct stat s;
  memset(&s, 0, sizeof(struct stat));
  if (stat(opts->proj_tmpls_root_path, &s)) {
    perror("stat template root path");
    return -1;
  }
  if (!S_ISDIR(s.st_mode)) {
    perror("template root path is not a directory");
    return -1;
  }

  return 0;
}

int handle_opts(int argc, char **argv, realize_options_t *opts) {
  opts->command = cmd_print_usage;

  int c;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"debug", no_argument, 0, 'd'},
        //{"delete",  required_argument, 0,  0 },
        {"version", no_argument, 0, 'v'},
        {"list", no_argument, 0, 'l'},
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

  if (optind < argc) {
    fprintf(stderr, "non-option ARGV-elements: ");
    while (optind < argc) {
      fprintf(stderr, "%s ", argv[optind++]);
    }
    fprintf(stderr, "\n");
    print_usage(argc, argv);
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
