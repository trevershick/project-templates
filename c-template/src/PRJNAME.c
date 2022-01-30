#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cb(const char *fpath, const struct stat *sb, int typeflag) {
  char tbuf[20];
  memset(tbuf, 0, sizeof(tbuf));

  switch (typeflag) {
  case FTW_F:
    strcpy(tbuf, "f");
    break;
  case FTW_D:
    strcpy(tbuf, "d");
    break;
  default:
    strcpy(tbuf, "?");
    break;
  }
  printf("%-50s %-4s %ld\n", fpath, tbuf, sb->st_size);
  return 0;
}

int main(void) {
  int ret = ftw(".", cb, 0);
  exit(ret);
}
