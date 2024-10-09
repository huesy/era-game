#include "editor/editor.h"
#include "engine/logging.h"
#include "engine/memory.h"

static EditorState *state = NULL;

ENGINE_API EngineResult editor_init(void) {
    state = memory_allocate(sizeof(EditorState), MEMORY_TAG_EDITOR);
    if (!state) {
        log_error("Failed to allocate memory for editor state.");
        return ENGINE_FAILURE;
    }

    memory_zero(state, sizeof(EditorState));
    state->isRunning = true;

    log_info("Editor initialized.");
    return ENGINE_SUCCESS;
}

ENGINE_API void editor_shutdown(void) {
    if (state) {
        memory_free(state, sizeof(EditorState), MEMORY_TAG_EDITOR);
        state = NULL;
    }

    log_info("Editor shut down.");
}

ENGINE_API void editor_update(f32 deltaTime) {
    if (!state) {
        return;
    }

    log_info("Editor update: %f", deltaTime);
    // Add editor update logic here.
    // For example:
    // - Handle input
    // - Update editor UI
    // - Process editor commands
    // - Update selected entities
    // - ...
}

ENGINE_API void editor_render(void) {
    if (!state) {
        return;
    }

    // Add editor rendering logic here.
    // For example:
    // - Render scene view
    // - Render UI elements
    // - Render gizmos
}
