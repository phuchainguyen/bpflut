#include "commands/publish_ios.h"
#include "commands/build_ios.h"
#include "utils/logger.h"
#include "utils/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glob.h>
#include "utils/bpflut_const.h"

static int check_ios_build_exists() {
    glob_t glob_result;
    int result = glob("build/ios/ipa/*.ipa", GLOB_NOSORT, NULL, &glob_result);
    
    if (result == 0 && glob_result.gl_pathc > 0) {
        globfree(&glob_result);
        return 1; // Build exists
    }
    
    globfree(&glob_result);
    return 0; // Build doesn't exist
}

int publish_ios_handler(void) {
    log_step("Publishing iOS application to App Store");
    
    // Check if build exists, if not, build it
    if (!check_ios_build_exists()) {
        log_info("iOS build not found, building first...");
        if (build_ios_handler() != 0) {
            return -1;
        }
    }
    
    // Load iOS configuration
    Config config;
    if (load_config(BPFLUT_IOS_CONFIG, &config) != 0) {
        return -1;
    }
    
    // Install API key
    log_step("Installing App Store Connect API key");
    system("mkdir -p ~/private_keys");
    
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "cp '%s' ~/private_keys/AuthKey_%s.p8", 
        config.api_private_key_path, config.api_key_id);
    system(cmd);
    
    // Find IPA file
    glob_t glob_result;
    if (glob("build/ios/ipa/*.ipa", GLOB_NOSORT, NULL, &glob_result) != 0 || glob_result.gl_pathc == 0) {
        log_error("IPA file not found");
        globfree(&glob_result);
        return -1;
    }
    
    const char* ipa_path = glob_result.gl_pathv[0];
    log_info("Found IPA: %s", ipa_path);
    
    // Upload to App Store
    log_step("Uploading to App Store Connect");
    snprintf(cmd, sizeof(cmd), 
        "xcrun altool --upload-app --type ios -f '%s' --apiKey '%s' --apiIssuer '%s'", 
        ipa_path, config.api_key_id, config.issuer_id);
    
    int result = system(cmd);
    globfree(&glob_result);
    
    if (result == 0) {
        log_success("iOS app uploaded to App Store successfully!");
        return 0;
    } else {
        log_error("Failed to upload iOS app to App Store");
        return -1;
    }
}
