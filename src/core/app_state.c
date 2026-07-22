#include <stddef.h>
#include "core/app_state.h"

void init_app_state(AppState* state){
    if (!state) return;

    state->current_path[0] = '\0';
    state->language_code[0] = 'e';
    state->language_code[1] = 's';
    state->language_code[2] = '\0';
    state->theme_color = 0xFFFFFF; // Default to white color
    state->running = 1;
    state->selectedIndex = 0;
    state->selectedFileIndex = 0;
    state->modeCommand = false;
    state->needs_reload = false;
}