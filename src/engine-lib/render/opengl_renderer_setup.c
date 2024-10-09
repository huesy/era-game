#include "engine/render/opengl_renderer.h"
#include "engine/render/renderer.h"

void setup_opengl_renderer(void) {
    RendererAPI openglApi = {0};
    openglApi.initialize = opengl_renderer_init;
    openglApi.shutdown = opengl_renderer_shutdown;
    openglApi.draw = opengl_renderer_draw;

    renderer_set_api(&openglApi);
}
