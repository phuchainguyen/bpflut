#include "utils/logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

static void log_with_color(const char* color, const char* level, const char* format, va_list args) {
    time_t now;
    struct tm *tm_info;
    char timestamp[20];
    
    time(&now);
    tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", tm_info);
    
    printf("%s[%s] [%s]%s ", color, timestamp, level, COLOR_RESET);
    vprintf(format, args);
    printf("\n");
    fflush(stdout);
}

void log_info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_with_color(COLOR_GREEN, "INFO", format, args);
    va_end(args);
}

void log_warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_with_color(COLOR_YELLOW, "WARNING", format, args);
    va_end(args);
}

void log_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_with_color(COLOR_RED, "ERROR", format, args);
    va_end(args);
}

void log_step(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_with_color(COLOR_BLUE, "STEP", format, args);
    va_end(args);
}

void log_success(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log_with_color(COLOR_GREEN, "SUCCESS", format, args);
    va_end(args);
}
