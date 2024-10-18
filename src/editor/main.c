#include "editor/editor.h"
#include <engine/application.h>
#include <engine/engine.h>
#include <engine/logging.h>
#include <engine/memory.h>
#include <engine/platform.h>
#include <stdio.h>
#include <time.h>

void application_update(f32 deltaTime) {
    // Update the editor.
    log_debug("Editor updating with deltaTime %.3f seconds.", deltaTime);
}

void application_render(void) {
    log_debug("Editor rendering.");
}

int main(void) {

    log_info("Starting editor...");

    // Define window configuration.
    WindowConfig windowConfig = {0};
    windowConfig.title = "Era Editor";
    windowConfig.x = 100;
    windowConfig.y = 100;
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.fullScreen = false;

    // Define renderer configuration.
    RendererConfig rendererConfig = {0};
    rendererConfig.name = "SDL3";

    // Define application configuration.
    ApplicationConfig appConfig = {0};
    appConfig.window = windowConfig;
    appConfig.renderer = rendererConfig;
    appConfig.update = application_update;
    appConfig.render = application_render;

    // Initialize the engine.
    Engine engine = {0};
    EngineConfig engineConfig = {0};
    engineConfig.memoryPoolSize = 1024 * 1024 * 64; // 64MB

    if (engine_init(&engineConfig, &engine) != ENGINE_SUCCESS) {
        log_error("Failed to initialize engine.");
        return -1;
    }

    // Initialize the application with user configurations and callbacks.
    Application app = {0};
    if (application_init(&engine, &appConfig, &app) != ENGINE_SUCCESS) {
        log_error("Failed to initialize application.");
        engine_shutdown(&engine);
        return -1;
    }

    // Run the application.
    engine_run(&engine, &app);

    // Shutdown the application.
    application_shutdown(&app);

    // Detect memory leaks.
    memory_pool_detect_leaks(&engine.memoryPool);

    // Shutdown the engine.
    engine_shutdown(&engine);

    log_info("Editor finished.");
    return 0;
}
