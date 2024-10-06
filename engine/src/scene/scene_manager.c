#include "scene/scene_manager.h"
#include "core/logging.h"

void scene_manager_load_scene(const char *scene_name) {
    log_info("Loading scene: %s", scene_name);
}

void scene_manager_unload_scene(void) {
    log_info("Unloading scene");
}
