#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils/path_utils.h"
#include "utils/config.h"
#include "utils/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trim_whitespace(char* str) {
    char* end;
    
    // Trim leading space
    while (*str == ' ' || *str == '\t') str++;
    
    // All spaces?
    if (*str == 0) return;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;
    
    // Write new null terminator
    *(end + 1) = 0;
}

static void parse_config_line(const char* line, Config* config) {
    char key[128], value[512];
    
    if (sscanf(line, "%127[^=]=%511[^\n]", key, value) == 2) {
        trim_whitespace(key);
        trim_whitespace(value);
        if (strcmp(key, "DISTRIBUTION_CER_PRIVATE_FILE_PATH") == 0) {
            strcpy(config->distribution_cer_private_path, value);
        } else if (strcmp(key, "DISTRIBUTION_CER_FILE_PATH") == 0) {
            strcpy(config->distribution_cer_path, value);
        } else if (strcmp(key, "MOBILEPROVISION_FILE_PATH") == 0) {
            strcpy(config->mobileprovision_file_path, value);
        } else if (strcmp(key, "SIGNING_PASSWORD") == 0) {
            strcpy(config->signing_password, value);
        } else if (strcmp(key, "API_PRIVATE_KEY_PATH") == 0) {
            strcpy(config->api_private_key_path, value);
        } else if (strcmp(key, "API_KEY_ID") == 0) {
            strcpy(config->api_key_id, value);
        } else if (strcmp(key, "ISSUER_ID") == 0) {
            strcpy(config->issuer_id, value);
        } else if (strcmp(key, "KEYSTORE_PATH") == 0) {
            strcpy(config->keystore_path, value);
        } else if (strcmp(key, "KEYSTORE_PASSWORD") == 0) {
            strcpy(config->keystore_password, value);
        } else if (strcmp(key, "KEY_ALIAS") == 0) {
            strcpy(config->key_alias, value);
        } else if (strcmp(key, "KEY_PASSWORD") == 0) {
            strcpy(config->key_password, value);
        } else if (strcmp(key, "PACKAGE_NAME") == 0) {
            strcpy(config->package_name, value);
        } else if (strcmp(key, "SERVICE_ACCOUNT_JSON") == 0) {
            strcpy(config->service_account_json, value);
        } else if (strcmp(key, "TRACK") == 0) {
            strcpy(config->track, value);
        }
    }
}

int load_config(const char* config_file, Config* config) {
    FILE* file = fopen(config_file, "r");
    if (!file) {
        log_error("Failed to open config file: %s", config_file);
        return -1;
    }
    
    // Initialize config with defaults
    memset(config, 0, sizeof(Config));
    strcpy(config->track, "internal");
    
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }
        
        parse_config_line(line, config);
    }
    
    fclose(file);
    // Chuyển các trường PATH sang tuyệt đối nếu là path tương đối
    char config_dir[256];
    strncpy(config_dir, config_file, 256-1);
    config_dir[256-1] = '\0';
    char* dir = dirname(config_dir);
    char abs_path[256];

    snprintf(abs_path, sizeof(abs_path), "%s/%s", dir, config->distribution_cer_private_path);
    strncpy(config->distribution_cer_private_path, abs_path, 256-1);
    // make_abs_path(config->distribution_cer_private_path, dir);

    snprintf(abs_path, sizeof(abs_path), "%s/%s", dir, config->distribution_cer_path);
    strncpy(config->distribution_cer_path, abs_path, 256-1);
    // make_abs_path(config->distribution_cer_path, dir);

    snprintf(abs_path, sizeof(abs_path), "%s/%s", dir, config->mobileprovision_file_path);
    strncpy(config->mobileprovision_file_path, abs_path, 256-1);
    // make_abs_path(config->mobileprovision_file_path, dir);

    snprintf(abs_path, sizeof(abs_path), "%s/%s", dir, config->api_private_key_path);
    strncpy(config->api_private_key_path, abs_path, 256-1);
    // make_abs_path(config->api_private_key_path, dir);

    snprintf(abs_path, sizeof(abs_path), "%s/%s", dir, config->keystore_path);
    strncpy(config->keystore_path, abs_path, 256-1);
    // make_abs_path(config->keystore_path, dir);

    snprintf(abs_path, sizeof(abs_path), "%s/%s", dir, config->service_account_json);
    strncpy(config->service_account_json, abs_path, 256-1);
    // make_abs_path(config->service_account_json, dir);
    log_info("Configuration loaded from: %s", config_file);

    return 0;
}
