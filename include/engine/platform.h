/**
 * @file platform.h
 * @author Andrew Hughes (a.hughes@gmail.com)
 * @brief Platform abstraction layer to provide cross-platform functionality for
 * the engine. Supports creating windows, handling dynamic libraries, and other
 * platform-specific tasks.
 * @version 0.1
 * @date 2024-10-08
 *
 * @copyright Era Engine is Copyright (c) Andrew Hughes 2024
 */

#ifndef ENGINE_PLATFORM_H
#define ENGINE_PLATFORM_H

#include "engine/defines.h"

/**
 * @brief Configuration structure for platform initialisation.
 */
typedef struct PlatformConfig {
    u32 width;         /**< The width of the window to create. */
    u32 height;        /**< The height of the window to create. */
    const char *title; /**< The title of the window to create. */
} PlatformConfig;

/**
 * @brief Initialises the platform, including creating a window if needed.
 *
 * @param config Pointer to the platform configuration structure.
 * @return ENGINE_SUCCESS if initialisation was successful, otherwise an error code.
 */
EngineResult platform_init(PlatformConfig *config);

/**
 * @brief Shuts down the platform, cleaning up any resources.
 */
void platform_shutdown(void);

/**
 * @brief Polls the platform for events such as input or window events.
 */
void platform_poll_events(void);

/**
 * @brief Checks if the platform is still running.
 *
 * @return b8 True if the platform is running, false otherwise.
 */
b8 platform_is_running(void);

/**
 * @brief Loads a dynamic library from the given path.
 *
 * @param path The path to the dynamic library to load.
 * @return void* A handle to the loaded library, or NULL if an error occurred.
 */
void *platform_load_library(const char *path);

/**
 * @brief Retrieves a function pointer from a loaded dynamic library.
 *
 * @param library The handle to the loaded library.
 * @param functionName The name of the function to retrieve.
 * @return void* A pointer to the function, or NULL if an error occurred.
 */
void *platform_get_library_function(void *library, const char *functionName);

/**
 * @brief Unloads a dynamic library.
 *
 * @param library The handle to the loaded library.
 */
void platform_unload_library(void *library);

/**
 * @brief Retrieves the absolute time in seconds since the platform was started.
 *
 * @return f32 The absolute time in seconds.
 */
f32 platform_get_absolute_time(void);

#endif // ENGINE_PLATFORM_H
