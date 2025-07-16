#ifndef LOGGER_H
#define LOGGER_H

// ANSI color codes
#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_RESET   "\033[0m"

// Logging functions
void log_info(const char* format, ...);
void log_warning(const char* format, ...);
void log_error(const char* format, ...);
void log_step(const char* format, ...);
void log_success(const char* format, ...);

#endif
