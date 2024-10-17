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
#include "engine/window.h"

// Threading
#if defined(PLATFORM_WINDOWS)
#    include <windows.h>
typedef CRITICAL_SECTION PlatformMutex;
#else
#    include <pthread.h>
typedef pthread_mutex_t PlatformMutex;
#endif

// Forward declrations for platform-specific handles.
typedef struct PlatformWindow PlatformWindow;
typedef struct PlatformRenderer PlatformRenderer;

// Platform Configuration
typedef struct PlatformWindowConfig {
    const char *title; /**< The title of the window to create. */
    i32 width;         /**< The width of the window to create. */
    i32 height;        /**< The height of the window to create. */
    b8 fullScreen;     /**< True if the window should be created in full screen mode. */
} PlatformWindowConfig;

typedef struct PlatformRendererConfig {

} PlatformRendererConfig;

typedef struct Platform {
    // Window-related functions.
    EngineResult (*createWindow)(const PlatformWindowConfig *config, PlatformWindow *window);
    void (*destroyWindow)(PlatformWindow *window);
    void (*pollEvents)(b8 *isRunning);

    // Renderer-related functions.
    void *(*createRenderer)(const PlatformRendererConfig *config, PlatformRenderer *renderer);
    void (*destroyRenderer)(PlatformRenderer *renderer);
    void (*renderPresent)(PlatformRenderer *renderer);
    void (*setDrawColor)(PlatformRenderer *renderer, u8 r, u8 g, u8 b, u8 a);
    void (*renderClear)(PlatformRenderer *renderer);

    // Dynamic library loading functions.
    void *(*loadLibrary)(const char *path);
    void *(*getLibraryFunction)(void *library, const char *functionName);
    void (*unloadLibrary)(void *library);

    // Memory functions.
    void *(*memoryAllocate)(u64 size);
    void *(*memoryAlignedAllocate)(u64 size, u16 alignment);
    void (*memoryFree)(void *block);
    void *(*memoryCopy)(void *dest, const void *src, u64 size);
    void *(*memorySet)(void *dest, i32 value, u64 size);
    void *(*memoryZero)(void *block, u64 size);

    // Threading functions.
    void (*mutexInit)(PlatformMutex *lock);
    void (*mutexLock)(PlatformMutex *lock);
    void (*mutexUnlock)(PlatformMutex *lock);
    void (*mutexDestroy)(PlatformMutex *lock);

    // Platform-specific data.
    void *data;
} Platform;

/**
 * @brief Initialises the platform, including creating a window if needed.
 *
 * @param config Pointer to the platform configuration structure.
 * @param platform Pointer to the platform structure to initialise.
 * @return ENGINE_SUCCESS if initialisation was successful, otherwise an error code.
 */
ENGINE_API EngineResult
platform_init(Platform *platform);

/**
 * @brief Shuts down the platform, cleaning up any resources.
 *
 * @param platform Pointer to the platform structure to shut down.
 */
ENGINE_API void
platform_shutdown(Platform *platform);

#endif // ENGINE_PLATFORM_H
