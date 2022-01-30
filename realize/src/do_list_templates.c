#include "realize_defs.h"

#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define PATTERN "^template-([a-zA-Z0-9]+)$"

int do_list_templates(realize_options_t *opts) {
  // get the root project dir
  // list dirs in that dir that match template-x
  // return just the x part
  DIR *d = opendir(opts->proj_tmpls_root_path);
  if (d == NULL) {
    perror("opendir");
    return -1;
  }

  regex_t regex;
  regmatch_t pmatch[2];
  if (regcomp(&regex, PATTERN, REG_EXTENDED | REG_NEWLINE)) {
    perror("regcomp");
    return -1;
  }

  struct dirent *de;
  char template_name[NAME_MAX];

  while ((de = readdir(d)) != NULL) {
    if (regexec(&regex, de->d_name, ARRAY_SIZE(pmatch), pmatch, 0) ==
        REG_NOMATCH) {
      continue;
    }

    memset(template_name, 0, sizeof(template_name));
    strncpy(template_name, de->d_name + pmatch[1].rm_so,
            pmatch[1].rm_eo - pmatch[1].rm_so);

    printf("%s\n", template_name);
  }
  closedir(d);

  return 0;
}
