#include "engine.h"
#include "core/logging.h"
#include "platform/platform.h"
#include <stdio.h>

void load_plugin(Plugin *plugin, const char *path) {
    plugin->handle = platform_load_library(path);
    if (!plugin->handle) {
        log_error("Failed to load plugin from path: %s", path);
        return;
    }

    plugin->init = (PluginFunc)platform_get_library_function(plugin->handle, "plugin_init");
    plugin->update = (PluginFunc)platform_get_library_function(plugin->handle, "plugin_update");
    plugin->shutdown = (PluginFunc)platform_get_library_function(plugin->handle, "plugin_shutdown");

    if (plugin->init && plugin->update && plugin->shutdown) {
        log_info("Plugin loaded successfully: %s", path);
    } else {
        log_error("Failed to load plugin functions from path: %s", path);
        platform_unload_library(plugin->handle);
        plugin->handle = NULL;
    }
}

void unload_plugin(Plugin *plugin) {
    if (plugin->handle) {
        plugin->shutdown();
        platform_unload_library(plugin->handle);
        plugin->handle = NULL;
        log_debug("Plugin unloaded successfully.");
    }
}

void run_plugin(Plugin *plugin) {
    if (plugin->init) {
        plugin->init();
    }

    while (platform_is_running()) {
        if (plugin->update) {
            plugin->update();
        }

        platform_poll_events();
    }

    if (plugin->shutdown) {
        plugin->shutdown();
    }
}
