#ifndef FS_READER_H
#define FS_READER_H

#include <stddef.h>
#include "core/app_state.h"

#define MAX_FILENAME_LEN 256
#define MAX_PATH_LEN 4096
#define MAX_FILES_PER_DIR 1024

typedef struct {
    char name[MAX_FILENAME_LEN];
    int isDirectory;
    unsigned long long size;
} FileInfo;

typedef struct {
    FileInfo items[MAX_FILES_PER_DIR];
    size_t count;
}FSDirectoryContent;

void fs_directory_content_init(FSDirectoryContent* content);
int fs_read_directory(const char* path, FSDirectoryContent* content);
void fs_print_directory_content(AppState* state, const FSDirectoryContent* content, const char* current_path, int selected_index);

#endif