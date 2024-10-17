#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include "engine/defines.h"
#include "engine/platform.h"

// Window configuration structure.
typedef struct WindowConfig {
    const char *title; /**< The title of the window to create. */
    i32 width;         /**< The width of the window to create. */
    i32 height;        /**< The height of the window to create. */
    b8 fullScreen;     /**< True if the window should be created in full screen mode. */
} WindowConfig;

// Window structure.
typedef struct Window {
    PlatformWindow *platformWindow; /**< Platform-specific window handle. */
} Window;

// =============================================================================
// Window

/**
 * @brief Creates a window.
 *
 * @param config A pointer to the window configuration structure.
 * @param window A pointer to the window to create.
 * @param platform A pointer to the platform to create the window on.
 * @return ENGINE_SUCCESS if the window was created successfully, otherwise an error code.
 */
ENGINE_API EngineResult
window_create(WindowConfig *config, Window *window, Platform *platform);

/**
 * @brief Destroys a window.
 *
 * @param window A pointer to the window to destroy.
 * @param platform A pointer to the platform to destroy the window on.
 * @return void
 */
ENGINE_API void
window_destroy(Window *window, Platform *platform);

#endif // ENGINE_WINDOW_H
