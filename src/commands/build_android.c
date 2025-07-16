#include "commands/build_android.h"
#include "utils/logger.h"
#include "utils/config.h"
#include "utils/bpflut_const.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int build_android_handler(void) {
    log_step("Building Android application");
    
    // Check if Android config exists
    if (access(BPFLUT_ANDROID_CONFIG, F_OK) != 0) {
        log_error("Android config not found. Run 'bpflut init' first.");
        return -1;
    }
    // Load Android configuration
    Config config;
    if (load_config(BPFLUT_ANDROID_CONFIG, &config) != 0) {
        return -1;
    }
    
    // Generate build number
    time_t build_number = time(NULL);
    log_info("Generated build number: %ld", build_number);
    
    // Update pubspec.yaml with build number
    char cmd[512];
    snprintf(cmd, sizeof(cmd), 
        "perl -i -pe 's/^(version:\\s+\\d+\\.\\d+\\.)(\\d+)(\\+)(\\d+)$/${1}${2}${3}%ld/e' pubspec.yaml", 
        build_number);
    
    if (system(cmd) != 0) {
        log_warning("Failed to update build number in pubspec.yaml");
    }
    
    // Setup keystore signing
    log_step("Setting up Android keystore signing");
    
    // Create key.properties file for signing
    FILE* key_properties = fopen("android/key.properties", "w");
    if (key_properties) {
        fprintf(key_properties,
            "storePassword=%s\n"
            "keyPassword=%s\n"
            "keyAlias=%s\n"
            "storeFile=%s\n",
            config.keystore_password,
            config.key_password,
            config.key_alias,
            config.keystore_path);
        fclose(key_properties);
        log_info("Created key.properties for signing");
    }
    
    // Build Android App Bundle
    log_step("Building Android App Bundle");
    system("flutter pub get");
    
    if (system("flutter build appbundle --release") != 0) {
        log_error("Flutter Android build failed");
        return -1;
    }
    
    // Also build APK for testing
    log_step("Building APK for testing");
    if (system("flutter build apk --release") != 0) {
        log_warning("APK build failed, but AAB build succeeded");
    }
    
    log_success("Android build completed successfully");
    log_info("AAB file: build/app/outputs/bundle/release/app-release.aab");
    log_info("APK file: build/app/outputs/flutter-apk/app-release.apk");
    
    return 0;
}
