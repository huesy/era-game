#include "editor/editor.h"
#include "engine/logging.h"
#include "engine/memory.h"
#include "engine/platform.h"
#include <stdio.h>
#include <time.h>

typedef struct EditorState {
    b8 isRunning;
    Window *windows;
} EditorState;

// static EditorState state = {0};

ENGINE_API EngineResult editor_init(void) {
    log_info("Editor initialized.");
    return ENGINE_SUCCESS;
}

ENGINE_API void editor_shutdown(void) {
    log_info("Editor shutdown.");
}

ENGINE_API void editor_update(f32 deltaTime) {
    ENGINE_UNUSED(deltaTime);
    // TODO: Implement editor-specific logic.
    // log_debug("Editor update: %.3f seconds elapsed.", deltaTime);
}

ENGINE_API void editor_render(void) {
    // TODO: Implement editor-specific rendering logic.
    // log_debug("Editor render.");
}
