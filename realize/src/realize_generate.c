#include "realize_generate.h"

#include <assert.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ezfs.h"
#include "ezstring.h"
#include "realize_types.h"

#define TOKEN "PRJNAME"

char line_buffer[4096];
// add debug(bool, const char*, ... ) options

// this is disappointing
static const realize_options_t *g_opts = 0;

static int visit_path(const char *path, const struct stat *s, int typeflag) {
  // work out the destination path
  char destination_path[PATH_MAX];
  memset(destination_path, 0, sizeof(destination_path));
  memcpy(destination_path, path, strlen(path));
  // replace any tokens in the path and change it's relative location
  str_replace(destination_path, sizeof(destination_path), TOKEN,
              g_opts->project_name);
  str_replace(destination_path, sizeof(destination_path),
              g_opts->proj_tmpl_path, ".");

  // work out a human readable source path
  char b[PATH_MAX];
  memcpy(b, path, strlen(path));
  str_replace(b, sizeof(b), g_opts->proj_tmpls_root_path, "<root>");

  // print out what we're doing
  char type = ' ';
  char action = ' ';
  if (!ez_exists(destination_path)) {
    action = '+';
  }

  if (typeflag == FTW_D) {
    type = 'D';
    if (action == '+') {
      if (mkdir(destination_path, S_IRWXU| S_IRGRP | S_IXGRP | S_IROTH| S_IXOTH) == -1) {
        perror(destination_path);
        return -1;
      }
    }
  } else if (typeflag == FTW_F) {
    type = 'F';
    if (g_opts->force) {
      action = '+';
    }
    if (action == '+') {
      // create the file and filter its contents
      FILE *fw = fopen(destination_path, "w");
      if (fw == NULL) {
        perror(destination_path);
        return -1;
      }
      FILE *fr = fopen(path, "r");
      if (fr == NULL) {
        perror(path);
        return -1;
      }

      char* line = NULL;
      size_t len = 0;
      ssize_t read;
      while ((read = getline(&line, &len, fr)) != -1) {
        memset(line_buffer, 0, sizeof(line_buffer));
        memcpy(line_buffer, line, read);
        str_replace(line_buffer, sizeof(line_buffer), TOKEN,
                    g_opts->project_name);
        fwrite(line_buffer, strlen(line_buffer), 1, fw);
        line= NULL;
      }
      fclose(fw);
      fclose(fr);
    }
  }

  // screen geometry calc
  int sz = (g_opts->window_size.ws_col - 10) / 2;
  printf("%c %-*.*s -> %-*.*s  %c\n", type, sz, sz, b, sz, sz, destination_path,
         action);

  // directory?
  //   calculate name, check for directory in destination create if not there
  return 0;
};

int generate_project(const realize_options_t *opts) {
  assert(!g_opts);
  g_opts = opts;
  // iterate thru the template directory
  int ret = 0;
  ret = ftw(opts->proj_tmpl_path, visit_path, FTW_MOUNT | FTW_PHYS);
  if (ret == -1) {
    return -1;
  }

  //   create any directories needed
  //   rename files
  //   write the file to the new location replacing
  //      project name token
  g_opts = 0;
  return 0;
}
