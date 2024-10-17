#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include "engine/defines.h"
#include "engine/platform.h"
#include "engine/renderer.h"
#include "engine/window.h"

typedef struct ApplicationConfig {
    RendererConfig renderer;
    WindowConfig window;
} ApplicationConfig;

typedef struct Application {
    Renderer *renderer;
    Window *window;
    Platform *platform;
} Application;

ENGINE_API EngineResult application_create(const ApplicationConfig *config, Application *app);
ENGINE_API void application_run(Application *app);
ENGINE_API void application_shutdown(Application *app);

#endif // ENGINE_APPLICATION_H
