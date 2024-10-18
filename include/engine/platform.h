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
#include "engine/memory.h"
#include "engine/window.h"

// =============================================================================
#pragma region Types

// Forward declaration.
typedef struct Platform Platform;
typedef struct Renderer Renderer;

/**
 * @brief Function pointer types for platform abstraction.
 */
typedef EngineResult (*PlatformInitFunc)(Platform *platform, MemoryPool *pool);
typedef void (*PlatformShutdownFunc)(Platform *platform);
typedef void (*PlatformPollEventsFunc)(Platform *platform);
typedef b8 (*PlatformIsRunningFunc)(Platform *platform);
typedef u64 (*PlatformGetAbsoluteTimeFunc)(Platform *platform);

/**
 * @brief Platform abstraction structure.
 */
typedef struct Platform {
    Window *window;                              /**< Pointer to the window instance. */
    Renderer *renderer;                          /**< Pointer to the renderer instance. */
    MemoryPool *memoryPool;                      /**< Pointer to the memory pool instance. */
    void *data;                                  /**< Pointer to platform-specific data. */
    PlatformInitFunc init;                       /**< Function pointer to initialize the platform. */
    PlatformShutdownFunc shutdown;               /**< Function pointer to shutdown the platform. */
    PlatformPollEventsFunc pollEvents;           /**< Function pointer to poll events. */
    PlatformIsRunningFunc isRunning;             /**< Function pointer to check if the platform is running. */
    PlatformGetAbsoluteTimeFunc getAbsoluteTime; /**< Function pointer to get the absolute time. */
} Platform;

#pragma endregion
// =============================================================================
#pragma region Platform

/**
 * @brief Initializes the platform.
 *
 * @param platform A pointer to the Platform structure.
 * @param pool A pointer to the memory pool structure.
 * @return ENGINE_SUCCESS if the platform was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult platform_init(Platform *platform, MemoryPool *pool);

/**
 * @brief Shuts down the platform.
 *
 * @param platform A pointer to the Platform structure.
 * @return void
 */
ENGINE_API void platform_shutdown(Platform *platform);

/**
 * @brief Polls platform-specific events.
 *
 * @param platform A pointer to the Platform structure.
 * @return void
 */
ENGINE_API void platform_poll_events(Platform *platform);

/**
 * @brief Check if the platform should continue running.
 *
 * @param platform A pointer to the Platform structure.
 * @return b8 True if the platform should continue running, otherwise false.
 */
ENGINE_API b8 platform_is_running(Platform *platform);

/**
 * @brief Gets the absolute time in seconds.
 *
 * @param platform A pointer to the Platform structure.
 * @return The absolute time in seconds.
 */
ENGINE_API u64 platform_get_absolute_time(Platform *platform);

#pragma endregion
// =============================================================================
// #pragma region Window

// /**
//  * @brief Creates a window.
//  *
//  * @param config A pointer to the window configuration structure.
//  * @param window A double pointer to the window to create.
//  * @return ENGINE_SUCCESS if the window was created successfully, otherwise an error code.
//  */
// ENGINE_API EngineResult platform_window_create(const PlatformWindowConfig *config, PlatformWindow **window);

// /**
//  * @brief Destroys a window.
//  *
//  * @param window A pointer to the window to destroy.
//  * @return void
//  */
// ENGINE_API void platform_window_destroy(PlatformWindow *window);

// #pragma endregion
// // =============================================================================
#pragma region Renderer

/**
 * @brief Clears the renderer using the platform.
 *
 * @param platform A pointer to the platform to clear the renderer on.
 * @return void
 */
ENGINE_API void platform_renderer_clear(Platform *platform);

/**
 * @brief Presents the renderer using the platform.
 *
 * @param platform A pointer to the platform to present the renderer on.
 * @return void
 */
ENGINE_API void platform_renderer_present(Platform *platform);

#pragma endregion
// =============================================================================
#pragma region Memory

/**
 * @brief Allocates memory.
 *
 * @param size The size of the memory to allocate.
 * @return A pointer to the allocated memory.
 */
ENGINE_API void *platform_memory_allocate(u64 size);

/**
 * @brief Frees memory.
 *
 * @param block A pointer to the memory to free.
 * @return void
 */
ENGINE_API void platform_memory_free(void *block);

/**
 * @brief Allocates aligned memory.
 *
 * @param size The size of the memory to allocate.
 * @param alignment The alignment of the memory to allocate.
 * @return A pointer to the allocated memory.
 */
ENGINE_API void *platform_memory_allocate_aligned(u64 size, u16 alignment);

/**
 * @brief Frees aligned memory.
 *
 * @param block A pointer to the memory to free.
 * @return void
 */
ENGINE_API void platform_memory_free_aligned(void *block);

/**
 * @brief Copies memory.
 *
 * @param dest A pointer to the destination memory.
 * @param src A pointer to the source memory.
 * @param size The size of the memory to copy.
 * @return A pointer to the destination memory.
 */
ENGINE_API void *platform_memory_copy(void *dest, const void *src, u64 size);

/**
 * @brief Sets memory.
 *
 * @param dest A pointer to the destination memory.
 * @param value The value to set the memory to.
 * @param size The size of the memory to set.
 * @return A pointer to the destination memory.
 */
ENGINE_API void *platform_memory_set(void *dest, i32 value, u64 size);

/**
 * @brief Zeros memory.
 *
 * @param block A pointer to the memory to zero.
 * @param size The size of the memory to zero.
 * @return A pointer to the zeroed memory.
 */
ENGINE_API void *platform_memory_zero(void *block, u64 size);

#pragma endregion
// =============================================================================
#pragma region Threading

/**
 * @brief Creates a mutex.
 *
 * @param lock A double pointer to the mutex to create.
 * @return void
 */
ENGINE_API void platform_mutex_create(void **lock);

/**
 * @brief Destroys a mutex.
 *
 * @param lock A pointer to the mutex to destroy.
 * @return void
 */
ENGINE_API void platform_mutex_destroy(void *lock);

/**
 * @brief Locks a mutex.
 *
 * @param lock A pointer to the mutex to lock.
 * @return void
 */
ENGINE_API void platform_mutex_lock(void *lock);

/**
 * @brief Unlocks a mutex.
 *
 * @param lock A pointer to the mutex to unlock.
 * @return void
 */
ENGINE_API void platform_mutex_unlock(void *lock);

#pragma endregion
// =============================================================================
// #pragma region Shared Library

// /**
//  * @brief Loads a dynamic library.
//  *
//  * @param path The path to the dynamic library to load.
//  * @return A pointer to the loaded dynamic library.
//  */
// ENGINE_API void *platform_dynamic_library_load(const char *path);

// /**
//  * @brief Unloads a dynamic library.
//  *
//  * @param library A pointer to the dynamic library to unload.
//  * @return void
//  */
// ENGINE_API void platform_dynamic_library_unload(void *library);

// /**
//  * @brief Loads a function from a dynamic library.
//  *
//  * @param library A pointer to the dynamic library to load the function from.
//  * @param symbol The name of the function to load.
//  * @return A pointer to the loaded function.
//  */
// ENGINE_API void *platform_dynamic_library_load_function(
//     void *library,
//     const char *symbol);

// #pragma endregion
// // =============================================================================

#endif // ENGINE_PLATFORM_H
