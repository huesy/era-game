#include "engine/input/input.h"
#include "engine/logging.h"

static InputAPI *currentInputApi = NULL;

void input_set_api(InputAPI *api) {
    currentInputApi = api;
}

void input_initialize(void) {
    if (currentInputApi && currentInputApi->initialize) {
        currentInputApi->initialize();
    }
}

void input_shutdown(void) {
    if (currentInputApi && currentInputApi->shutdown) {
        currentInputApi->shutdown();
    }
}

void input_update(void) {
    if (currentInputApi && currentInputApi->update) {
        currentInputApi->update();
    }
}

u8 input_key_pressed(InputKey key) {
    if (currentInputApi && currentInputApi->keyPressed) {
        return currentInputApi->keyPressed(key);
    }

    return 0;
}
