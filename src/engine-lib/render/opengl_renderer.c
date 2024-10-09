#include "engine/render/opengl_renderer.h"
#include "engine/logging.h"
#ifdef PLATFORM_WINDOWS
// Prevent clang-format from reordering these includes.
// windows.h must come before gl.h.
// clang-format off
#    include <windows.h>
#    include <GL/gl.h>
// clang-format on
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
#    include <OpenGL/gl.h>
#endif

void opengl_renderer_init(void) {
    log_info("Initialising OpenGL renderer");

    // TODO: Innitialize OpenGL context here.
    // For example, set up projection matrices, load shaders, etc.
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void opengl_renderer_shutdown(void) {
    log_info("Shutting down OpenGL renderer");
    // TODO: Cleanup OpenGL resources here.
}

void opengl_renderer_draw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: Render sprites and other objects here.
    // Example: Drawing a simple triangle.
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex2f(0.0f, 0.5f);
    glEnd();
    // TODO: Swap buffers handled by the windowing system.
}
