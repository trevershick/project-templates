#include "ezfs.h"

#include <sys/stat.h>
#include <unistd.h>

bool ez_exists(const char *p)
{
    return (access(p, F_OK) == 0);
}

bool ez_is_dir(const char *p)
{
    struct stat s;
    return (stat(p, &s) == 0 && S_ISDIR(s.st_mode));
}

bool ez_is_file(const char *p)
{
    struct stat s;
    return (stat(p, &s) == 0 && S_ISREG(s.st_mode));
}
