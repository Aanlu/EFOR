#include <string.h>
#include "core/app_state.h"

void init_app_state(AppState* state) {
    if (!state) return;

    memset(state->current_path, 0, sizeof(state->current_path));
    memset(state->target_path, 0, sizeof(state->target_path));
    memset(state->language_code, 0, sizeof(state->language_code));
    state->theme_color = 0xFFFFFF;
    state->running = 1;
    state->selectedFileIndex = 0;
    state->needs_reload = false;
    state->needs_sync = false;
    state->current_mode = STATE_NAVIGATION;
    memset(state->command_buffer, 0, sizeof(state->command_buffer));
    memset(state->pending_cmd, 0, sizeof(state->pending_cmd));
    state->buffer_len = 0;
}

void app_state_set_mode(AppState* state, AppStateMode new_mode) {
    if (!state) return;

    if (new_mode == STATE_COMMAND_INPUT || new_mode == STATE_NAVIGATION) {
        state->buffer_len = 0;
        memset(state->command_buffer, 0, sizeof(state->command_buffer));
    }

    state->current_mode = new_mode;
}