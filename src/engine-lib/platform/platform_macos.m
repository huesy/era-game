#include "engine/platform.h"

#ifdef PLATFORM_MACOS
#    include "engine/logging.h"
#    include <Cocoa/Cocoa.h>
#    include <dlfcn.h>
#    include <mach/mach_time.h>
#    include <pthread.h>
#    include <stdlib.h>

typedef struct PlatformState {
    b8 isRunning;
    NSWindow *window;
} PlatformState;

static PlatformState state = {0};

@interface EngineAppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation EngineAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    log_info("Application did finish launching.");
    state.isRunning = true;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    state.isRunning = false;
    return YES;
}
@end

ENGINE_API EngineResult platform_init(PlatformConfig *config) {
    if (!config) {
        log_error("Invalid platform configuration.");
        return ENGINE_ERROR;
    }

    state.isRunning = true;

    return ENGINE_SUCCESS;
}

ENGINE_API void platform_shutdown(void) {
    // TODO: macOS-specific window shutdown code.
    log_info("Platform shutdown.");

    state.isRunning = false;
}

ENGINE_API void platform_poll_events(void) {
    // macOS-specific event polling using the NSApplication loop.
    NSEvent *event;
    while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES])) {
        [NSApp sendEvent:event];
        [NSApp updateWindows];
    }
}

ENGINE_API b8 platform_is_running(void) {
    return state.isRunning;
}

ENGINE_API f32 platform_get_absolute_time(void) {
    static mach_timebase_info_data_t timebase;
    static bool initialized = false;

    if (!initialized) {
        mach_timebase_info(&timebase);
        initialized = true;
    }

    uint64_t time = mach_absolute_time();
    f64 seconds = (f64)time * (f64)timebase.numer / (f64)timebase.denom / 1e9;

    return (f32)seconds;
}

// =============================================================================
// Windowing

ENGINE_API void *platform_create_window(WindowConfig *config) {
    if (!config) {
        log_error("Invalid window configuration.");
        return NULL;
    }

    // macOS-specific window creation code.
    log_info("Creating window: %dx%d - %s", config->width, config->height, config->title);

    // Create an application instance.
    [NSApplication sharedApplication];

    // Create a window with specific dimensions and properties.
    static EngineAppDelegate *delegate = nil;
    if (!delegate) {
        delegate = [[EngineAppDelegate alloc] init];
        [NSApp setDelegate:delegate];
    }

    // Define the window's frame.
    NSRect frame = NSMakeRect(0, 0, config->width, config->height);
    NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

    // Create the window.
    NSWindow *window = [[NSWindow alloc] initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];

    [window setTitle:@(config->title)];
    [window makeKeyAndOrderFront:nil];

    // Activate the application and bring the window to the front.
    [NSApp activateIgnoringOtherApps:YES];

    // Store the window reference in the platform state.
    state.window = window;
    
    // Start the application's main event loop if not already started.
    [NSApp run];

    return (void* )window;
}

ENGINE_API void platform_destroy_window(void *window) {
    if (window) {
        NSWindow *nsWindow = (__bridge NSWindow *)window;
        [nsWindow close];
    }

    state.window = nil;
    state.isRunning = false;
}

ENGINE_API u32 platform_get_window_width(void *window) {
    if (!window) {
        log_error("Invalid window handle.");
        return 0;
    }

    NSWindow *nsWindow = (__bridge NSWindow *)window;
    NSRect frame = [nsWindow frame];

    return (u32)frame.size.width;
}

ENGINE_API u32 platform_get_window_height(void *window) {
    if (!window) {
        log_error("Invalid window handle.");
        return 0;
    }

    NSWindow *nsWindow = (__bridge NSWindow *)window;
    NSRect frame = [nsWindow frame];

    return (u32)frame.size.height;
}

ENGINE_API void platform_set_window_title(void *window, const char *title) {
    if (!window) {
        log_error("Invalid window handle.");
        return;
    }

    if (!title) {
        log_error("Invalid window title.");
        return;
    }

    NSWindow *nsWindow = (__bridge NSWindow *)window;
    [nsWindow setTitle:@(title)];
}

ENGINE_API b8 platform_is_window_open(void *window) {
    if (!window) {
        log_error("Invalid window handle.");
        return false;
    }

    NSWindow *nsWindow = (__bridge NSWindow *)window;
    return [nsWindow isVisible];
}

// =============================================================================
// Dynamic Library Loading

ENGINE_API void *platform_load_library(const char *path) {
    if (!path) {
        log_error("Invalid library path.");
        return NULL;
    }

    return dlopen(path, RTLD_LAZY);
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

    return dlsym(library, functionName);
}

ENGINE_API void platform_unload_library(void *library) {
    if (!library) {
        log_error("Invalid library handle.");
        return;
    }

    dlclose(library);
}

// =============================================================================
// Memory

void *platform_memory_allocate(u64 size, u16 alignment) {
    ENGINE_UNUSED(alignment);
    return malloc(size);
}

void platform_memory_free(void *block) {
    free(block);
}

void *platform_memory_copy(void *dest, const void *src, u64 size) {
    return memcpy(dest, src, size);
}

void *platform_memory_set(void *dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void *platform_memory_zero(void *block, u64 size) {
    return platform_memory_set(block, 0, size);
}

// =============================================================================
// Threading

void platform_mutex_init(void *lock) {
    if (!lock) {
        log_error("Invalid mutex lock.");
        return;
    }

    pthread_mutex_init((pthread_mutex_t *)lock, NULL);
}

void platform_mutex_lock(void *lock) {
    if (!lock) {
        log_error("Invalid mutex lock.");
        return;
    }

    pthread_mutex_lock((pthread_mutex_t *)lock);
}

void platform_mutex_unlock(void *lock) {
    if (!lock) {
        log_error("Invalid mutex lock.");
        return;
    }

    pthread_mutex_unlock((pthread_mutex_t *)lock);
}

void platform_mutex_destroy(void *lock) {
    if (!lock) {
        log_error("Invalid mutex lock.");
        return;
    }

    pthread_mutex_destroy((pthread_mutex_t *)lock);
}

#endif // PLATFORM_MACOS
