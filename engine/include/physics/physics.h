#ifndef ENGINE_PHYSICS_H
#define ENGINE_PHYSICS_H

#include "defines.h"

typedef struct Rigidbody {
    /** @brief X position of the rigidbody. */
    f32 x;
    /** @brief Y position of the rigidbody. */
    f32 y;
    /** @brief Z position of the rigidbody. */
    f32 z;
    /** @brief Mass of the rigidbody. */
    f32 mass;
    /** @brief Velocity in the X direction. */
    f32 vx;
    /** @brief Velocity in the Y direction. */
    f32 vy;
    /** @brief Velocity in the Z direction. */
    f32 vz;
} Rigidbody;

/**
 * @brief Initializes the physics system.
 */
void physics_initialize(void);

/**
 * @brief Shuts down the physics system.
 */
void physics_shutdown();

/**
 * @brief Updates the physics system.
 */
void physics_update(f32 delta_time);

#endif // ENGINE_PHYSICS_H
