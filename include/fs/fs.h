#ifndef FS_H
#define FS_H

#include <stdbool.h>

bool fs_create_directory(const char* path);
bool fs_create_file(const char* path);
bool fs_delete(const char* path);
bool fs_rename(const char* old_path, const char* new_path);
bool fs_is_directory(const char* path);

#endif