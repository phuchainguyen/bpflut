
#include "commands/init_android.h"
#include "utils/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/file_utils.h"
#include "utils/bpflut_const.h"

static const char* android_config_template = 
"# Android Build Configuration\n"
"GIT_REPO_URL=https://github.com/user/repo.git\n"
"KEYSTORE_PATH=./keystore/app-release-key.jks\n"
"KEYSTORE_PASSWORD=your_keystore_password\n"
"KEY_ALIAS=your_key_alias\n"
"KEY_PASSWORD=your_key_password\n"
"FLUTTER_VERSION=3.32.0\n"
"PROJECT_NAME=your_project_name\n"
"PACKAGE_NAME=com.example.app\n"
"# Google Play Console Service Account JSON\n"
"SERVICE_ACCOUNT_JSON=./keys/service-account.json\n"
"TRACK=internal\n";



int init_android_handler(void) {
    log_step("Initializing Android configuration");
    char keystore_dir[512], keys_dir[512];
    snprintf(keystore_dir, sizeof(keystore_dir), "%s/keystore", BPFLUT_ANDROID_DIR);
    snprintf(keys_dir, sizeof(keys_dir), "%s/keys", BPFLUT_ANDROID_DIR);

    if (create_directory(BPFLUT_ANDROID_DIR) != 0) return -1;
    if (create_directory(keystore_dir) != 0) return -1;
    if (create_directory(keys_dir) != 0) return -1;
    if (write_config_file(BPFLUT_ANDROID_CONFIG, android_config_template) != 0) return -1;

    log_success("Android initialization completed in %s", BPFLUT_ANDROID_DIR);
    return 0;
}
