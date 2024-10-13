#include "editor/editor.h"
#include "engine/logging.h"
#include "engine/platform.h"
#include <stdio.h>
#include <time.h>

ENGINE_API b8 editor_init(void) {
    log_info("Editor initialized.");

    WindowConfig windowConfig = {0};
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.title = "Editor";

    PlatformConfig platformConfig = {0};
    platformConfig.window = windowConfig;

    if (platform_init(&platformConfig) != ENGINE_SUCCESS) {
        return false;
    }

    void *window = platform_create_window(&windowConfig);
    if (!window) {
        log_error("Failed to create window.");
        platform_shutdown();
        return false;
    }

    return true;
}

ENGINE_API void editor_shutdown(void) {
    log_info("Editor shutdown.");
    platform_shutdown();
}

ENGINE_API void editor_update(f32 deltaTime) {
    ENGINE_UNUSED(deltaTime);
    // TODO: Implement editor-specific logic.
    // log_debug("Editor update: %.3f seconds elapsed.", deltaTime);
}

ENGINE_API void editor_render(void) {
    // TODO: Implement editor-specific rendering logic.
    // log_debug("Editor render.");
}
