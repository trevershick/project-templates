#include "ezstring.h"

#include <stdlib.h>
#include <string.h>

void ensmallen(char *buffer, size_t sz, const char *str) {
  int l = strlen(str);
  if (l > sz) {
    memcpy(buffer, str, l / 2 - 2);
    memset(buffer + (l / 2 - 2), '.', 4);
    memcpy(buffer + (l / 2 + 2), str + (l / 2 - 2), l / 2 - 2);
  } else {
    memcpy(buffer, str, l);
  }
}

void str_replace(char *target, size_t target_sz, const char *needle,
                 const char *replacement) {
  size_t needle_len = strlen(needle);
  size_t repl_len = strlen(replacement);

  char *buffer = calloc(target_sz, sizeof(char));
  char *insert_point = &buffer[0];
  const char *tmp = target;

  while (1) {
    const char *p = strstr(tmp, needle);

    // walked past last occurrence of needle; copy remaining part
    if (p == NULL) {
      strcpy(insert_point, tmp);
      break;
    }

    // copy part before needle
    memcpy(insert_point, tmp, p - tmp);
    insert_point += p - tmp;

    // copy replacement string
    memcpy(insert_point, replacement, repl_len);
    insert_point += repl_len;

    // adjust pointers, move on
    tmp = p + needle_len;
  }

  // write altered string back to target
  strncpy(target, buffer, target_sz);
  free(buffer);
}
