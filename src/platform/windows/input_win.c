#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#include "core/interfaces/input.h"

static HANDLE input_handle;
static DWORD original_console_mode;

bool platform_input_init(void) {
    input_handle = GetStdHandle(STD_INPUT_HANDLE);
    if (!GetConsoleMode(input_handle, &original_console_mode)) return false;

    DWORD raw_mode = original_console_mode & ~(ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    if (!SetConsoleMode(input_handle, raw_mode)) return false;

    return true;
}

void platform_input_shutdown(void) {
    SetConsoleMode(input_handle, original_console_mode);
}

InputEvent input_get_action(void) {
    INPUT_RECORD input_record;
    DWORD events_read = 0;

    while (true) {
        if (!ReadConsoleInput(input_handle, &input_record, 1, &events_read)){
            DWORD error_code = GetLastError();
            fprintf(stderr, "ReadConsoleInput failed:  %lu\n", (unsigned long)error_code);
            return (InputEvent){ .action = KEY_ACTION_ERROR, .character = '\0' };
        }

        if (events_read == 0) {
            fprintf(stderr, "ReadConsoleInput returned no events.\n");
            return (InputEvent){ .action = KEY_ACTION_ERROR, .character = '\0' };
        }

        if (input_record.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            return (InputEvent){ .action = KEY_ACTION_RESIZE, .character = '\0' };
        }

        if (input_record.EventType == KEY_EVENT && input_record.Event.KeyEvent.bKeyDown) {
            WORD virtual_key_code = input_record.Event.KeyEvent.wVirtualKeyCode;
            DWORD control_state = input_record.Event.KeyEvent.dwControlKeyState;
            char ascii_character = input_record.Event.KeyEvent.uChar.AsciiChar;

            bool is_control = (control_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;
            bool is_altgr = (control_state & (LEFT_CTRL_PRESSED | RIGHT_ALT_PRESSED)) == (LEFT_CTRL_PRESSED | RIGHT_ALT_PRESSED);

            if (is_control && !is_altgr) {
                switch (virtual_key_code) {
                    case 'C': return (InputEvent){ .action = KEY_ACTION_COPY, .character = '\0' };
                    default:  return (InputEvent){ .action = KEY_ACTION_NONE, .character = '\0' };
                }
            }

            switch (virtual_key_code) {
                case VK_UP:     return (InputEvent){ .action = KEY_ACTION_UP,        .character = '\0' };
                case VK_DOWN:   return (InputEvent){ .action = KEY_ACTION_DOWN,      .character = '\0' };
                case VK_LEFT:   return (InputEvent){ .action = KEY_ACTION_LEFT,      .character = '\0' };
                case VK_RIGHT:  return (InputEvent){ .action = KEY_ACTION_RIGHT,     .character = '\0' };
                case VK_RETURN: return (InputEvent){ .action = KEY_ACTION_ENTER,     .character = '\0' };
                case VK_ESCAPE: return (InputEvent){ .action = KEY_ACTION_ESCAPE,    .character = '\0' };
                case VK_BACK:   return (InputEvent){ .action = KEY_ACTION_BACKSPACE, .character = '\0' };
            }

            if (ascii_character >= 32 && ascii_character <= 126) {
                return (InputEvent){ .action = KEY_ACTION_TEXT, .character = ascii_character };
            }
        }
    }
}


void platform_clear_screen(void) {
    printf("\033[2J\033[3J\033[H");
}
