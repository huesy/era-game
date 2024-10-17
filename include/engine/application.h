#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include "engine/defines.h"
#include "engine/renderer.h"
#include "engine/window.h"

typedef struct ApplicationConfig {
    RendererConfig rendererConfig;
    WindowConfig windowConfig;
} ApplicationConfig;

typedef struct Application {
    Renderer *renderer;
    Window *window;
} Application;

void application_init(ApplicationConfig *config, Application *app);
void application_run(Application *app);
void application_shutdown(Application *app);

#endif // ENGINE_APPLICATION_H
