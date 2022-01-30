#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "realize_config.h"

enum realize_command {
  cmd_generate_project,
  cmd_list_templates,
  cmd_print_usage,
  cmd_print_version,
};

typedef struct realize_options_s {
  enum realize_command command;
} realize_options_t;

void print_usage(int arc, char **argv);
int handle_opts(int, char **, realize_options_t *);
void list_templates();
void print_version();




int main(int argc, char **argv) {

  realize_options_t options;
  memset(&options, 0, sizeof(options));
  handle_opts(argc, argv, &options);

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
  exit(0);
}

int handle_opts(int argc, char **argv, realize_options_t *opts) {
  opts->command = cmd_generate_project;

  int c;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        //{"delete",  required_argument, 0,  0 },
        {"version", no_argument, 0, 'v'},
        {"list", no_argument, 0, 'l'},
        {0, 0, 0, 0}};
    c = getopt_long(argc, argv, "hv", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
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

  // we only list projects once we have our options filledin
  // since we need to
  return 0;
}

void list_templates(realize_options_t* opts) {
}

void print_version() {
  fprintf(stdout, "%d.%d\n", realize_VERSION_MAJOR, realize_VERSION_MINOR);
}

void print_usage(int arc, char **argv) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  %s (--list|-l)                   List available templates.\n", argv[0]);
  fprintf(stderr, "  %s [options] <template> <name>   Generate a project from a template.\n", argv[0]);
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr,"  -h, --help            Print this help message\n");
  fprintf(stderr,"  -v, --version         Print version information\n");
}
