#include <string.h>
#include <stdio.h>
#include <libgen.h>

#include <limits.h>


void make_abs_path(char* field, const char* dir) {
    if (field[0] == '\0') return;
    if (field[0] != '/' && !(field[1] == ':' && field[2] == '\\')) {
        char abs_path[256];
        snprintf(abs_path, sizeof(abs_path), "%s/%s", dir, field);
        strncpy(field, abs_path, 256-1);
        field[256-1] = '\0';
    }
}
