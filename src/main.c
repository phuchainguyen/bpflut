#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/logger.h"
#include "commands/init.h"
#include "commands/build.h"
#include "commands/publish.h"
#include "commands/clean.h"

void show_usage(const char* program_name) {
    printf("Usage: %s <command> [options]\n\n", program_name);
    printf("Commands:\n");
    printf("  init <repository_url>                   Initialize project structure\n");
    printf("  build [android|ios|all]                 Build for platform (default: all)\n");
    printf("  publish [android|ios|all]               Publish to stores (default: all)\n");
    printf("  clean                                   Publish to stores (default: all)\n");
    printf("\nOptions:\n");
    printf("  -h, --help                              Show this help message\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        log_error("No command specified");
        show_usage(argv[0]);
        return 1;
    }

    const char* command = argv[1];

    if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        show_usage(argv[0]);
        return 0;
    }

    if (strcmp(command, "init") == 0) {
        if (argc > 2) {
            const char* repository = argv[2];
            return cmd_init(repository);
        } else {
            log_error("Invalid option command", command);
            show_usage(argv[0]);
            return 1;
        }
    } else if (strcmp(command, "build") == 0) {
        const char* platform = (argc > 2) ? argv[2] : "all";
        return cmd_build(platform);
    } else if (strcmp(command, "publish") == 0) {
        const char* platform = (argc > 2) ? argv[2] : "all";
        return cmd_publish(platform);
    } else if (strcmp(command, "clean") == 0) {
        return cmd_clean();
    } else {
        log_error("Unknown command: %s", command);
        show_usage(argv[0]);
        return 1;
    }
}
