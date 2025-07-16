#include "commands/publish_android.h"
#include "commands/build_android.h"
#include "utils/logger.h"
#include "utils/config.h"
#include "utils/bpflut_const.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int check_android_build_exists() {
    return access("build/app/outputs/bundle/release/app-release.aab", F_OK) == 0;
}

int publish_android_handler(void) {
    log_step("Publishing Android application to Google Play");
    
    // Check if build exists, if not, build it
    if (!check_android_build_exists()) {
        log_info("Android build not found, building first...");
        if (build_android_handler() != 0) {
            return -1;
        }
    }
    
    // Load Android configuration
    Config config;
    if (load_config(BPFLUT_ANDROID_CONFIG, &config) != 0) {
        return -1;
    }
    
    // Check if bundletool is available
    if (system("which bundletool > /dev/null 2>&1") != 0) {
        log_error("bundletool not found. Please install bundletool for Google Play publishing.");
        log_info("You can install it via: brew install bundletool");
        return -1;
    }
    
    // Check if required Python packages are available
    if (system("python3 -c 'import google.oauth2.service_account, googleapiclient.discovery' 2>/dev/null") != 0) {
        log_error("Required Python packages not found.");
        log_info("Please install: pip install google-auth google-auth-oauthlib google-auth-httplib2 google-api-python-client");
        return -1;
    }
    
    // Upload to Google Play using Google Play Console API
    log_step("Uploading to Google Play Console");
    
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), 
        "python3 -c \""
        "import subprocess, json, sys; "
        "from google.oauth2 import service_account; "
        "from googleapiclient.discovery import build; "
        "from googleapiclient.http import MediaFileUpload; "
        "try: "
        "    credentials = service_account.Credentials.from_service_account_file('%s', scopes=['https://www.googleapis.com/auth/androidpublisher']); "
        "    service = build('androidpublisher', 'v3', credentials=credentials); "
        "    package_name = '%s'; "
        "    aab_file = 'build/app/outputs/bundle/release/app-release.aab'; "
        "    edit_request = service.edits().insert(body={}, packageName=package_name); "
        "    edit_result = edit_request.execute(); "
        "    edit_id = edit_result['id']; "
        "    print(f'Created edit: {edit_id}'); "
        "    bundle_response = service.edits().bundles().upload(editId=edit_id, packageName=package_name, media_body=MediaFileUpload(aab_file)).execute(); "
        "    print(f'Uploaded bundle with version code: {bundle_response[\\\"versionCode\\\"]}'); "
        "    track_response = service.edits().tracks().update(editId=edit_id, track='%s', packageName=package_name, body={'releases': [{'versionCodes': [bundle_response['versionCode']], 'status': 'completed'}]}).execute(); "
        "    print(f'Updated track: %s'); "
        "    commit_request = service.edits().commit(editId=edit_id, packageName=package_name).execute(); "
        "    print('Upload successful!') "
        "except Exception as e: "
        "    print(f'Upload failed: {e}'); "
        "    sys.exit(1)"
        "\"", 
        config.service_account_json, config.package_name, config.track, config.track);
    
    int result = system(cmd);
    
    if (result == 0) {
        log_success("Android app uploaded to Google Play successfully!");
        log_info("Track: %s", config.track);
        return 0;
    } else {
        log_error("Failed to upload Android app to Google Play");
        return -1;
    }
}
