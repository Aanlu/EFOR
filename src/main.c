#include <stdio.h>
#include "core/app_state.h"

void init_app_state(AppState* state){
    if (!state) return;

    state->current_path[0] = '\0';
    state->language_code[0] = 'e';
    state->language_code[1] = 's';
    state->language_code[2] = '\0';
    state->theme_color = 0xFFFFFF; // Default to white color
    state->running = 1;
}

int main() {
    AppState state;
    init_app_state(&state);

    printf("Current Path: %s\n", state.current_path);
    printf("Language Code: %s\n", state.language_code);
    printf("Theme Color: #%06X\n", state.theme_color);
    printf("Running: %d\n", state.running);
    
    return 0;
}

