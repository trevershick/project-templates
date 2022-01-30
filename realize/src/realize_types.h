#ifndef __REALIZE_TYPES_H__
#define __REALIZE_TYPES_H__

#define _GNU_SOURCE
#define _XOPEN_SOURCE 500
#include <linux/limits.h>
#include <stdbool.h>
#include <sys/ioctl.h>

#define TEMPLATE_NAME_MAX 16
#define PROJECT_NAME_MAX 16

#define DEFAULT_PROJ_TMPLS_ROOT_PATH "~/project-templates"
#define TEMPLATE_FOLDER_FMT "%s/template-%s"

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

  // ~/project-templates/template-c
  char proj_tmpl_path[PATH_MAX];

  // c or whatever
  char template_name[TEMPLATE_NAME_MAX];
  // test1 for example
  char project_name[PROJECT_NAME_MAX];
  bool debug;
  struct winsize window_size;
} realize_options_t;

#endif
