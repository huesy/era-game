#include <core/logging.h>

void plugin_init(void) {
    log_info("Game plugin initialized.");
}

void plugin_update(void) {
    log_info("Game plugin updated.");
}

void plugin_shutdown(void) {
    log_info("Game plugin shutdown.");
}
