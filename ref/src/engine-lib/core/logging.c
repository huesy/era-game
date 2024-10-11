#include "engine/logging.h"
#include <stdarg.h>
#include <stdio.h>

void log_message(LogLevel level, const char *format, ...) {
    const char *level_str;
    switch (level) {
    case LOG_LEVEL_FATAL:
        level_str = "FATAL";
        break;
    case LOG_LEVEL_ERROR:
        level_str = "ERROR";
        break;
    case LOG_LEVEL_WARNING:
        level_str = "WARNING";
        break;
    case LOG_LEVEL_INFO:
        level_str = "INFO";
        break;
    case LOG_LEVEL_DEBUG:
        level_str = "DEBUG";
        break;
    default:
        level_str = "UNKNOWN";
        break;
    }

    printf("[%s]: ", level_str);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}
