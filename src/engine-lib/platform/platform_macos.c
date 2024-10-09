#include "engine/platform.h"

#ifdef PLATFORM_MACOS
#    include "engine/logging.h"
#    include <dlfcn.h>
#    include <mach/mach_time.h>

typedef struct PlatformState {
    b8 isRunning;
} PlatformState;

static PlatformState state = {0};

EngineResult platform_init(PlatformConfig *config) {
    if (!config) {
        log_error("Invalid platform configuration.");
        return ENGINE_FAILURE;
    }

    // TODO: macOS-specific window creation code.
    log_info("Platform initialized with title: %s, width: %u, height: %u", config->title, config->width, config->height);

    state.isRunning = true;

    return ENGINE_SUCCESS;
}

void platform_shutdown(void) {
    // TODO: macOS-specific window shutdown code.
    log_info("Platform shutdown.");

    state.isRunning = false;
}

void platform_poll_events(void) {
    // TODO: macOS-specific event polling (e.g., NSApplication run loop).
    log_info("Polling events...");

    // TODO: Setting false to exit loop after one iteration for now.
    state.isRunning = false;
}

b8 platform_is_running(void) {
    return state.isRunning;
}

void *platform_load_library(const char *path) {
    if (!path) {
        log_error("Invalid library path.");
        return NULL;
    }

    return dlopen(path, RTLD_LAZY);
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

    return dlsym(library, functionName);
}

void platform_unload_library(void *library) {
    if (!library) {
        log_error("Invalid library handle.");
        return;
    }

    dlclose(library);
}

f32 platform_get_absolute_time(void) {
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

#endif // PLATFORM_MACOS
