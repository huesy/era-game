#include "engine/ecs/system.h"
#include "engine/core/logging.h"

#define MAX_SYSTEMS 128

static SystemUpdateFn registered_systems[MAX_SYSTEMS];
static u32 system_count = 0;

void system_register(SystemUpdateFn update_fn) {
    if (system_count >= MAX_SYSTEMS) {
        log_error("Maximum number of systems reached.");
        return;
    }

    registered_systems[system_count++] = update_fn;
}

void system_update(f32 delta_time) {
    for (u32 i = 0; i < system_count; ++i) {
        registered_systems[i](delta_time);
    }
}
