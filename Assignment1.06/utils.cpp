#include <sys/stat.h>
#include <sys/types.h>

#include <cstring>
#include <cerrno>
#include <cstdio>

#include "utils.h"

int makedirectory(char *dir)
{
  char *slash;

  for (slash = dir + std::strlen(dir); slash > dir && *slash != '/'; slash--)
    ;

  if (slash == dir) {
    return 0;
  }

  if (mkdir(dir, 0700)) {
    if (errno != ENOENT && errno != EEXIST) {
      fprintf(stderr, "mkdir(%s): %s\n", dir, strerror(errno));
      return 1;
    }
    
    if (*slash != '/') {
      return 1;
    }

    *slash = '\0';

    if (makedirectory(dir)) {
      *slash = '/';
      return 1;
    }

    *slash = '/';

    if (mkdir(dir, 0700) && errno != EEXIST) {
      fprintf(stderr, "mkdir(%s): %s\n", dir, strerror(errno));
      return 1;
    }
  }

  return 0;
}
