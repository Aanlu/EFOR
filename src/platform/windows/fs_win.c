#include "fs/fs.h"
#include <windows.h>
#include <stdbool.h>

bool fs_create_directory(const char* path) {
    if (!path) return false;
    return CreateDirectoryA(path, NULL) != 0;
}

bool fs_create_file(const char* path) {
    if (!path) return false;
    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return false;
    CloseHandle(hFile);
    return true;
}

bool fs_delete(const char* path) {
    if (!path) return false;
    
    if (DeleteFileA(path) != 0) return true;
    
    return RemoveDirectoryA(path) != 0;
}

bool fs_rename(const char* old_path, const char* new_path) {
    if (!old_path || !new_path) return false;
    return MoveFileA(old_path, new_path) != 0;
}

bool fs_is_directory(const char* path) {
    if (!path) return false;
    DWORD attrs = GetFileAttributesA(path);
    return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_DIRECTORY);
}