#include "engine/engine.h"
#include "engine/logging.h"
#include "engine/platform.h"
#include <stdio.h>

EngineResult engine_init(EngineConfig *config) {

    ENGINE_UNUSED(config);

    return ENGINE_SUCCESS;
}

void engine_shutdown(void) {
    log_info("Shutting down engine...");
    platform_shutdown();
}

void engine_run(void) {
    log_info("Running engine...");

    while (platform_is_running()) {
        platform_poll_events();
        // TODO: Additional game loop logic here.
    }

    log_info("Engine stopped.");
}
