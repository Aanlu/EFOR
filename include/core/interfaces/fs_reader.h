#pragma once

#include <stddef.h>

#include "core/constants.h"

typedef struct AppState AppState;

typedef struct FileInfo {
    char name[MAX_FILENAME_LENGTH];
    int isDirectory;
    unsigned long long size;
} FileInfo;

typedef struct FSDirectoryContent {
    FileInfo items[MAX_FILES_PER_DIR];
    size_t count;
}FSDirectoryContent;

void fs_directory_content_init(FSDirectoryContent* content);
int fs_read_directory(const char* path, FSDirectoryContent* content);
void fs_print_directory_content(AppState* state, const FSDirectoryContent* content, const char* current_path, int selected_index);
