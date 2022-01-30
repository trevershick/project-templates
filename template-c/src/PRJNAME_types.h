#ifndef __PRJNAME_TYPES_H__
#define __PRJNAME_TYPES_H__

#include <stdbool.h>

enum PRJNAME_command {
  cmd_do_main_function,
  cmd_print_usage,
  cmd_print_version,
};

typedef struct PRJNAME_options_s {
  enum PRJNAME_command command;
  bool debug;
} PRJNAME_options_t;

#endif
