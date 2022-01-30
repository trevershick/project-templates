#include "ezfs.h"

#include <sys/stat.h>

bool ez_directory_exists(const char* p) {
  struct stat s;
  return (stat(p, &s) == 0 && S_ISDIR(s.st_mode));
}
