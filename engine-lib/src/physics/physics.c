#include "physics/physics.h"

#include "core/logging.h"

#define MAX_RIGIDBODIES 1024

static Rigidbody rigidbodies[MAX_RIGIDBODIES];
static u32 rigidbodyCount = 0;

void physics_initialize(void) {
    rigidbodyCount = 0;
}

void physics_shutdown() {
    rigidbodyCount = 0;
}

void physics_update(f32 delta_time) {
    for (u32 i = 0; i < rigidbodyCount; ++i) {
        Rigidbody *rb = &rigidbodies[i];
        rb->x += rb->vx * delta_time;
        rb->y += rb->vy * delta_time;
        rb->z += rb->vz * delta_time;
    }
}
