#include "editor/editor.h"
#include "engine/platform.h"
#include <stdio.h>

ENGINE_API b8 editor_init(void) {
    printf("Editor initialized.\n");

    WindowConfig windowConfig = {0};
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.title = "Editor";

    PlatformConfig platformConfig = {0};
    platformConfig.window = windowConfig;

    if (platform_init(&platformConfig) != ENGINE_SUCCESS) {
        return false;
    }

    platform_create_window(&windowConfig);

    return true;
}

ENGINE_API void editor_shutdown(void) {
    printf("Editor shutdown.\n");
}

ENGINE_API void editor_update(f32 deltaTime) {
    printf("Editor update: %f\n", deltaTime);
}

ENGINE_API void editor_render(void) {
    printf("Editor render.\n");
}
