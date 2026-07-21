#include <stdio.h>
#include <string.h>

#include "core/app_state.h"
#include "platform/fs_reader.h"
#include "platform/input.h"

void handle_input(AppState* state) {
    if (!state) return;

    KeyAction action = input_get_action();
    switch (action) {
        case KEY_ACTION_UP:
            printf("Up key pressed\n");
            break;
        case KEY_ACTION_DOWN:
            printf("Down key pressed\n");
            break;
        case KEY_ACTION_LEFT:
            printf("Left key pressed\n");
            break;
        case KEY_ACTION_RIGHT:
            printf("Right key pressed\n");
            break;
        case KEY_ACTION_ENTER:
            printf("Enter key pressed\n");
            break;
        case KEY_ACTION_ESCAPE:
            printf("Escape key pressed, exiting...\n");
            state->running = 0; // Set running to 0 to exit the loop
            break;
        default:
            // No action or unrecognized key
            break;
    }
}

int main() {
    AppState state;
    FSDirectoryContent content;
    init_app_state(&state);
    fs_directory_content_init(&content);

    strcpy(state.current_path, "C:\\");
    do{
        handle_input(&state);
        /*if (fs_read_directory(state.current_path, &content) == 0) {
            fs_print_directory_content(&content, state.current_path);
        } else {
            printf("Failed to read directory: %s\n", state.current_path);
        }*/
    platform_sleep_ms(10);


    }while (state.running);
    

    return 0;
}

