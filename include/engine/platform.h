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

// Forward declrations for platform-specific handles.
typedef struct PlatformWindow PlatformWindow;
typedef struct PlatformRenderer PlatformRenderer;

// Platform Configuration
typedef struct PlatformWindowConfig {
    const char *title; /**< The title of the window to create. */
    i32 x;             /**< The x position of the window to create. */
    i32 y;             /**< The y position of the window to create. */
    i32 width;         /**< The width of the window to create. */
    i32 height;        /**< The height of the window to create. */
    b8 fullScreen;     /**< True if the window should be created in full screen mode. */
} PlatformWindowConfig;

typedef struct PlatformRendererConfig {
    // TODO: Add renderer-specific configuration...
    const char *name;
} PlatformRendererConfig;

// =============================================================================
#pragma region Platform

/**
 * @brief Initializes the platform.
 *
 * @return ENGINE_SUCCESS if the platform was initialized successfully, otherwise an error code.
 */
ENGINE_API EngineResult platform_init(void);

/**
 * @brief Shuts down the platform.
 *
 * @return void
 */
ENGINE_API void platform_shutdown(void);

// =============================================================================
#pragma region Window

/**
 * @brief Creates a window.
 *
 * @param config A pointer to the window configuration structure.
 * @param window A double pointer to the window to create.
 * @return ENGINE_SUCCESS if the window was created successfully, otherwise an error code.
 */
ENGINE_API EngineResult platform_window_create(const PlatformWindowConfig *config, PlatformWindow **window);

/**
 * @brief Destroys a window.
 *
 * @param window A pointer to the window to destroy.
 * @return void
 */
ENGINE_API void platform_window_destroy(PlatformWindow *window);

// =============================================================================
#pragma region Renderer

/**
 * @brief Creates a renderer.
 *
 * @param config A pointer to the renderer configuration structure.
 * @param window A pointer to the window to create the renderer on.
 * @param renderer A double pointer to the renderer to create.
 * @return ENGINE_SUCCESS if the renderer was created successfully, otherwise an error code.
 */
ENGINE_API EngineResult platform_renderer_create(const PlatformRendererConfig *config, PlatformWindow *window, PlatformRenderer **renderer);

/**
 * @brief Destroys a renderer.
 *
 * @param renderer A pointer to the renderer to destroy.
 * @return void
 */
ENGINE_API void platform_renderer_destroy(PlatformRenderer *renderer);

/**
 * @brief Sets the draw color for the renderer.
 *
 * @param renderer A pointer to the renderer to set the draw color on.
 * @return void
 */
ENGINE_API void platform_renderer_present(PlatformRenderer *renderer);

/**
 * @brief Sets the draw color for the renderer.
 *
 * @param renderer A pointer to the renderer to set the draw color on.
 * @param r The red component of the color.
 * @param g The green component of the color.
 * @param b The blue component of the color.
 * @param a The alpha component of the color.
 * @return void
 */
ENGINE_API void platform_renderer_set_draw_color(PlatformRenderer *renderer, u8 r, u8 g, u8 b, u8 a);

/**
 * @brief Clears the renderer.
 *
 * @param renderer A pointer to the renderer to clear.
 * @return void
 */
ENGINE_API void platform_renderer_clear(PlatformRenderer *renderer);

// =============================================================================
#pragma region Timing

/**
 * @brief Gets the absolute time in seconds.
 *
 * @return The absolute time in seconds.
 */
ENGINE_API u64 platform_get_absolute_time(void);

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
ENGINE_API void *platform_memory_aligned_allocate(u64 size, u16 alignment);

/**
 * @brief Frees aligned memory.
 *
 * @param block A pointer to the memory to free.
 * @return void
 */
ENGINE_API void platform_memory_aligned_free(void *block);

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

// =============================================================================
#pragma region Shared Library

/**
 * @brief Loads a dynamic library.
 *
 * @param path The path to the dynamic library to load.
 * @return A pointer to the loaded dynamic library.
 */
ENGINE_API void *platform_dynamic_library_load(const char *path);

/**
 * @brief Unloads a dynamic library.
 *
 * @param library A pointer to the dynamic library to unload.
 * @return void
 */
ENGINE_API void platform_dynamic_library_unload(void *library);

/**
 * @brief Loads a function from a dynamic library.
 *
 * @param library A pointer to the dynamic library to load the function from.
 * @param symbol The name of the function to load.
 * @return A pointer to the loaded function.
 */
ENGINE_API void *platform_dynamic_library_load_function(
    void *library,
    const char *symbol);

#endif // ENGINE_PLATFORM_H
