// #include "engine.h"
// #include "game.h"
#include <stdio.h>

int main(void) {
    printf("Game started\n");
    // gameInit();
    return 0;
}

// #include <windows.h>

// typedef void (*EngineInitFunc)(void);

// // void gameInit() { printf("Game function called\n"); }

// int main(void) {

//     HMODULE hEngine;
//     EngineInitFunc pEngineInit;

//     printf("Game started\n");

//     // Load the engine DLL
//     hEngine = LoadLibrary("engine.dll");
//     if (hEngine == NULL) {
//         printf("Failed to load engine.dll\n");
//         return 1;
//     }

//     // Get the address of the EngineInit function.
//     pEngineInit = (EngineInitFunc)GetProcAddress(hEngine, "EngineInit");
//     if (pEngineInit == NULL) {
//         printf("Failed to get address of EngineInit\n");
//         FreeLibrary(hEngine);
//         return 1;
//     }

//     // Call the EngineInit function.
//     pEngineInit();
//     // gameInit();

//     // Unload the engine DLL
//     FreeLibrary(hEngine);

//     return 0;
// }
