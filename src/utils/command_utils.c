#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_BUFFER_SIZE 1024
#define OUTPUT_INITIAL_SIZE 4096

char* run_command(const char* command) {
    FILE* fp;
    char buffer[CMD_BUFFER_SIZE];
    size_t output_size = OUTPUT_INITIAL_SIZE;
    size_t len = 0;

    // Cấp phát bộ nhớ ban đầu cho output
    char* output = (char*)malloc(output_size);
    if (!output) {
        perror("malloc failed");
        return NULL;
    }
    output[0] = '\0';

    // Mở pipe để đọc output từ lệnh
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen failed");
        free(output);
        return NULL;
    }

    // Đọc từng dòng từ pipe
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t buf_len = strlen(buffer);

        // Mở rộng vùng nhớ nếu cần
        if (len + buf_len + 1 > output_size) {
            output_size *= 2;
            char* new_output = (char*)realloc(output, output_size);
            if (!new_output) {
                perror("realloc failed");
                free(output);
                pclose(fp);
                return NULL;
            }
            output = new_output;
        }

        // Nối buffer vào output
        strcpy(output + len, buffer);
        len += buf_len;
    }

    pclose(fp);
    return output; // Đừng quên free sau khi dùng
}

char* run_command_static(const char* command) {
    static char output[8192];  // bộ nhớ tĩnh
    FILE* fp;
    char buffer[256];
    output[0] = '\0';

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen failed");
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strlen(output) + strlen(buffer) < sizeof(output)) {
            strcat(output, buffer);
        } else {
            // Output bị cắt nếu quá dài
            break;
        }
    }

    pclose(fp);
    return output;  // không cần free()
}