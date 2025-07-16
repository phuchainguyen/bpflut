#include "utils/file_utils.h"
#include "utils/logger.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

int create_directory(const char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        if (MKDIR(path) == -1) {
            log_error("Failed to create directory: %s", path);
            return -1;
        }
    }
    return 0;
}

int write_config_file(const char* path, const char* content) {
    FILE* file = fopen(path, "r");
    if (file) {
        fclose(file);
        log_info("Config file already exists, skipping: %s", path);
        return 0;
    }
    file = fopen(path, "w");
    if (!file) {
        log_error("Failed to create config file: %s", path);
        return -1;
    }
    fprintf(file, "%s", content);
    fclose(file);
    log_info("Created config file: %s", path);
    return 0;
}
