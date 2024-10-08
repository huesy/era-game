// #include "input/input.h"
// #ifdef PLATFORM_MACOS
// #    include <Carbon/Carbon.h>

// static u8 keyStates[KEY_MAX];

// static void macos_input_initialize() {
//     // TODO: Custom initialization for macOS input.
// }

// static void macos_input_shutdown() {
//     // TODO: Custom shutdown for macOS input.
// }

// static void macos_input_update() {
//     for (InputKey key = 0; key < KEY_MAX; ++key) {
//         keyStates[key] = (GetCurrentKeyModifiers() & key) != 0;
//     }
// }

// static u8 macos_key_pressed(InputKey key) {
//     return keyStates[key];
// }

// static InputAPI macosInputApi = {0};

// void input_macos_set_api() {
//     macosInputApi.initialize = macos_input_initialize;
//     macosInputApi.shutdown = macos_input_shutdown;
//     macosInputApi.update = macos_input_update;
//     macosInputApi.keyPressed = macos_key_pressed;

//     input_set_api(&macosInputApi);
// }

// #endif // PLATFORM_MACOS
