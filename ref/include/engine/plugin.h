#ifndef ENGINE_PLUGIN_H
#define ENGINE_PLUGIN_H

#include "defines.h"
#include "platform.h"

#define MAX_PLUGIN_NAME 256
#define MAX_PLUGINS 1024

typedef void (*PluginInitFunc)(void);
typedef void (*PluginUpdateFunc)(f32 deltaTime);
typedef void (*PluginShutdownFunc)(void);

typedef struct Plugin {
    char name[MAX_PLUGIN_NAME];
    void *handle;
    PluginInitFunc init;
    PluginUpdateFunc update;
    PluginShutdownFunc shutdown;
} Plugin;

#endif // ENGINE_PLUGIN_H
