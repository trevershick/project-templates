#include "realize_defs.h"
#include <string.h>

int load_opts_defaults(realize_options_t *opts) {
  strncpy(opts->proj_tmpls_root_path, DEFAULT_PROJ_TMPLS_ROOT_PATH,
          sizeof(opts->proj_tmpls_root_path));
  ioctl(1, TIOCGWINSZ, &opts->window_size);
  strcpy(opts->project_name, NOT_SET_STRING);
  strcpy(opts->template_name, NOT_SET_STRING);
  return 0;
}
