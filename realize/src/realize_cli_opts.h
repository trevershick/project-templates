#ifndef _CLI_OPTIONS_H
#define _CLI_OPTIONS_H

#include "realize_types.h"

#define DEFAULT_PROJ_TMPLS_ROOT_PATH "~/project-templates"

int validate_opts(const realize_options_t *);
void show_opts(const realize_options_t *);
void print_usage(int, char **);
int handle_opts(int, char **, realize_options_t *);
int load_opts_defaults(realize_options_t *);

#endif
