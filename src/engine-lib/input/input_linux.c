#include "engine/core/logging.h"
#include "engine/input/input.h"

#ifdef PLATFORM_LINUX
#    include <X11/Xlib.h>
#    include <X11/keysym.h>

static Display *display;
static u8 keyStates[KEY_MAX];

static void linux_input_initialize() {
    display = XOpenDisplay(NULL);
    if (!display) {
        log_error("Failed to open X display.");
    }
}

static void linux_input_shutdown() {
    if (display) {
        XCloseDisplay(display);
    }
}

static void linux_input_update() {
    if (!display) {
        return;
    }

    char keys[32];
    XQueryKeymap(display, keys);
    for (InputKey key = 0; key < KEY_MAX; ++key) {
        KeyCode keycode = XKeysymToKeycode(display, key);
        keyStates[key] = (keys[keycode / 8] & (1 << (keycode % 8))) != 0;
    }
}

static u8 linux_key_pressed(InputKey key) {
    return keyStates[key];
}

static InputAPI linuxInputApi = {0};

void input_linux_set_api() {
    linuxInputApi.initialize = linux_input_initialize;
    linuxInputApi.shutdown = linux_input_shutdown;
    linuxInputApi.update = linux_input_update;
    linuxInputApi.keyPressed = linux_key_pressed;

    input_set_api(&linuxInputApi);
}

#endif // PLATFORM_LINUX
