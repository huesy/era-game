#include "engine/window.h"
#include "engine/logging.h"
#include "engine/platform.h"

ENGINE_API EngineResult window_init(MemoryPool *pool, const WindowConfig *config, Window *window) {
    if (!pool || !config || !window) {
        log_error("Invalid MemoryPool, WindowConfig, or Window provided to window_create.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    // Assign configuration.
    window->config = *config;

    // TODO:
    // Platform-specific window handle is already created in the platform-specific code.
    // Therefore, no additional initialization is required here.
    // Alternatively, we can retrieve the window handle from the Platform struct.
    // For simplicity, we'll assume it's already handled.

    log_info("Window initialized: %s [%dx%d] at (%d, %d). Fullscreen: %s",
             window->config.title,
             window->config.width,
             window->config.height,
             window->config.x,
             window->config.y,
             window->config.fullScreen ? "true" : "false");

    return ENGINE_SUCCESS;
}

ENGINE_API void window_shutdown(MemoryPool *pool, Window *window) {
    if (!pool || !window) {
        log_error("Invalid MemoryPool or Window provided to window_destroy.");
        return;
    }

    // TODO:
    // Platform-specific window handle is destroyed in the platform-specific code.
    // Therefore, no additional shutdown is needed here.

    log_info("Window shutdown: %s", window->config.title);
}
