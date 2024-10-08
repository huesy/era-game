#ifndef ENGINE_LOGGING_H
#define ENGINE_LOGGING_H

#include "defines.h"

#define LOG_INFO_ENABLED 1
#define LOG_WARNING_ENABLED 1
#define LOG_ERROR_ENABLED 1
#define LOG_FATAL_ENABLED 1

#if _RELEASE == 1
#    define LOG_DEBUG_ENABLED 0
#    define LOG_TRACE_ENABLED 0
#else
#    define LOG_DEBUG_ENABLED 1
#    define LOG_TRACE_ENABLED 1
#endif

typedef enum {
    /** @brief A fatal error has occurred. Should be used to stop the application when hit. */
    LOG_LEVEL_FATAL = 0,
    /** @brief An error has occurred. Should be used to indicate critical runtime problems that cause the application to run improperly or not at all. */
    LOG_LEVEL_ERROR = 1,
    /** @brief A warning has occurred. Should be used to indicate potential problems that could cause the application to run improperly. */
    LOG_LEVEL_WARNING = 2,
    /** @brief An informational message. Should be used to indicate general information. */
    LOG_LEVEL_INFO = 3,
    /** @brief A debug message. Should be used for detailed debugging information. */
    LOG_LEVEL_DEBUG = 4,
    /** @brief A trace message. Should be used for very detailed debugging information. */
    LOG_LEVEL_TRACE = 5,
} LogLevel;

/**
 * @brief Outputs a log message at the specified level.
 *
 * @param level The level to log the message at.
 * @param format The format string for the message.
 * @param ... Any formatted data to be included in the message.
 */
void log_message(LogLevel level, const char *format, ...);

#if LOG_FATAL_ENABLED == 1
/**
 * @brief Logs a fatal error message. Should be used to stop the application when hit.
 *
 * @param message The message to log.
 * @param ... The format string and any formatted data to be included in the message.
 */
#    define log_fatal(message, ...) log_message(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#else
#    define log_fatal(message, ...)
#endif

#if LOG_ERROR_ENABLED == 1
/**
 * @brief Logs an error message. Should be used to indicate critical runtime problems that cause the application to run improperly or not at all.
 *
 * @param message The message to log.
 * @param ... The format string and any formatted data to be included in the message.
 */
#    define log_error(message, ...) log_message(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#else
#    define log_error(message, ...)
#endif

#if LOG_WARNING_ENABLED == 1
/**
 * @brief Logs a warning message. Should be used to indicate potential problems that could cause the application to run improperly.
 *
 * @param message The message to log.
 * @param ... The format string and any formatted data to be included in the message.
 */
#    define log_warning(message, ...) log_message(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#else
#    define log_warning(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
/**
 * @brief Logs an informational message. Should be used to indicate general information.
 *
 * @param message The message to log.
 * @param ... The format string and any formatted data to be included in the message.
 */
#    define log_info(message, ...) log_message(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
#    define log_info(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
/**
 * @brief Logs a debug-level message. Should be used for detailed debugging information.
 *
 * @param message The message to log.
 * @param ... The format string and any formatted data to be included in the message.
 */
#    define log_debug(message, ...) log_message(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
#    define log_debug(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
/**
 * @brief Logs a trace-level message. Should be used for very detailed debugging information.
 *
 * @param message The message to log.
 * @param ... The format string and any formatted data to be included in the message.
 */
#    define log_trace(message, ...) log_message(LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
#    define log_trace(message, ...)
#endif

#endif // ENGINE_LOGGING_H
