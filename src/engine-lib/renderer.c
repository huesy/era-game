#include "engine/renderer.h"

ENGINE_API EngineResult
renderer_create(RendererConfig *config, Renderer *renderer, Platform *platform) {
    if (!config) {
        log_error("Invalid renderer configuration.");
        return ENGINE_FAILURE;
    }

    if (!renderer) {
        log_error("Invalid renderer.");
        return ENGINE_FAILURE;
    }

    if (!platform) {
        log_error("Invalid platform.");
        return ENGINE_FAILURE;
    }

    if (!platform->createRenderer) {
        log_error("Platform does not support renderer creation.");
        return ENGINE_FAILURE;
    }

    PlatformRendererConfig platformConfig = {0};

    if (platform->createRenderer(&platformConfig, renderer->platformRenderer) != ENGINE_SUCCESS) {
        log_error("Failed to create renderer.");
        return ENGINE_FAILURE;
    }

    return ENGINE_SUCCESS;
}

ENGINE_API void
renderer_destroy(Renderer *renderer, Platform *platform) {
    if (!renderer) {
        log_error("Invalid renderer.");
        return;
    }

    if (!platform) {
        log_error("Invalid platform.");
        return;
    }

    if (!platform->destroyRenderer) {
        log_error("Platform does not support renderer destruction.");
        return;
    }

    platform->destroyRenderer(renderer->platformRenderer);
}

ENGINE_API void
renderer_present(Renderer *renderer, Platform *platform) {
    if (!renderer) {
        log_error("Invalid renderer.");
        return;
    }

    if (!platform) {
        log_error("Invalid platform.");
        return;
    }

    if (!platform->renderPresent) {
        log_error("Platform does not support renderer presentation.");
        return;
    }

    platform->renderPresent(renderer->platformRenderer);
}

ENGINE_API void
renderer_set_draw_color(Renderer *renderer, u8 r, u8 g, u8 b, u8 a) {
    if (!renderer) {
        log_error("Invalid renderer.");
        return;
    }

    if (!renderer->platformRenderer) {
        log_error("Invalid platform renderer.");
        return;
    }

    if (!renderer->platformRenderer->setDrawColor) {
        log_error("Platform renderer does not support setting the draw color.");
        return;
    }

    renderer->platformRenderer->setDrawColor(renderer->platformRenderer, r, g, b, a);
}

ENGINE_API void
renderer_clear(Renderer *renderer, Platform *platform) {
    if (!renderer) {
        log_error("Invalid renderer.");
        return;
    }

    if (!platform) {
        log_error("Invalid platform.");
        return;
    }

    if (!platform->renderClear) {
        log_error("Platform does not support renderer clearing.");
        return;
    }

    platform->renderClear(renderer->platformRenderer);
}
