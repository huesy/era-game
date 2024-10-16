#include "editor/editor.h"
#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"
#include <stdio.h>
#include <time.h>

typedef struct AppState {
    b8 isRunning;
    Window *windows;
} AppState;

global AppState state = {0};

int main(void) {
    log_info("Starting editor...");

    // Stand up the platform/window.
    WindowConfig windowConfig = {0};
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.title = "Editor";

    PlatformConfig platformConfig = {0};
    platformConfig.window = windowConfig;

    if (platform_init(&platformConfig) != ENGINE_SUCCESS) {
        return 1;
    }

    state.isRunning = true;
    state.windows = memory_allocate(sizeof(Window) * 1, MEMORY_TAG_EDITOR); // Allocate memory for one window.

    if (!state.windows) {
        log_error("Failed to allocate memory for windows.");
        platform_shutdown();
        return 1;
    }

    // TODO: Pass rendering context to the editor lib.
    if (editor_init() != ENGINE_SUCCESS) {
        log_error("Editor initialization failed.");
        return -1;
    }

    if (platform_create_window(&windowConfig, &state.windows[0]) != ENGINE_SUCCESS) {
        log_error("Failed to create window.");
        editor_shutdown();

        memory_free(state.windows, MEMORY_TAG_EDITOR);

        platform_shutdown();
        return 1;
    }

    f32 previousTime = platform_get_absolute_time();
    f32 deltaTime = 0.0f;

    // Main loop.
    while (platform_is_running()) {
        // Poll for platform-specific events.
        platform_poll_events();

        // Calculate deltaTime
        f32 currentTime = platform_get_absolute_time();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        // Update editor state.
        editor_update(deltaTime);

        // Render editor.
        editor_render();
    }

    editor_shutdown();
    memory_free(state.windows, MEMORY_TAG_EDITOR);
    platform_shutdown();

    log_info("Editor finished.");
    return 0;
}
