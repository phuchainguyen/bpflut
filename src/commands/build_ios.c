#include "commands/build_ios.h"
#include "utils/logger.h"
#include "utils/config.h"
#include "utils/bpflut_const.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int build_ios_handler(void) {
    log_step("Building iOS application");
    
    // Check if iOS config exists
    if (access(BPFLUT_IOS_CONFIG, F_OK) != 0) {
        log_error("iOS config not found. Run 'bpflut init' first.");
        return -1;
    }
    // Load iOS configuration
    Config config;
    if (load_config(BPFLUT_IOS_CONFIG, &config) != 0) {
        return -1;
    }
    
    char cmd[512];
    
    // Setup certificates and provisioning profile
    log_step("Setting up certificates and provisioning profile");
    
    // Create temporary keychain
    char keychain_path[256];
    snprintf(keychain_path, sizeof(keychain_path), "/tmp/bpflut-keychain.keychain-db");
    
    snprintf(cmd, sizeof(cmd), 
        "security create-keychain -p '%s' '%s'", 
        config.signing_password, keychain_path);
    system(cmd);
    
    snprintf(cmd, sizeof(cmd), 
        "security set-keychain-settings -lut 21600 '%s'", keychain_path);
    system(cmd);
    
    snprintf(cmd, sizeof(cmd), 
        "security unlock-keychain -p '%s' '%s'", 
        config.signing_password, keychain_path);
    system(cmd);
    
    // Export p12 from cer
    char pem_file_path[512];
    char p12_file_path[512];
    int cer_extension_length = (int)(strstr(config.distribution_cer_path, ".cer") - config.distribution_cer_path);
    snprintf(pem_file_path, sizeof(pem_file_path), "%.*s.pem", cer_extension_length, config.distribution_cer_path);
    snprintf(p12_file_path, sizeof(p12_file_path), "%.*s.p12", cer_extension_length, config.distribution_cer_path);

    snprintf(cmd, sizeof(cmd), 
        "openssl x509 -inform der -in %s -out %s", 
        config.distribution_cer_private_path, pem_file_path);
    log_info(cmd);
    system(cmd);

    snprintf(cmd, sizeof(cmd), 
        "openssl pkcs12 -export -out %s -inkey %s -in %s -passout pass:\"%s\"",
        p12_file_path, config.distribution_cer_private_path, pem_file_path, config.signing_password);
    log_info(cmd);
    system(cmd);

    // Import certificate
    snprintf(cmd, sizeof(cmd), 
        "security import '%s' -P '%s' -A -t cert -f pkcs12 -k '%s'", 
        p12_file_path, config.signing_password, keychain_path);
    log_info(cmd);
    system(cmd);
    
    snprintf(cmd, sizeof(cmd), 
        "security list-keychain -d user -s '%s'", keychain_path);
    log_info(cmd);
    system(cmd);
    
    // Install provisioning profile
    system("mkdir -p ~/Library/MobileDevice/Provisioning\\ Profiles");
    snprintf(cmd, sizeof(cmd), 
        "cp '%s' ~/Library/MobileDevice/Provisioning\\ Profiles/", 
        config.mobileprovision_file_path);
    system(cmd);
    
    // Create ExportOptions.plist
    log_step("Creating ExportOptions.plist");
    snprintf(cmd, sizeof(cmd), "mkdir -p %s/Runner", BPFLUT_IOS_RUNNER_DIR);
    system(cmd);
    
    FILE* export_options = fopen(BPFLUT_IOS_EXPORTOPTIONS_PATH, "w");
    if (export_options) {
        fprintf(export_options, 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\">\n"
            "<dict>\n"
            "    <key>method</key>\n"
            "    <string>app-store</string>\n"
            "    <key>teamID</key>\n"
            "    <string>%s</string>\n"
            "    <key>signingStyle</key>\n"
            "    <string>manual</string>\n"
            "    <key>uploadBitcode</key>\n"
            "    <false/>\n"
            "    <key>uploadSymbols</key>\n"
            "    <true/>\n"
            "    <key>destination</key>\n"
            "    <string>upload</string>\n"
            "</dict>\n"
            "</plist>\n", config.team_id);
        fclose(export_options);
    }
    
    // Build IPA
    log_step("Building IPA");
    system("flutter pub get");
    
    snprintf(cmd, sizeof(cmd), 
        "cd %s && flutter build ipa --release --export-options-plist=%s", 
        BPFLUT_WORKSPACE_DIR, BPFLUT_IOS_EXPORTOPTIONS);
    if (system(cmd) != 0) {
        log_error("Flutter iOS build failed");
        // Cleanup
        snprintf(cmd, sizeof(cmd), "security delete-keychain '%s'", keychain_path);
        system(cmd);
        return -1;
    }
    
    // Cleanup keychain
    snprintf(cmd, sizeof(cmd), "security delete-keychain '%s'", keychain_path);
    system(cmd);
    
    log_success("iOS build completed successfully");
    return 0;
}
