#ifndef APP_STATE_H
#define APP_STATE_H

#include <stdbool.h>

#define MAX_PATH_LENGTH 4096
#define LANG_CODE_LEN 3

typedef struct {
    char current_path[MAX_PATH_LENGTH];
    char language_code[LANG_CODE_LEN];
    unsigned int theme_color;
    int running;
    int selectedIndex;
    int selectedFileIndex;
    bool modeCommand;
    bool needs_reload;
} AppState;

void init_app_state(AppState* state);
#endif