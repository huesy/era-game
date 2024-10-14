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

// Threading
#if defined(PLATFORM_WINDOWS)
#    include <windows.h>
typedef CRITICAL_SECTION PlatformMutex;
#else
#    include <pthread.h>
typedef pthread_mutex_t PlatformMutex;
#endif

/**
 * @brief Configuration structure for platform initialisation.
 */
typedef struct WindowConfig {
    u32 width;         /**< The width of the window to create. */
    u32 height;        /**< The height of the window to create. */
    const char *title; /**< The title of the window to create. */
} WindowConfig;

/**
 * @brief Configuration structure for platform initialisation.
 */
typedef struct PlatformConfig {
    WindowConfig window; /**< Configuration for the window to create. */
} PlatformConfig;

typedef struct Window {
    /** @brief The title of the window.  */
    const char *title;

    /** @brief The width of the window. */
    u16 width;
    /** @brief The height of the window. */
    u16 height;

    /** @brief The handle to the window. */
    void *handle;
} Window;

/**
 * @brief Initialises the platform, including creating a window if needed.
 *
 * @param config Pointer to the platform configuration structure.
 * @return ENGINE_SUCCESS if initialisation was successful, otherwise an error code.
 */
ENGINE_API EngineResult platform_init(PlatformConfig *config);

/**
 * @brief Shuts down the platform, cleaning up any resources.
 */
ENGINE_API void platform_shutdown(void);

/**
 * @brief Polls the platform for events such as input or window events.
 */
ENGINE_API void platform_poll_events(void);

/**
 * @brief Checks if the platform is still running.
 *
 * @return b8 True if the platform is running, false otherwise.
 */
ENGINE_API b8 platform_is_running(void);

/**
 * @brief Retrieves the absolute time in seconds since the platform was started.
 *
 * @return f32 The absolute time in seconds.
 */
ENGINE_API f32 platform_get_absolute_time(void);

// =============================================================================
// Windowing

/**
 * @brief Creates a window with the specified configuration.
 *
 * @param config Pointer to the platform configuration structure.
 * @param window Pointer to the window to create.
 * @return ENGINE_SUCCESS if the window was created successfully, otherwise an error code.
 */
ENGINE_API EngineResult platform_create_window(WindowConfig *config, Window *window);

/**
 * @brief Destroys the window.
 *
 * @param window Pointer to the window to destroy.
 */
ENGINE_API void platform_destroy_window(Window *window);

/**
 * @brief Retrieves the width of the window.
 *
 * @param window Pointer to the window.
 * @return u32 The width of the window.
 */
ENGINE_API u32 platform_get_window_width(void *window);

/**
 * @brief Retrieves the height of the window.
 *
 * @param window Pointer to the window.
 * @return u32 The height of the window.
 */
ENGINE_API u32 platform_get_window_height(void *window);

/**
 * @brief Sets the title of the window.
 *
 * @param window Pointer to the window.
 * @param title The title of the window.
 */
ENGINE_API void platform_set_window_title(void *window, const char *title);

/**
 * @brief Checks if the window is still open.
 *
 * @param window Pointer to the window.
 * @return b8 True if the window is open, false otherwise.
 */
ENGINE_API b8 platform_is_window_open(void *window);

// =============================================================================
// Dynamic Library Loading

/**
 * @brief Loads a dynamic library from the given path.
 *
 * @param path The path to the dynamic library to load.
 * @return void* A handle to the loaded library, or NULL if an error occurred.
 */
ENGINE_API void *platform_load_library(const char *path);

/**
 * @brief Retrieves a function pointer from a loaded dynamic library.
 *
 * @param library The handle to the loaded library.
 * @param functionName The name of the function to retrieve.
 * @return void* A pointer to the function, or NULL if an error occurred.
 */
ENGINE_API void *platform_get_library_function(void *library, const char *functionName);

/**
 * @brief Unloads a dynamic library.
 *
 * @param library The handle to the loaded library.
 */
ENGINE_API void platform_unload_library(void *library);

// =============================================================================
// Memory

/**
 * @brief Allocates a block of memory from the platform.
 *
 * @param size Size of the memory block to allocate.
 * @param alignment Alignment of the memory block.
 * @return void* Pointer to the allocated memory block.
 */
void *platform_memory_allocate(u64 size, u16 alignment);

/**
 * @brief Frees a block of memory from the platform.
 *
 * @param block Pointer to the memory block to free.
 */
void platform_memory_free(void *block);

/**
 * @brief Copy memory from the source block to the destination block.
 *
 * @param dest Pointer to the destination memory block.
 * @param src Pointer to the source memory block that will be copied.
 * @param size Size of the memory block to copy.
 * @return void* Pointer to the destination memory block.
 */
void *platform_memory_copy(void *dest, const void *src, u64 size);

/**
 * @brief Set the memory block with the specified value.
 *
 * @param dest Pointer to the memory block to set.
 * @param value Value to set the memory block to.
 * @param size Size of the memory block to set.
 * @return void* Pointer to the memory block.
 */
void *platform_memory_set(void *dest, i32 value, u64 size);

/**
 * @brief Zero out the memory block with the specified size.
 *
 * @param block Pointer to the memory block to zero out.
 * @param size Size of the memory block to zero out.
 * @return void* Pointer to the zeroed out memory block.
 */
void *platform_memory_zero(void *block, u64 size);

// =============================================================================
// Threading

/**
 * @brief Initializes a mutex lock for thread safety.
 *
 * @param lock Pointer to the mutex lock to initialize.
 */
void platform_mutex_init(void *lock);

/**
 * @brief Locks a mutex lock for thread safety.
 *
 * @param lock Pointer to the mutex lock to lock.
 */
void platform_mutex_lock(void *lock);

/**
 * @brief Unlocks a mutex lock for thread safety.
 *
 * @param lock Pointer to the mutex lock to unlock.
 */
void platform_mutex_unlock(void *lock);

/**
 * @brief Destroys a mutex lock for thread safety.
 *
 * @param lock Pointer to the mutex lock to destroy.
 */
void platform_mutex_destroy(void *lock);

#endif // ENGINE_PLATFORM_H
