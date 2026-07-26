#ifndef APP_STATE_H
#define APP_STATE_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_PATH_LENGTH 4096
#define LANG_CODE_LEN 3

typedef enum {
    STATE_NAVIGATION,
    STATE_COMMAND_INPUT,
    STATE_CONFIRMATION,
    STATE_PROCESSING,
    STATE_EXITING
} AppStateMode;

typedef struct {
    char current_path[MAX_PATH_LENGTH];
    char target_path[MAX_PATH_LENGTH];
    char language_code[LANG_CODE_LEN];
    unsigned int theme_color;
    int running;
    int selectedFileIndex;
    bool needs_reload;
    bool needs_sync;

    AppStateMode current_mode;
    char command_buffer[256];
    size_t buffer_len;
} AppState;

void init_app_state(AppState* state);
void app_state_set_mode(AppState* state, AppStateMode new_mode);

#endif