#ifndef __EZFS_H__
#define __EZFS_H__

#include <stdbool.h>

bool ez_is_dir(const char*);
bool ez_is_file(const char*);
bool ez_exists(const char*);

#endif
