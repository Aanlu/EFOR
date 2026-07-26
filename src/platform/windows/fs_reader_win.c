#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "platform/fs_reader.h"
#include "platform/input.h"

void fs_directory_content_init(FSDirectoryContent* content) {
    if (!content) return;

    content->count = 0;
}
void fs_print_directory_content(AppState* state, const FSDirectoryContent* content, const char* current_path, int selected_index) {
    if (!content || !current_path) return;

    platform_clear_screen();

    printf("==================================================\n");
    printf(" RUTA ACTUAL: %s\n", current_path);
    printf("==================================================\n\n");

    if (content->count == 0) {
        printf("  (Directorio vacio)\n");
        return;
    }

    for (size_t i = 0; i < content->count; i++) {
    if (selected_index >= 0 && i == (size_t)selected_index) {
        printf(" -> ");
    } else {
        printf("    ");
    }

    if (content->items[i].isDirectory) {
        printf("[%s]\n", content->items[i].name);
    } else {
        printf(" %s\n", content->items[i].name);
    }
}

    printf("\n--------------------------------------------------\n");
    printf(" Usa W/S o Flechas para moverte | ESC para salir\n");

    printf("==================================================\n");

    if (state->current_mode == STATE_COMMAND_INPUT) {
        printf("%s", state->command_buffer);
    } else {
        printf("Presiona ':' para ingresar un comando.");
    }
    
    fflush(stdout);
}


int fs_read_directory(const char* path, FSDirectoryContent* content) {
    if (!path || !content) return -1;

    WIN32_FIND_DATA fFileData;

    char search_path[MAX_PATH_LEN];
    snprintf(search_path, sizeof(search_path), "%s\\*", path);

    HANDLE hFind = FindFirstFile(search_path, &fFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return -1;
    }
    content -> count = 0;
    do{
        if (strcmp(fFileData.cFileName, ".") == 0 || strcmp(fFileData.cFileName, "..") == 0) {
            continue;
        }

        if ((fFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) || (fFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) || (fFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
            continue;
        }

        if (content->count < MAX_FILES_PER_DIR) {
            strncpy(content->items[content->count].name, fFileData.cFileName, MAX_FILENAME_LEN - 1);
            content->items[content->count].name[MAX_FILENAME_LEN - 1] = '\0';
            content->items[content->count].isDirectory = (fFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
            content->items[content->count].size = ((unsigned long long)fFileData.nFileSizeHigh << 32) | fFileData.nFileSizeLow;
            content->count++;
        }

    }while (FindNextFile(hFind, &fFileData) && content->count < MAX_FILES_PER_DIR);

    FindClose(hFind);
    return 0;
}

