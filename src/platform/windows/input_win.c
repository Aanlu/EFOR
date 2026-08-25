#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#include "core/interfaces/input.h"

static HANDLE hInput;
static DWORD originalConsoleMode;

void platform_input_init(void) {
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hInput, &originalConsoleMode);
    
    DWORD rawMode = originalConsoleMode & ~(ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hInput, rawMode);
}

void platform_input_shutdown(void) {
    SetConsoleMode(hInput, originalConsoleMode);
}

InputEvent input_get_action(void) {
    INPUT_RECORD inputRecord;
    DWORD eventsRead = 0;

    while (true) {
        ReadConsoleInput(hInput, &inputRecord, 1, &eventsRead);

        if (inputRecord.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            return (InputEvent){ .action = KEY_ACTION_RESIZE, .character = '\0' };
        }

        if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
            WORD vkCode = inputRecord.Event.KeyEvent.wVirtualKeyCode;
            DWORD controlState = inputRecord.Event.KeyEvent.dwControlKeyState;
            char asciiChar = inputRecord.Event.KeyEvent.uChar.AsciiChar;

            bool isControl = (controlState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;
            bool isAltGr = (controlState & (LEFT_CTRL_PRESSED | RIGHT_ALT_PRESSED)) == (LEFT_CTRL_PRESSED | RIGHT_ALT_PRESSED);

            if (isControl && !isAltGr) {
                switch (vkCode) {
                    case 'C': return (InputEvent){ .action = KEY_ACTION_COPY, .character = '\0' };
                    default:  return (InputEvent){ .action = KEY_ACTION_NONE, .character = '\0' };
                }
            }

            switch (vkCode) {
                case VK_UP:     return (InputEvent){ .action = KEY_ACTION_UP,        .character = '\0' };
                case VK_DOWN:   return (InputEvent){ .action = KEY_ACTION_DOWN,      .character = '\0' };
                case VK_LEFT:   return (InputEvent){ .action = KEY_ACTION_LEFT,      .character = '\0' };
                case VK_RIGHT:  return (InputEvent){ .action = KEY_ACTION_RIGHT,     .character = '\0' };
                case VK_RETURN: return (InputEvent){ .action = KEY_ACTION_ENTER,     .character = '\0' };
                case VK_ESCAPE: return (InputEvent){ .action = KEY_ACTION_ESCAPE,    .character = '\0' };
                case VK_BACK:   return (InputEvent){ .action = KEY_ACTION_BACKSPACE, .character = '\0' };
            }

            if (asciiChar >= 32 && asciiChar <= 126) {
                return (InputEvent){ .action = KEY_ACTION_TEXT, .character = asciiChar };
            }
        }
    }
}

void platform_sleep_ms(unsigned int milliseconds) {
    Sleep(milliseconds);
}

void platform_clear_screen(void) {
    printf("\033[2J\033[3J\033[H");
}
