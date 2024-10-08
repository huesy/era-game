#include "platform/platform.h"

#ifdef PLATFORM_WINDOWS
#    include "core/logging.h"
#    include <windows.h>

static b8 isRunning = false;

u32 platform_init(PlatformCreateInfo *config) {
    if (!config) {
        log_error("Invalid platform configuration.");
        return 1;
    }

    // TODO: Windows-specific window creation code.
    MessageBoxA(NULL, "Platform initialized.", "Platform", MB_OK);

    isRunning = true;
    return 0;
}

void platform_shutdown(void) {
    // TODO: Windows-specific window shutdown code.
    MessageBoxA(NULL, "Platform shutdown.", "Platform", MB_OK);

    isRunning = false;
}

void platform_poll_events(void) {
    // TODO: Windows-specific event polling (e.g., PeekMessage or GetMEssage loop).
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            isRunning = false;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

b8 platform_is_running(void) {
    return isRunning;
}

void *platform_load_library(const char *path) {
    if (!path) {
        log_error("Invalid library path.");
        return NULL;
    }

    return (void *)LoadLibraryA(path);
}

void *platform_get_library_function(void *library, const char *functionName) {
    if (!library) {
        log_error("Invalid library handle.");
        return NULL;
    }

    if (!functionName) {
        log_error("Invalid function name.");
        return NULL;
    }

    return (void *)GetProcAddress((HMODULE)library, functionName);
}

void platform_unload_library(void *library) {
    if (!library) {
        log_error("Invalid library handle.");
        return;
    }

    FreeLibrary((HMODULE)library);
}

#endif // PLATFORM_WINDOWS
