#ifndef __CLI_OPTIONS_H__
#define __CLI_OPTIONS_H__

#include "realize_types.h"

int validate_opts(const realize_options_t *);
void show_opts(const realize_options_t *);
void print_usage(int, char **);
int handle_opts(int, char **, realize_options_t *);
int load_opts_defaults(realize_options_t *);

#endif
