#include "input/input.h"

#ifdef PLATFORM_WINDOWS
#    include <windows.h>

static u8 keyStates[KEY_MAX];

static void windows_input_initialize() {
    // TODO: Custom initialization for Windows input.
}

static void windows_input_shutdown() {
    // TODO: Custom shutdown for Windows input.
}

static void windows_input_update() {
    for (InputKey key = 0; key < KEY_MAX; ++key) {
        // Update the key state. 0x8000 is the high bit for the key state.
        // This needs to be masked out to get the actual key state.
        keyStates[key] = GetAsyncKeyState(key) & 0x8000;
    }
}

static u8 windows_key_pressed(InputKey key) {
    return keyStates[key];
}

static InputAPI windows_input_api = {0};
windows_input_api.initialize = windows_input_initialize;
windows_input_api.shutdown = windows_input_shutdown;
windows_input_api.update = windows_input_update;
windows_input_api.keyPressed = windows_key_pressed;

void input_windows_set_api() {
    input_set_api(&windows_input_api);
}

#endif // PLATFORM_WINDOWS
