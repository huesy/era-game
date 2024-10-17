#include "engine/window.h"
#include "engine/platform.h"

ENGINE_API EngineResult
window_create(WindowConfig *config, Window *window, Platform *platform) {
    if (!config) {
        log_error("Invalid window configuration.");
        return ENGINE_FAILURE;
    }

    if (!window) {
        log_error("Invalid window.");
        return ENGINE_FAILURE;
    }

    if (!platform) {
        log_error("Invalid platform.");
        return ENGINE_FAILURE;
    }

    if (!platform->createWindow) {
        log_error("Platform does not support window creation.");
        return ENGINE_FAILURE;
    }

    PlatformWindowConfig platformConfig = {0};
    platformConfig.title = config->title;
    platformConfig.width = config->width;
    platformConfig.height = config->height;
    platformConfig.fullScreen = config->fullScreen;

    if (platform->createWindow(&platformConfig, window->platformWindow) != ENGINE_SUCCESS) {
        log_error("Failed to create window.");
        return ENGINE_FAILURE;
    }

    return ENGINE_SUCCESS;
}

ENGINE_API void
window_destroy(Window *window, Platform *platform) {
    if (!window) {
        log_error("Invalid window.");
        return;
    }

    if (!platform) {
        log_error("Invalid platform.");
        return;
    }

    if (!platform->destroyWindow) {
        log_error("Platform does not support window destruction.");
        return;
    }

    platform->destroyWindow(window->platformWindow);
}
