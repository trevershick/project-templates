#ifndef __PRJNAME_TYPES_H__
#define __PRJNAME_TYPES_H__

#include <stdbool.h>

#define LOG_LEVEL_TRACE 2
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 0
#define LOG_LEVEL_QUIET -1
#define LOG_LEVEL_ERROR -2
#define LOG_LEVEL_USAGE -99

enum PRJNAME_command
{
    cmd_do_main_function,
    cmd_print_usage,
    cmd_print_version,
};

typedef struct PRJNAME_options_s
{
    enum PRJNAME_command command;
    int log_level;
    bool force;
    bool dryrun;
} PRJNAME_options_t;

#endif
