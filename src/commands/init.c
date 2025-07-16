#include "commands/init.h"
#include "commands/init_ios.h"
#include "commands/init_android.h"
#include "utils/logger.h"
#include "utils/file_utils.h"
#include "utils/bpflut_const.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int cmd_init(const char *repository) {
    log_step("Cloning repository: %s", repository);
    // Nếu thư mục APP_SRC_DIR đã tồn tại thì xóa đi
    struct stat st_src = {0};
    if (stat(BPFLUT_WORKSPACE_DIR, &st_src) == 0) {
        char rm_cmd[1024];
        snprintf(rm_cmd, sizeof(rm_cmd), "rm -rf %s", BPFLUT_WORKSPACE_DIR);
        if (system(rm_cmd) != 0) {
            log_error("Failed to remove directory: %s", BPFLUT_WORKSPACE_DIR);
            return -1;
        }
    }

    char git_cmd[1024];
    snprintf(git_cmd, sizeof(git_cmd), "git clone %s %s > /dev/null 2>&1", repository, BPFLUT_WORKSPACE_DIR);
    int git_result = system(git_cmd);
    if (git_result != 0) {
        log_error("Failed to clone repository");
        return -1;
    }

    // Tạo thư mục _bpflut nếu chưa có
    struct stat st = {0};
    if (stat(BPFLUT_DIR, &st) == -1) {
        if (create_directory(BPFLUT_DIR) != 0) {
            log_error("Failed to create bpflut asset directory: %s", BPFLUT_DIR);
            return -1;
        }
    }

    // Gọi init ios/android với base path _bpflut
    int ios_result = init_ios_handler();
    int android_result = init_android_handler();

    if (ios_result == 0 && android_result == 0) {
        log_success("Project structure initialized successfully!");
        log_info("Please edit the config files:");
        log_info("  - %s", BPFLUT_IOS_CONFIG);
        log_info("  - %s", BPFLUT_ANDROID_CONFIG);
        return 0;
    } else {
        log_error("Failed to initialize some platforms");
        return -1;
    }
}
