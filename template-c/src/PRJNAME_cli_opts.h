#ifndef __PRJNAME_CLI_OPTIONS_H__
#define __PRJNAME_CLI_OPTIONS_H__

#include "PRJNAME_types.h"

int validate_opts(const PRJNAME_options_t *);
void show_opts(const PRJNAME_options_t *);
void print_usage(int, char **);
int handle_opts(int, char **, PRJNAME_options_t *);
int load_opts_defaults(PRJNAME_options_t *);

#endif
