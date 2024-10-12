#include "editor/editor.h"
#include "engine/logging.h"
#include "engine/platform.h"
#include <stdio.h>
#include <time.h>

int main(void) {
    log_info("Starting editor...");

    if (!editor_init()) {
        log_error("Editor initialization failed.");
        return -1;
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

    log_info("Editor finished.");
    return 0;
}
