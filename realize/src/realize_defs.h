#ifndef __REALIZE_DEFS_H__
#define __REALIZE_DEFS_H__

#include <linux/limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/ioctl.h>

#define TEMPLATE_NAME_MAX 16
#define PROJECT_NAME_MAX 16
#define DEFAULT_PROJ_TMPLS_ROOT_PATH "~/project-templates"
#define TEMPLATE_FOLDER_FMT "%s/template-%s"
#define NOT_SET_STRING "<none>"
#define PNAME_PATTERN "^[a-zA-Z0-9]+$"
#define TOKEN "PRJNAME"
#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

enum realize_command {
  cmd_generate_project,
  cmd_list_templates,
  cmd_print_usage,
  cmd_print_version,
};

typedef struct realize_stats_s {
  size_t template_files;
  size_t template_dirs;
  size_t files_added;
  size_t files_replaced;
  size_t dirs_added;
} realize_stats_t;

#define LOG_LEVEL_TRACE 2
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 0
#define LOG_LEVEL_QUIET -1
#define LOG_LEVEL_ERROR -2
#define LOG_LEVEL_USAGE -99

#define Y_OR_N(i) (i ? "Y" : "N")

typedef struct realize_options_s {
  enum realize_command command;
  // ~/project-templates by default
  char proj_tmpls_root_path[PATH_MAX - 10 - TEMPLATE_NAME_MAX];

  // ~/project-templates/template-c
  char proj_tmpl_path[PATH_MAX];

  // c or whatever
  char template_name[TEMPLATE_NAME_MAX];
  // test1 for example
  char project_name[PROJECT_NAME_MAX];

  int log_level;

  bool force;

  bool dryrun;

  struct winsize window_size;
  realize_stats_t stats;
} realize_options_t;

#endif
