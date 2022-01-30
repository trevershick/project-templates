#ifndef _REALIZE_TYPES_H
#define _REALIZE_TYPES_H

#include <limits.h>
#include <stdbool.h>

#define TEMPLATE_NAME_MAX 16
#define PROJECT_NAME_MAX 16

enum realize_command {
  cmd_generate_project,
  cmd_list_templates,
  cmd_print_usage,
  cmd_print_version,
};

typedef struct realize_options_s {
  enum realize_command command;
  // ~/project-templates by default
  char proj_tmpls_root_path[PATH_MAX];
  // c or whatever
  char template_name[TEMPLATE_NAME_MAX];
  // test1 for example
  char project_name[PROJECT_NAME_MAX];
  bool debug;
} realize_options_t;

#endif
