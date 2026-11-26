#include <string.h>
#include "core/app_state.h"

void init_app_state(AppState* state) {
    if (!state) return;

    memset(state->current_path, 0, sizeof(state->current_path));
    memset(state->target_path, 0, sizeof(state->target_path));
    memset(state->language_code, 0, sizeof(state->language_code));
    state->theme_color = 0xFFFFFF;
    state->running = 1;
    state->selected_file_index = 0;
    state->needs_reload = false;
    state->needs_sync = false;
    state->current_mode = STATE_NAVIGATION;
    memset(state->command_buffer, 0, sizeof(state->command_buffer));
    memset(state->pending_cmd, 0, sizeof(state->pending_cmd));
    state->buffer_len = 0;
    memset(state->status_message, 0, sizeof(state->status_message));
    state->status_is_error = false;
    state->input_failed = false;
}

void app_state_set_mode(AppState* state, AppStateMode new_mode) {
    if (!state) return;

    if (new_mode == STATE_COMMAND_INPUT || new_mode == STATE_NAVIGATION) {
        state->buffer_len = 0;
        memset(state->command_buffer, 0, sizeof(state->command_buffer));
    }

    state->current_mode = new_mode;
}

int app_state_clamp_index(int index, size_t count) {
    if (count == 0) return 0;
    if (index < 0) return 0;

    /* index is non-negative here, so the cast to size_t keeps its value */
    if ((size_t)index >= count) {
        /* count is bounded by MAX_FILES_PER_DIR, so it always fits in an int */
        return (int)(count - 1);
    }

    return index;
}

const char* app_state_footer_text(const AppState* state) {
    if (!state) return "";

    if (state->current_mode == STATE_COMMAND_INPUT) return state->command_buffer;
    if (state->current_mode == STATE_CONFIRMATION)return "Confirm? [Y/N]";
    if (state->status_message[0] != '\0') return state->status_message;

    return "Press ':' to enter a command.";
}
