#include "game/game.h"
#include <stdio.h>

ENGINE_API b8 game_init(void) {
    printf("Game initialized.\n");
    return true;
}

ENGINE_API void game_shutdown(void) {
    printf("Game shutdown.\n");
}

ENGINE_API void game_update(f32 deltaTime) {
    printf("Game update: %f\n", deltaTime);
}

ENGINE_API void game_render(void) {
    printf("Game render.\n");
}
