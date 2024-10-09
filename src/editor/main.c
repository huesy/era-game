#include "editor.h"
#include "editor/editor.h"
#include "engine/engine.h"
#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"

int main(void) {
    EngineConfig engineConfig = {0};
    engineConfig.memoryPoolSize = 1024 * 1024 * 64; // 64MB memory pool.

    if (engine_init(&engineConfig) != ENGINE_SUCCESS) {
        log_error("Failed to initialize engine.");
        return 1;
    }

    editor_init();
    engine_run();
    editor_shutdown();
    engine_shutdown();

    return 0;
}
