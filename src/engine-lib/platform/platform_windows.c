#include "engine/platform.h"

#ifdef PLATFORM_WINDOWS
#    include "engine/logging.h"
#    include <windows.h>

typedef struct PlatformState {
    b8 isRunning;
} PlatformState;

static PlatformState state = {0};

ENGINE_API EngineResult platform_init(PlatformConfig *config) {
    if (!config) {
        log_error("Invalid platform configuration.");
        return ENGINE_ERROR;
    }

    state.isRunning = true;

    return ENGINE_SUCCESS;
}

ENGINE_API void platform_shutdown(void) {
    // TODO: Windows-specific window shutdown code.
    MessageBoxA(NULL, "Platform shutdown.", "Platform", MB_OK);

    state.isRunning = false;
}

ENGINE_API void platform_poll_events(void) {
    // TODO: Windows-specific event polling (e.g., PeekMessage or GetMessage loop).
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            state.isRunning = false;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

ENGINE_API b8 platform_is_running(void) {
    return state.isRunning;
}

// =============================================================================
// Windowing

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam) {

    LRESULT result = 0;

    switch (msg) {
        case WM_SIZE: {
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_CLOSE: {
            OutputDebugStringA("WM_CLOSE\n");
        } break;

        case WM_DESTROY: {
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC deviceContext = BeginPaint(hwnd, &ps);
            i32 x = ps.rcPaint.left;
            i32 y = ps.rcPaint.top;
            i32 width = ps.rcPaint.right - ps.rcPaint.left;
            i32 height = ps.rcPaint.bottom - ps.rcPaint.top;
            PatBlt(deviceContext, x, y, width, height, BLACKNESS);
            EndPaint(hwnd, &ps);

            ENGINE_UNUSED(deviceContext);
        } break;

        default: {
            result = DefWindowProc(hwnd, msg, wParam, lParam);
        } break;
    }

    return result;
}

ENGINE_API EngineResult platform_create_window(WindowConfig *config, Window *window) {
    if (!config) {
        log_error("Invalid window configuration.");
        return ENGINE_FAILURE;
    }

    log_info("Creating window: %dx%d - %s", config->width, config->height, config->title);

    WNDCLASS windowClass = {0};
    // TODO(Andrew): Check if HREDRAW and VREDRAW are needed.
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = GetModuleHandle(NULL);
    // windowClass.hIcon;
    windowClass.lpszClassName = "EngineWindowClass";

    if (RegisterClass(&windowClass) == 0) {
        log_error("Failed to register window class.");
        return ENGINE_FAILURE;
    }

    HWND windowHandle = CreateWindowEx(
        0,
        windowClass.lpszClassName,
        config->title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        config->width,
        config->height,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    if (windowHandle == NULL) {
        log_error("Failed to create window.");
        return ENGINE_FAILURE;
    }

    window->handle = windowHandle;

    return ENGINE_SUCCESS;
}

ENGINE_API void platform_destroy_window(Window *window);
ENGINE_API u32 platform_get_window_width(void *window);
ENGINE_API u32 platform_get_window_height(void *window);
ENGINE_API void platform_set_window_title(void *window, const char *title);
ENGINE_API b8 platform_is_window_open(void *window);

// =============================================================================
// Dynamic Library Loading

ENGINE_API void *platform_load_library(const char *path) {
    if (!path) {
        log_error("Invalid library path.");
        return NULL;
    }

    return (void *)LoadLibraryA(path);
}

ENGINE_API void *platform_get_library_function(void *library, const char *functionName) {
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

ENGINE_API void platform_unload_library(void *library) {
    if (!library) {
        log_error("Invalid library handle.");
        return;
    }

    FreeLibrary((HMODULE)library);
}

ENGINE_API f32 platform_get_absolute_time(void) {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return (f32)counter.QuadPart / (f32)frequency.QuadPart;
}

// =============================================================================
// Memory

void *platform_memory_allocate(u64 size, u16 alignment) {
    ENGINE_UNUSED(alignment);
    return (void *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
}

void platform_memory_free(void *block) {
    HeapFree(GetProcessHeap(), 0, block);
}

void *platform_memory_copy(void *dest, const void *src, u64 size) {
    return memcpy(dest, src, size);
}

void *platform_memory_set(void *dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void *platform_memory_zero(void *block, u64 size) {
    return memset(block, 0, size);
}

// =============================================================================
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
