#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char distribution_cer_path[256];
    char distribution_cer_private_path[256];
    char mobileprovision_file_path[256];
    char signing_password[128];

    char api_private_key_path[256];
    char api_key_id[64];
    char issuer_id[128];
    
    // Android specific
    char keystore_path[256];
    char keystore_password[128];
    char key_alias[128];
    char key_password[128];
    char package_name[128];
    char service_account_json[256];
    char track[32];
} Config;

int load_config(const char* config_file, Config* config);

#endif
