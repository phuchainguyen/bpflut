
#include "commands/init_ios.h"
#include "utils/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/file_utils.h"
#include "utils/bpflut_const.h"

static const char* ios_config_template = 
"# iOS Build Configuration\n"
"DISTRIBUTION_CER_PRIVATE_FILE_PATH=certificates/distribution_private.key\n"
"DISTRIBUTION_CER_FILE_PATH=certificates/distribution.cer\n"
"MOBILEPROVISION_FILE_PATH=certificates/distribution.mobileprovision\n"
"SIGNING_PASSWORD=1qazZAQ!\n"
"\n"
"API_PRIVATE_KEY_PATH=./keys/AuthKey_XXXXXXXXXX.p8\n"
"API_KEY_ID=XXXXXXXXXX\n"
"ISSUER_ID=xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx\n"
"FLUTTER_VERSION=3.32.0\n"
"PROJECT_NAME=your_project_name\n"
"BUNDLE_ID=com.example.app\n"
"TEAM_ID=XXXXXXXXXX\n";



int init_ios_handler(void) {
    log_step("Initializing iOS configuration in");
    char cert_dir[512], keys_dir[512];
    snprintf(cert_dir, sizeof(cert_dir), "%s/certificates", BPFLUT_IOS_DIR);
    snprintf(keys_dir, sizeof(keys_dir), "%s/keys", BPFLUT_IOS_DIR);

    if (create_directory(BPFLUT_IOS_DIR) != 0) return -1;
    if (create_directory(cert_dir) != 0) return -1;
    if (create_directory(keys_dir) != 0) return -1;
    if (write_config_file(BPFLUT_IOS_CONFIG, ios_config_template) != 0) return -1;

    log_success("iOS initialization completed in %s", BPFLUT_IOS_DIR);
    return 0;
}
