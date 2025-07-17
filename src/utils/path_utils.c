#include <string.h>
#include <stdio.h>
#include <libgen.h>
#if defined(_WIN32) && !defined(PATH_MAX)
#define PATH_MAX 260
#else
#include <limits.h>
#endif

void make_abs_path(char* field, const char* dir) {
    if (field[0] != '/' && !(field[1] == ':' && field[2] == '\\')) {
        char abs_path[PATH_MAX];
        snprintf(abs_path, sizeof(abs_path), "%s/%s", dir, field);
        strncpy(field, abs_path, PATH_MAX-1);
        field[PATH_MAX-1] = '\0';
    }
}
