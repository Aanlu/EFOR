#include <stdbool.h>
#include <stdio.h>

#include "core/app_controller.h"

bool handle_input(AppState* state, const FSDirectoryContent* content) {
    if (!state || !content) return false;

    KeyAction action = input_get_action();

    if (action == KEY_ACTION_NONE) {
        return false;
    }

    int max_file_index = (content->count > 0) ? (int)content->count - 1 : 0;

    switch (action) {
        case KEY_ACTION_UP:
            if (state->selectedFileIndex > 0) {
                state->selectedFileIndex--;
                return true;
            }
            return false;

        case KEY_ACTION_DOWN:
            if (state->selectedFileIndex < max_file_index) {
                state->selectedFileIndex++;
                return true;
            }
            return false;

        case KEY_ACTION_LEFT:
            if (state->selectedIndex > 0) {
                state->selectedIndex--;
                state->needs_reload = true;
                return true;
            }
            return false;

        case KEY_ACTION_RIGHT:
            state->selectedIndex++;
            state->needs_reload = true;
            return true;

        case KEY_ACTION_ENTER:
            if (state->modeCommand) {
                return false;
            }
            state->needs_reload = true;
            return true;

        case KEY_ACTION_ESCAPE:
            printf("Escape key pressed. Exiting the application.\n");
            state->running = 0;
            return true;

        default:
            return false;
    }
}