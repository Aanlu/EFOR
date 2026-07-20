#include <stdio.h>
#include "core/app_state.h"


int main() {
    AppState state;
    init_app_state(&state);

    printf("Current Path: %s\n", state.current_path);
    printf("Language Code: %s\n", state.language_code);
    printf("Theme Color: #%06X\n", state.theme_color);
    printf("Running: %d\n", state.running);

    return 0;
}

