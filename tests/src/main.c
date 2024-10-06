#include "ecs/component.h"
#include "ecs/entity.h"
#include "input/input.h"
#include "physics/physics.h"
#include "render/renderer.h"
#include "scene/scene_manager.h"
#include <stdio.h>

void render_test(void);
void physics_test(void);
void ecs_test(void);
void input_test(void);

int main(void) {
    printf("Testbed for Era Engine\n");
    printf("Select an option to test:\n");
    printf("1. Rendering\n");
    printf("2. Physics\n");
    printf("3. ECS\n");
    printf("4. Input\n");
    printf("5. Scene Management\n");
    printf("6. All\n");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        render_test();
        break;
    case 2:
        physics_test();
        break;
    case 3:
        ecs_test();
        break;
    case 4:
        input_test();
        break;
    case 5:
        scene_manager_test();
        break;
    case 6:
        render_test();
        physics_test();
        ecs_test();
        input_test();
        scene_manager_test();
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }

    return 0;
}
