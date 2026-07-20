#include <stdio.h>
#include <string.h>
#include "core/app_state.h"
#include "platform/fs_reader.h"


int main() {
    AppState state;
    FSDirectoryContent content;
    init_app_state(&state);
    fs_directory_content_init(&content);

    strcpy(state.current_path, "C:\\");

    printf("Current Path: %s\n", state.current_path);
    printf("Language Code: %s\n", state.language_code);
    printf("Theme Color: #%06X\n", state.theme_color);
    printf("Running: %d\n", state.running);

    if (fs_read_directory(state.current_path, &content) == 0) {
        fs_print_directory_content(&content, state.current_path);
    } else {
        printf("Failed to read directory: %s\n", state.current_path);
    }

    return 0;
}

