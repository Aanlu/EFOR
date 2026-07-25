#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "core/app_controller.h"

static void navigate_into_dir(AppState* state, const FSDirectoryContent* content);
static void navigate_back_dir(AppState* state);

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
            navigate_back_dir(state);
            state->needs_reload = true;
            return true;

        case KEY_ACTION_RIGHT:
            navigate_into_dir(state, content);
            state->needs_reload = true;
            return true;

        case KEY_ACTION_ENTER:
            if (state->current_mode == STATE_COMMAND_INPUT) {
                return false;
            }
            navigate_into_dir(state, content);
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

void app_sync_filesystem(AppState* state, FSDirectoryContent* content){
    if (!state || !content) return;

    if (state->needs_sync){
        if (fs_read_directory(state->target_path, content) == 0){
            strncpy (state->current_path, state->target_path, MAX_PATH_LENGTH);
            state->selectedFileIndex = 0;
        }else{
            strncpy(state->target_path, state->current_path, MAX_PATH_LENGTH);
            fs_read_directory(state->current_path, content);
        }
        state->needs_sync =false;
    }else if (state->needs_reload){
        fs_read_directory(state->current_path, content);
        state->needs_reload = false;
    }
}

void navigate_into_dir(AppState* state, const FSDirectoryContent* content) {
    if (!state || !content) return;

    if (state->selectedFileIndex < 0 || state->selectedFileIndex >= (int)content->count) {
        return;
    }

    const FileInfo* selectedItem = &content->items[state->selectedFileIndex];
    if (selectedItem->isDirectory) {

        char new_path[MAX_PATH_LENGTH];
        size_t current_path_len = strlen(state->current_path);

        if (current_path_len > 0 && state->current_path[current_path_len - 1] != '\\') {
            snprintf(new_path, sizeof(new_path), "%s\\%s", state->current_path, selectedItem->name);
        } else {
            snprintf(new_path, sizeof(new_path), "%s%s", state->current_path, selectedItem->name);
        }

        strncpy(state->target_path, new_path, MAX_PATH_LENGTH - 1);
        state->target_path[MAX_PATH_LENGTH - 1] = '\0';

        state->selectedFileIndex = 0;
        state->needs_sync = true;
    }
}

void navigate_back_dir(AppState* state) {
    if (!state) return;

    strncpy(state->target_path, state->current_path, MAX_PATH_LENGTH);

    char* last_backslash = strrchr(state->target_path, '\\');

    if (last_backslash){
        size_t index = last_backslash - state->target_path;

        if (index == 2 && *(last_backslash + 1) == '\0'){
            return;
        }

        if (index == 2) {
            *(last_backslash + 1) = '\0';
            state->needs_sync = true;
        }
        else if (index > 2) {
            *last_backslash = '\0';
            state->needs_sync = true;
        }
    }
}