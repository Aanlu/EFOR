#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "core/app_state.h"
#include "core/app_controller.h"
#include "core/interfaces/fs_reader.h"
#include "core/interfaces/input.h"


int main(void) {
    AppState state;
    FSDirectoryContent content;
    int result = -1;

    init_app_state(&state);
    fs_directory_content_init(&content);

    if (!platform_input_init()) {
        fprintf(stderr, "Failed to initialize input platform\n");
        return EXIT_FAILURE;
    }

    strcpy(state.current_path, "C:\\");
    strcpy(state.target_path, "C:\\");

    if (fs_read_directory(state.current_path, &content) != 0) {
        printf("Failed to read directory: %s\n", state.current_path);
        goto cleanup;
    }

    result = 0;

    fs_print_directory_content(&state, &content, state.current_path, state.selected_file_index);

while (state.running) {
        bool needs_redraw = handle_input(&state, &content);

        if (needs_redraw) {
            app_sync_filesystem(&state, &content);
            fs_print_directory_content(&state, &content, state.current_path, state.selected_file_index);
        }
    }

    if (state.input_failed) {
    result = EXIT_FAILURE;
    }

    cleanup:
    platform_input_shutdown();
    return result;
}
