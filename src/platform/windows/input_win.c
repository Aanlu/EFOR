#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#include "platform/input.h"

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

KeyAction input_get_action(void) {
    DWORD eventsWaiting = 0;
    GetNumberOfConsoleInputEvents(hInput, &eventsWaiting);

    if (eventsWaiting == 0) {
        return KEY_ACTION_NONE;
    }

    INPUT_RECORD inputRecord;
    DWORD eventsRead = 0;

    ReadConsoleInput(hInput, &inputRecord, 1, &eventsRead);

    if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {

        WORD vkCode = inputRecord.Event.KeyEvent.wVirtualKeyCode;
        DWORD controlState = inputRecord.Event.KeyEvent.dwControlKeyState;
        char asciiChar = inputRecord.Event.KeyEvent.uChar.AsciiChar;

        bool isControl = ((controlState & LEFT_CTRL_PRESSED) || (controlState & RIGHT_CTRL_PRESSED));

        if (isControl) {
            switch (vkCode) {
                case 'C': return KEY_ACTION_COPY;
            }
            return KEY_ACTION_NONE;
        }

        switch (vkCode) {
            case VK_UP:      return KEY_ACTION_UP;
            case VK_DOWN:    return KEY_ACTION_DOWN;
            case VK_LEFT:    return KEY_ACTION_LEFT;
            case VK_RIGHT:   return KEY_ACTION_RIGHT;
            case VK_RETURN:  return KEY_ACTION_ENTER;
            case VK_BACK:    return KEY_ACTION_LEFT;
            case VK_ESCAPE:  return KEY_ACTION_ESCAPE;
        }

        switch (asciiChar) {
            case 'w': case 'W': return KEY_ACTION_UP;
            case 's': case 'S': return KEY_ACTION_DOWN;
            case 'a': case 'A': return KEY_ACTION_LEFT;
            case 'd': case 'D': return KEY_ACTION_RIGHT;
            case 'q': case 'Q': return KEY_ACTION_ESCAPE;
        }
    }
    
    return KEY_ACTION_NONE;
}

void platform_sleep_ms(unsigned int milliseconds) {
    Sleep(milliseconds);
}

void platform_clear_screen(void) {
    printf("\033[2J\033[3J\033[H");
}