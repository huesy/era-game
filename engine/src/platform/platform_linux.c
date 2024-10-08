#include "platform/platform.h"

#ifdef PLATFORM_LINUX

#    include "core/logging.h"
#    include <dlfcn.h>

static b8 isRunning = false;

u32 platform_init(PlatformCreateInfo *config) {
    if (!config) {
        log_error("Invalid platform configuration.");
        return 1;
    }

    // TODO: Linux-specific window creation code.
    log_info("Platform initialised with title: %s, width: %u, height: %u", config->title, config->width, config->height);

    isRunning = true;
    return 0;
}

void platform_shutdown(void) {
    // TODO: Linux-specific window shutdown code.
    log_info("Platform shutdown.");

    isRunning = false;
}

void platform_poll_events(void) {
    // TODO: Linux-specific event polling (e.g., X11 event loop).
    log_info("Polling events...");
}

b8 platform_is_running(void) {
    return isRunning;
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

#endif // PLATFORM_LINUX
