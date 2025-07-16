#include "commands/build.h"
#include "commands/build_ios.h"
#include "commands/build_android.h"
#include "utils/logger.h"
#include <stdio.h>
#include <string.h>

int cmd_build(const char* platform) {
    log_step("Starting build process for platform: %s", platform);
    
    if (strcmp(platform, "ios") == 0) {
        return build_ios_handler();
    } else if (strcmp(platform, "android") == 0) {
        return build_android_handler();
    } else if (strcmp(platform, "all") == 0) {
        log_info("Building for all platforms");
        int ios_result = build_ios_handler();
        int android_result = build_android_handler();
        
        if (ios_result == 0 && android_result == 0) {
            log_success("All builds completed successfully");
            return 0;
        } else {
            log_error("Some builds failed");
            return -1;
        }
    } else {
        log_error("Unknown platform: %s. Use 'ios', 'android', or 'all'", platform);
        return -1;
    }
}
