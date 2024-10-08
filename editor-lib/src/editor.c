#include <core/logging.h>
#include <engine.h>

int main(void) {
    PlatformCreateInfo config = {0};
    config.title = "Editor";
    config.width = 1280;
    config.height = 720;

    if (platform_init(&config) != 0) {
        log_error("Failed to initialize platform.");
        return 1;
    }

    Plugin editorPlugin = {0};
    load_plugin(&editorPlugin, "editor");

    if (editorPlugin.handle) {
        run_plugin(&editorPlugin);
        unloadPlugin(&editorPlugin);
    }

    platform_shutdown();
    return 0;
}
