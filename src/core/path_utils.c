#include <string.h>
#include <ctype.h>
#include "core/path_utils.h"

#if defined(_WIN32) || defined(__CYGWIN__)
    #define PATH_SEPARATOR '\\'
    #define ALT_PATH_SEPARATOR '/'
#else
    #define PATH_SEPARATOR '/'
    #define ALT_PATH_SEPARATOR '\\'
#endif

static char* find_last_path_separator(const char* path) {
    if (!path) return NULL;

    char* last_sep = strrchr(path, PATH_SEPARATOR);
    char* last_alt = strrchr(path, ALT_PATH_SEPARATOR);

    if (!last_sep) return last_alt;
    if (!last_alt) return last_sep;

    return (last_sep > last_alt) ? last_sep : last_alt;
}

bool is_root_path(const char* path) {
    if (!path) return false;

    size_t len = strlen(path);

    if (len == 1 && (path[0] == '/' || path[0] == '\\')) {
        return true;
    }

    if (len == 3 && isalpha((unsigned char)path[0]) && path[1] == ':') {
        if (path[2] == '\\' || path[2] == '/') {
            return true;
        }
    }

    if (len == 2 && isalpha((unsigned char)path[0]) && path[1] == ':') {
        return true;
    }

    return false;
}

bool navigate_path_back(char* path) {
    if (!path) return false;

    if (is_root_path(path)) {
        return false;
    }

    char* last_sep = find_last_path_separator(path);
    if (!last_sep) {
        return false;
    }

    size_t index = (size_t)(last_sep - path);

    if (index == 2 && isalpha((unsigned char)path[0]) && path[1] == ':') {
        *(last_sep + 1) = '\0';
        return true;
    }

    if (index == 0) {
        *(last_sep + 1) = '\0';
        return true;
    }

    *last_sep = '\0';
    return true;
}