#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include "engine/defines.h"
#include "engine/memory.h"

/**
 * @brief Configuration structure for initializing a new window.
 */
typedef struct WindowConfig {
    const char *title; /**< The title of the window to create. */
    i32 x;             /**< The x position of the window to create. */
    i32 y;             /**< The y position of the window to create. */
    i32 width;         /**< The width of the window to create. */
    i32 height;        /**< The height of the window to create. */
    b8 fullScreen;     /**< True if the window should be created in full screen mode. */
} WindowConfig;

/**
 * @brief Window structure to represent a window instance.
 */
typedef struct Window {
    WindowConfig config;  /**< Configuration for the window. */
    void *platformWindow; /**< Platform-specific window handle. */
} Window;

// =============================================================================

/**
 * @brief Initializes a new window.
 *
 * @param pool A pointer to the memory pool to allocate the window from.
 * @param config A pointer to the window configuration structure.
 * @param window A pointer to the window to create.
 * @return ENGINE_SUCCESS if the window was created successfully, otherwise an error code.
 */
ENGINE_API EngineResult window_init(MemoryPool *pool, const WindowConfig *config, Window *window);

/**
 * @brief Shuts down the window and frees resources.
 *
 * @param pool A pointer to the memory pool to free the window from.
 * @param window A pointer to the window to destroy.
 * @return void
 */
ENGINE_API void window_shutdown(MemoryPool *pool, Window *window);

#endif // ENGINE_WINDOW_H
