#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "core/app_controller.h"

int main(void) {
    AppState state;
    FSDirectoryContent content;

    init_app_state(&state);
    fs_directory_content_init(&content);

    strcpy(state.current_path, "C:\\");

    if (fs_read_directory(state.current_path, &content) != 0) {
        printf("Failed to read directory: %s\n", state.current_path);
        return -1;
    }

    fs_print_directory_content(&content, state.current_path, state.selectedFileIndex);

    while (state.running) {
        bool needs_redraw = handle_input(&state, &content);
        
        if (needs_redraw) {
            if (state.needs_reload) {
                fs_read_directory(state.current_path, &content);
                state.needs_reload = false;
            }
            fs_print_directory_content(&content, state.current_path, state.selectedFileIndex);
        }

        platform_sleep_ms(20);
    }

    return 0;
}