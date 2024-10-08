#include <core/logging.h>
#include <engine.h>

void editor_init(void) {
    printf("Editor initialized.\n");
}

void editor_shutdown(void) {
    printf("Editor shutdown.\n");
}

void editor_update(void) {
    printf("Editor update.\n");
}

void editor_render(void) {
    printf("Editor render.\n");
}

void create_application(Application *app) {
    app->config.name = "Editor";

    app->init = editor_init;
    app->shutdown = editor_shutdown;
    app->update = editor_update;
    app->render = editor_render;
}
