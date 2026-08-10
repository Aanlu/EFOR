#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "core/app_state.h"
#include "core/app_controller.h"
#include "platform/fs_reader.h"
#include "platform/input.h"


int main(void) {
    AppState state;
    FSDirectoryContent content;

    init_app_state(&state);
    fs_directory_content_init(&content);
    platform_input_init();

    strcpy(state.current_path, "C:\\");
    strcpy(state.target_path, "C:\\");

    if (fs_read_directory(state.current_path, &content) != 0) {
        printf("Failed to read directory: %s\n", state.current_path);
        return -1;
    }

    fs_print_directory_content(&state, &content, state.current_path, state.selectedFileIndex);

    while (state.running) {
        bool needs_redraw = handle_input(&state, &content);
        
        if (needs_redraw) {
            app_sync_filesystem(&state, &content);
            fs_print_directory_content(&state, &content, state.current_path, state.selectedFileIndex);
        }

        platform_sleep_ms(5);
    }

    platform_input_shutdown();
    return 0;
}