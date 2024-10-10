#include "engine/platform.h"

#ifdef PLATFORM_LINUX
#    include "engine/logging.h"
#    include <dlfcn.h>
#    include <pthread.h>
#    include <time.h>

static b8 isRunning = false;

EngineResult platform_init(PlatformConfig *config) {
    if (!config) {
        log_error("Invalid platform configuration.");
        return ENGINE_FAILURE;
    }

    // TODO: Linux-specific window creation code.
    log_info("Platform initialized with title: %s, width: %u, height: %u", config->title, config->width, config->height);

    isRunning = true;
    return ENGINE_SUCCESS;
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

f32 platform_get_absolute_time(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (f32)ts.tv_sec + (f32)ts.tv_nsec / 1.0e9f;
}

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

#endif // PLATFORM_LINUX
