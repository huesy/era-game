#include <plugin.h>
#include <stdio.h>

void plugin_init(void) {
    printf("Editor plugin initialized.");
}

void plugin_update(void) {
    printf("Editor plugin updating.");
}

void plugin_shutdown(void) {
    printf("Editor plugin shutting down.");
}
