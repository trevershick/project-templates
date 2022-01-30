#ifndef __EZREGEX_H__
#define __EZREGEX_H__

#define EZREGEX_BAD -1
#define EZREGEX_NOMATCH 0
#define EZREGEX_MATCH 1

int ez_matches(const char *regex, const char *str);

#endif
