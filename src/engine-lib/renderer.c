#include "engine/renderer.h"
#include "engine/logging.h"
#include "engine/platform.h"

ENGINE_API EngineResult renderer_init(MemoryPool *pool, const RendererConfig *config, Renderer *renderer, Platform *platform) {
    if (!pool || !config || !renderer || !platform) {
        log_error("Invalid MemoryPool, RendererConfig, Renderer, or Platform provided to renderer_init.");
        return ENGINE_ERROR_INVALID_ARGUMENT;
    }

    // Assign configuration.
    renderer->config = *config;

    // TODO:
    // Initialize renderer-specific resources via platform abstraction.
    // Currently, no additional initialization is required as platform handles
    // renderer creation.

    log_info("Renderer initialized: %s", renderer->config.name);
    return ENGINE_SUCCESS;
}

ENGINE_API void renderer_shutdown(Renderer *renderer, Platform *platform) {
    if (!renderer || !platform) {
        log_error("Invalid Renderer or Platform provided to renderer_shutdown.");
        return;
    }

    // Delegate shutdown to platform abstraction if needed.
    // Currently, platform handles renderer descruction.

    log_info("Renderer shutdown: %s", renderer->config.name);
}

ENGINE_API void renderer_clear(Renderer *renderer, Platform *platform) {
    if (!renderer || !platform) {
        log_error("Invalid Renderer or Platform provided to renderer_clear.");
        return;
    }

    // Delegate clearing to platform abstraction.
    platform_renderer_clear(platform);
}

ENGINE_API void renderer_present(Renderer *renderer, Platform *platform) {
    if (!renderer || !platform) {
        log_error("Invalid Renderer or Platform provided to renderer_present.");
        return;
    }

    // Delegate presenting to platform abstraction.
    platform_renderer_present(platform);
}
