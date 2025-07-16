#include "commands/publish.h"
#include "commands/publish_ios.h"
#include "commands/publish_android.h"
#include "utils/logger.h"
#include <stdio.h>
#include <string.h>

int cmd_publish(const char* platform) {
    log_step("Starting publish process for platform: %s", platform);
    
    if (strcmp(platform, "ios") == 0) {
        return publish_ios_handler();
    } else if (strcmp(platform, "android") == 0) {
        return publish_android_handler();
    } else if (strcmp(platform, "all") == 0) {
        log_info("Publishing for all platforms");
        int ios_result = publish_ios_handler();
        int android_result = publish_android_handler();
        
        if (ios_result == 0 && android_result == 0) {
            log_success("All publishes completed successfully");
            return 0;
        } else {
            log_error("Some publishes failed");
            return -1;
        }
    } else {
        log_error("Unknown platform: %s. Use 'ios', 'android', or 'all'", platform);
        return -1;
    }
}
