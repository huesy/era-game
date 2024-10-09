#ifndef ENGINE_H
#define ENGINE_H

#include "defines.h"

typedef struct EngineConfig {
    u64 memoryPoolSize;
} EngineConfig;

ENGINE_API b8 engine_init(EngineConfig *config);
ENGINE_API void engine_shutdown(void);

#endif // ENGINE_H
