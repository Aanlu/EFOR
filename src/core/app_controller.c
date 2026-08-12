#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core/app_state.h"
#include "core/interfaces/fs_reader.h"
#include "core/app_controller.h"
#include "core/interfaces/input.h"
#include "parsers/command_parser.h"
#include "core/command_executor.h"

static void navigate_into_dir(AppState* state, const FSDirectoryContent* content);
static void navigate_back_dir(AppState* state);

static bool handle_navigation_input(AppState* state, const FSDirectoryContent* content, InputEvent event);
static bool handle_command_input(AppState* state, InputEvent event);
static bool handle_confirmation_input(AppState* state, InputEvent event);

bool handle_input(AppState* state, const FSDirectoryContent* content) {
    if (!state || !content) return false;

    InputEvent event = input_get_action();
    if (event.action == KEY_ACTION_NONE) return false;

    switch(state->current_mode) {
        case STATE_NAVIGATION:
            return handle_navigation_input(state, content, event);

        case STATE_COMMAND_INPUT:
            return handle_command_input(state, event);

        case STATE_CONFIRMATION:
            return handle_confirmation_input(state, event);

        case STATE_PROCESSING:
            return false;

        case STATE_EXITING:
            state->running = 0;
            return true;

        default:
            return false;
    }
}

static bool handle_navigation_input(AppState* state, const FSDirectoryContent* content, InputEvent event) {
    int max_file_index = (content->count > 0) ? (int)content->count - 1 : 0;

    switch (event.action) {
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
            return true;

        case KEY_ACTION_RIGHT:
        case KEY_ACTION_ENTER:
            navigate_into_dir(state, content);
            return true;

        case KEY_ACTION_ESCAPE:
            app_state_set_mode(state, STATE_EXITING);
            state->running = 0;
            return true;

        case KEY_ACTION_TEXT:
            if (event.character == ':' && state->buffer_len == 0) {
                app_state_set_mode(state, STATE_COMMAND_INPUT);
                state->command_buffer[0] = ':';
                state->command_buffer[1] = '\0';
                state->buffer_len = 1;
                return true;
            }
            return false;

        default:
            return false;
    }
}

static bool handle_command_input(AppState* state, InputEvent event) {
    if (!state) return false;

    switch(event.action) {
        case KEY_ACTION_ESCAPE:
            app_state_set_mode(state, STATE_NAVIGATION);
            return true;
        
        case KEY_ACTION_BACKSPACE:
            if (state->buffer_len > 1) {
                state->buffer_len--;
                state->command_buffer[state->buffer_len] = '\0';
                return true;
            } else if (state->buffer_len == 1) {
                app_state_set_mode(state, STATE_NAVIGATION);
                return true;
            }
            return false;
        
        case KEY_ACTION_TEXT:
            if (state->buffer_len < 254) {
                state->command_buffer[state->buffer_len] = event.character;
                state->buffer_len++;
                state->command_buffer[state->buffer_len] = '\0';
                return true;
            }
            return false;
        
        case KEY_ACTION_ENTER: {
            ParsedCommand lineCommand = command_parser(state->command_buffer, state->buffer_len);

            if (lineCommand.is_destructive) {
                snprintf(state->pending_cmd, sizeof(state->pending_cmd), "%s", state->command_buffer);
                app_state_set_mode(state, STATE_CONFIRMATION);
            } else {
                app_state_set_mode(state, STATE_PROCESSING);
                execute_command(state, &lineCommand);
                app_state_set_mode(state, STATE_NAVIGATION);
            }
            return true;
        }

        default:
            return false;
    }
}

static bool handle_confirmation_input(AppState* state, InputEvent event) {
    if (!state) return false;

    if (event.action == KEY_ACTION_TEXT) {
        char c = (char)tolower((unsigned char)event.character);
        if (c == 'y') {
            app_state_set_mode(state, STATE_PROCESSING);
            ParsedCommand lineCommand = command_parser(state->pending_cmd, strlen(state->pending_cmd));
            execute_command(state, &lineCommand);
            memset(state->pending_cmd, 0, sizeof(state->pending_cmd));
            app_state_set_mode(state, STATE_NAVIGATION);
            return true;
        } else if (c == 'n') {
            memset(state->pending_cmd, 0, sizeof(state->pending_cmd));
            app_state_set_mode(state, STATE_NAVIGATION);
            return true;
        }
    } else if (event.action == KEY_ACTION_ESCAPE) {
        memset(state->pending_cmd, 0, sizeof(state->pending_cmd));
        app_state_set_mode(state, STATE_NAVIGATION);
        return true;
    }

    return false;
}

void app_sync_filesystem(AppState* state, FSDirectoryContent* content) {
    if (!state || !content) return;

    if (state->needs_sync) {
        if (fs_read_directory(state->target_path, content) == 0) {
            snprintf(state->current_path, sizeof(state->current_path), "%s", state->target_path);
            state->selectedFileIndex = 0;
        } else {
            snprintf(state->target_path, sizeof(state->target_path), "%s", state->current_path);
            fs_read_directory(state->current_path, content);
        }
        state->needs_sync = false;
    } else if (state->needs_reload) {
        fs_read_directory(state->current_path, content);
    }
    state->needs_reload = false;
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

        snprintf(state->target_path, sizeof(state->target_path), "%s", new_path);

        state->selectedFileIndex = 0;
        state->needs_sync = true;
    }
}

void navigate_back_dir(AppState* state) {
    if (!state) return;

    snprintf(state->target_path, sizeof(state->target_path), "%s", state->current_path);

    char* last_backslash = strrchr(state->target_path, '\\');

    if (last_backslash) {
        size_t index = last_backslash - state->target_path;

        if (index == 2 && *(last_backslash + 1) == '\0') {
            return;
        }

        if (index == 2) {
            *(last_backslash + 1) = '\0';
            state->needs_sync = true;
        } else if (index > 2) {
            *last_backslash = '\0';
            state->needs_sync = true;
        }
    }
}
