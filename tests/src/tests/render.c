#include "render/renderer.h"
#include <stdio.h>

void render_test(void) {
    renderer_initialize();
    printf("Rendering Test: Drawing a square...\n");
    // Call render functions here to draw shapes.
    renderer_draw();
    renderer_shutdown();
}
