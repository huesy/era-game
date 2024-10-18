#include <assert.h>
#include <engine/logging.h>
#include <engine/memory.h>
#include <engine/platform.h>

void test_platform_initialization(void) {
    MemoryPool pool;
    u64 poolSize = 1024 * 1024 * 1; // 1MB

    // Initialize memory pool.
    assert(memory_pool_init(&pool, poolSize) == ENGINE_SUCCESS);

    // Initialize Platform.
    Platform platform = {0};
    assert(platform_init(&platform, &pool) == ENGINE_SUCCESS);

    // Check if the platform is running.
    assert(platform_is_running(&platform) == true);

    // Shutdown Platform.
    platform_shutdown(&platform);

    // Shutdown memory pool.
    memory_pool_shutdown(&pool);

    log_info("Platform unit tests passed.");
}

int main(void) {
    test_platform_initialization();
    return 0;
}
