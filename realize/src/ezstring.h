#ifndef __EZSTRING_H__
#define __EZSTRING_H__

#include <string.h>

void str_replace(char *target, size_t target_sz, const char *needle,
                 const char *replacement);
void ensmallen(char *buffer, size_t sz, const char *str);

#endif
