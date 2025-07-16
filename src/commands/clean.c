#include "commands/clean.h"
#include "utils/logger.h"
#include "utils/bpflut_const.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int cmd_clean(void) {
    log_step("Cleanning workspace");
    // Nếu thư mục APP_SRC_DIR đã tồn tại thì xóa đi
    struct stat st_src = {0};
    if (stat(BPFLUT_WORKSPACE_DIR, &st_src) == 0) {
        char rm_cmd[1024];
        snprintf(rm_cmd, sizeof(rm_cmd), "rm -rf %s", BPFLUT_WORKSPACE_DIR);
        if (system(rm_cmd) != 0) {
            log_error("Failed to clean workspace: %s", BPFLUT_WORKSPACE_DIR);
            return -1;
        }
    }

    log_success("Project structure initialized successfully!");
    return 0;
}
