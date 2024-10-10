#include "engine/platform.h"

#ifdef PLATFORM_WINDOWS
#    include "engine/logging.h"
#    include <windows.h>

static b8 isRunning = false;

EngineResult platform_init(PlatformConfig *config) {
    if (!config) {
        log_error("Invalid platform configuration.");
        return ENGINE_FAILURE;
    }

    // TODO: Windows-specific window creation code.
    MessageBoxA(NULL, "Platform initialized.", "Platform", MB_OK);

    isRunning = true;
    return ENGINE_SUCCESS;
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

f32 platform_get_absolute_time(void) {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return (f32)counter.QuadPart / (f32)frequency.QuadPart;
}

// Threading

void platform_mutex_init(void *lock) {
    if (!lock) {
        log_error("Invalid mutex lock.");
        return;
    }

    InitializeCriticalSection((CRITICAL_SECTION *)lock);
}

void platform_mutex_lock(void *lock) {
    if (!lock) {
        log_error("Invalid mutex lock.");
        return;
    }

    EnterCriticalSection((CRITICAL_SECTION *)lock);
}

void platform_mutex_unlock(void *lock) {
    if (!lock) {
        log_error("Invalid mutex lock.");
        return;
    }

    LeaveCriticalSection((CRITICAL_SECTION *)lock);
}

void platform_mutex_destroy(void *lock) {
    if (!lock) {
        log_error("Invalid mutex lock.");
        return;
    }

    DeleteCriticalSection((CRITICAL_SECTION *)lock);
}

#endif // PLATFORM_WINDOWS
