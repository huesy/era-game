#include "render/renderer.h"

// #include <GL/gl.h>

static RendererAPI *currentApi = NULL;

void renderer_set_api(RendererAPI *api) {
    currentApi = api;
}

void renderer_initialize(void) {
    if (currentApi && currentApi->initialize) {
        currentApi->initialize();
    }
}

void renderer_shutdown(void) {
    if (currentApi && currentApi->shutdown) {
        currentApi->shutdown();
    }
}

void renderer_draw(void) {
    if (currentApi && currentApi->draw) {
        currentApi->draw();
    }
}
