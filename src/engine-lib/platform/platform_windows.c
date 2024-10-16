#include "engine/platform.h"

#ifdef PLATFORM_WINDOWS
#    include "engine/logging.h"
#    include <windows.h>

typedef struct PlatformState {
    b8 isRunning;
    BITMAPINFO bitmapInfo;
    void *bitmapMemory;
    HBITMAP bitmapHandle;
    HDC bitmapDeviceContext;
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

/**
 * @brief Resizes the DIB (Device Independent Bitmap) section of the window.
 *
 * @param width The new width of the DIB section.
 * @param height The new height of the DIB section.
 */
internal void platform_resize_dib_section(i32 width, i32 height) {

    // TODO: Bulletproof this.
    // Maybe don't free first, free after, then free first is that fails.

    if (state.bitmapHandle) {
        // Free the existing bitmap handle.
        DeleteObject(state.bitmapHandle);
    }

    if (!state.bitmapDeviceContext) {
        // TODO: Should we recreate these under certain special circumstances?
        state.bitmapDeviceContext = CreateCompatibleDC(0);
    }

    state.bitmapInfo.bmiHeader.biSize = sizeof(state.bitmapInfo.bmiHeader);
    state.bitmapInfo.bmiHeader.biWidth = width;
    state.bitmapInfo.bmiHeader.biHeight = height;
    state.bitmapInfo.bmiHeader.biPlanes = 1;
    state.bitmapInfo.bmiHeader.biBitCount = 32;
    state.bitmapInfo.bmiHeader.biCompression = BI_RGB;

    state.bitmapHandle = CreateDIBSection(
        state.bitmapDeviceContext,
        &state.bitmapInfo,
        DIB_RGB_COLORS,
        &state.bitmapMemory,
        0, 0);
}

internal void platform_update_window(HDC deviceContext, i32 x, i32 y, i32 width, i32 height) {
    StretchDIBits(deviceContext,
                  x, y, width, height,
                  x, y, width, height,
                  state.bitmapMemory,
                  &state.bitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK platform_window_proc(
    HWND window,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam) {

    LRESULT result = 0;

    switch (msg) {
        case WM_SIZE: {
            RECT clientRect;
            GetClientRect(window, &clientRect);
            u32 width = clientRect.right - clientRect.left;
            u32 height = clientRect.bottom - clientRect.top;
            platform_resize_dib_section(width, height);
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_CLOSE: {
            state.isRunning = false;
        } break;

        case WM_DESTROY: {
            state.isRunning = false;
        } break;

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC deviceContext = BeginPaint(window, &ps);
            i32 x = ps.rcPaint.left;
            i32 y = ps.rcPaint.top;
            i32 width = ps.rcPaint.right - ps.rcPaint.left;
            i32 height = ps.rcPaint.bottom - ps.rcPaint.top;
            platform_update_window(deviceContext, x, y, width, height);
            EndPaint(window, &ps);

            ENGINE_UNUSED(deviceContext);
        } break;

        default: {
            result = DefWindowProc(window, msg, wParam, lParam);
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
    windowClass.lpfnWndProc = platform_window_proc;
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
