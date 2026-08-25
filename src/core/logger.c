#include "core/logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static const char* log_level_to_string(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_FATAL: return "FATAL";
        default:              return "UNKNOWN";
    }
}

void logger_log(LogLevel level, const char* file, int line, const char* format, ...) {
    FILE* log_file = fopen("debug.log", "a");
    if (!log_file) {
        return;
    }

    time_t raw_time = time(NULL);
    struct tm* time_info = localtime(&raw_time);

    char time_str[32];
    if (time_info) {
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
    } else {
        snprintf(time_str, sizeof(time_str), "0000-00-00 00:00:00");
    }

    fprintf(log_file, "[%s] [%s] [%s:%d] ", time_str, log_level_to_string(level), file, line);

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fclose(log_file);
}