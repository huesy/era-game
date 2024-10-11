#include "engine/plugin_manager.h"
#include "engine/memory.h"
#include "engine/plugin.h"

ENGINE_API void plugin_manager_init(PluginManager *manager) {
    manager->dense = (Plugin *)memory_allocate(sizeof(Plugin) * MAX_PLUGINS, MEMORY_TAG_PLUGIN);
    manager->sparse = (u32 *)memory_allocate(sizeof(u32) * MAX_PLUGINS, MEMORY_TAG_PLUGIN);
    manager->size = 0;
    manager->capacity = MAX_PLUGINS;

    memory_zero(manager->sparse, sizeof(u32) * MAX_PLUGINS);
    for (u32 i = 0; i < MAX_PLUGINS; ++i) {
        manager->sparse[i] = INVALID_ID_U32;
    }
}

ENGINE_API void plugin_manager_shutdown(PluginManager *manager) {
    for (u32 i = 0; i < manager->size; ++i) {
        Plugin *plugin = &manager->dense[i];
        if (plugin->shutdown) {
            plugin->shutdown();
        }

        if (plugin->handle) {
            platform_unload_library(plugin->handle);
        }
    }

    memory_free(manager->dense, sizeof(Plugin) * MAX_PLUGINS, MEMORY_TAG_PLUGIN);
    memory_free(manager->sparse, sizeof(u32) * MAX_PLUGINS, MEMORY_TAG_PLUGIN);
    manager->size = 0;
    manager->capacity = 0;
}

ENGINE_API b8 plugin_manager_load(PluginManager *manager, const char *path);
ENGINE_API void plugin_manager_unload(PluginManager *manager, const char *path);
ENGINE_API void plugin_manager_update(PluginManager *manager, f32 deltaTime);
ENGINE_API Plugin *plugin_manager_get(PluginManager *manager, u32 id);
