#include "physics/physics.h"
#include <stdio.h>

void physics_test(void) {
    physics_initialize();
    Rigidbody rb = {0};
    rb.x = 0.0f;
    rb.y = 0.0f;
    rb.z = 0.0f;
    rb.mass = 1.0f;
    rb.vx = 1.0f;
    rb.vy = 1.0f;
    rb.vz = 1.0f;
    printf("Physics Test: Simulating rigidbody movement...\n");
    for (int i = 0; i < 10; ++i) {
        physics_update(0.016f); // Simulate 60 FPS
        printf("Entity Position: (%f, %f)\n", rb.x, rb.y);
    }
    physics_shutdown();
}
