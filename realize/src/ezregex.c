#include "ezregex.h"

#include <regex.h>

#include "realize_defs.h"

int ez_matches(const char *re, const char *str)
{
    regex_t regex;
    regmatch_t pmatch[1];
    if (regcomp(&regex, re, REG_EXTENDED | REG_NEWLINE))
        return EZREGEX_BAD;
    if (regexec(&regex, str, ARRAY_SIZE(pmatch), pmatch, 0) == REG_NOMATCH)
    {
        return EZREGEX_NOMATCH;
    }
    return EZREGEX_MATCH;
}
