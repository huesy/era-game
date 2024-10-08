#ifndef ENGINE_H
#define ENGINE_H

#include "platform/platform.h"

typedef void (*PluginFunc)(void);

typedef struct Plugin {
    void *handle;
    PluginFunc init;
    PluginFunc update;
    PluginFunc shutdown;
} Plugin;

/**
 * @brief Loads a plugin from the specified file.
 *
 * @param plugin Pointer to the plugin to load.
 * @param path The path of the plugin file.
 */
void load_plugin(Plugin *plugin, const char *path);

/**
 * @brief Unloads a plugin.
 *
 * @param plugin Pointer to the plugin to unload.
 */
void unload_plugin(Plugin *plugin);

/**
 * @brief Runs a plugin.
 *
 * @param plugin Pointer to the plugin to run.
 */
void run_plugin(Plugin *plugin);

#endif // ENGINE_H
