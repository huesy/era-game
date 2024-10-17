#include "engine/application.h"
#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"

void application_init(ApplicationConfig *config, Application *app) {
    if (!config) {
        log_error("Invalid application configuration.");
        return;
    }

    if (!app) {
        log_error("Invalid application.");
        return;
    }

    app->renderer = memory_allocate(sizeof(Renderer), MEMORY_TAG_RENDERER);
    app->window = memory_allocate(sizeof(Window), MEMORY_TAG_PLATFORM);

    renderer_init(&config->rendererConfig, app->renderer);
    window_init(&config->windowConfig, app->window);
}

void application_run(Application *app) {
    if (!app) {
        log_error("Invalid application.");
        return;
    }

    while (platform_is_running()) {
        platform_poll_events();
    }

    application_shutdown(app);
}

void application_shutdown(Application *app) {
    if (!app) {
        log_error("Invalid application.");
        return;
    }

    if (app->renderer) {
        renderer_shutdown(app->renderer);
        memory_free(app->renderer);
    }

    if (app->window) {
        window_shutdown(app->window);
        memory_free(app->window);
    }
}
