#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "core/constants.h"


typedef enum {
    STATE_NAVIGATION,
    STATE_COMMAND_INPUT,
    STATE_CONFIRMATION,
    STATE_PROCESSING,
    STATE_EXITING
} AppStateMode;

typedef struct AppState{
    char current_path[MAX_PATH_LENGTH];
    char target_path[MAX_PATH_LENGTH];
    char language_code[8];
    unsigned int theme_color;
    int running;
    int selectedFileIndex;
    bool needs_reload;
    bool needs_sync;
    
    AppStateMode current_mode;
    char command_buffer[MAX_BUFFER_LENGTH];
    size_t buffer_len;

    char pending_cmd[MAX_BUFFER_LENGTH];
    char status_message[MAX_BUFFER_LENGTH];
    bool status_is_error;
} AppState;

void init_app_state(AppState* state);
void app_state_set_mode(AppState* state, AppStateMode new_mode);
