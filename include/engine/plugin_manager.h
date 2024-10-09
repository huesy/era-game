#ifndef ENGINE_PLUGIN_MANAGER_H
#define ENGINE_PLUGIN_MANAGER_H

#include "defines.h"
#include "memory.h"
#include "platform.h"
#include "plugin.h"

typedef struct PluginManager {
    Plugin *dense;
    u32 *sparse;
    u32 size;
    u32 capacity;
} PluginManager;

ENGINE_API void plugin_manager_init(PluginManager *manager);
ENGINE_API void plugin_manager_shutdown(PluginManager *manager);
ENGINE_API b8 plugin_manager_load(PluginManager *manager, const char *path);
ENGINE_API void plugin_manager_unload(PluginManager *manager, const char *path);
ENGINE_API void plugin_manager_update(PluginManager *manager, f32 deltaTime);
ENGINE_API Plugin *plugin_manager_get(PluginManager *manager, u32 id);

#endif // ENGINE_PLUGIN_MANAGER_H
