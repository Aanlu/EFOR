#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "platform/fs_reader.h"

void fs_directory_content_init(FSDirectoryContent* content) {
    if (!content) return;

    content->count = 0;
}
void fs_print_directory_content(const FSDirectoryContent* content, const char* current_path){
    if (!content) return;
    printf("Directory\n");
    printf ("%s\n", current_path);
    printf("Directory Content:\n");
    for (int i = 0; i < content->count; i++) {
        printf("Name: %s, Is Directory: %d, Size: %llu bytes\n",
            content->items[i].name,
            content->items[i].isDirectory,
            content->items[i].size);
    }
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
    do{
        if (strcmp(fFileData.cFileName, ".") != 0 && strcmp(fFileData.cFileName, "..") != 0 && content->count < MAX_FILES_PER_DIR) {
            strncpy(content->items[content->count].name, fFileData.cFileName, MAX_FILENAME_LEN - 1);
            content->items[content->count].name[MAX_FILENAME_LEN - 1] = '\0'; // Ensure null-termination
            content->items[content->count].isDirectory = (fFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
            content->items[content->count].size = ((unsigned long long)fFileData.nFileSizeHigh << 32) | fFileData.nFileSizeLow;
            content->count++;
        }
    }while (FindNextFile(hFind, &fFileData) && content->count < MAX_FILES_PER_DIR);

    FindClose(hFind);
    return 0;
}
