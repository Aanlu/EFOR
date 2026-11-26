#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "core/logger.h"

static bool check_file_contains(const char* filename, const char* expected_text) {
    FILE* file = fopen(filename, "r");
    if (!file) return false;

    char line[256];
    bool found = false;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, expected_text) != NULL) {
            found = true;
            break;
        }
    }

    fclose(file);
    return found;
}

int main(void) {
    printf("--- RUNNING LOGGER UNIT TESTS ---\n");

    remove("debug.log");

    LOG_INFO("Info test message");
    LOG_WARN("Warning test message with code: %d", 404);
    LOG_ERROR("Critical error test message");

    if (!check_file_contains("debug.log", "[INFO]") || !check_file_contains("debug.log", "Info test message")) {
        printf("[FAIL] INFO log entry not found in debug.log\n");
        return 1;
    }

    if (!check_file_contains("debug.log", "[WARN]") || !check_file_contains("debug.log", "404")) {
        printf("[FAIL] WARN log entry not found in debug.log\n");
        return 1;
    }

    if (!check_file_contains("debug.log", "[ERROR]") || !check_file_contains("debug.log", "Critical error test message")) {
        printf("[FAIL] ERROR log entry not found in debug.log\n");
        return 1;
    }

    printf("--- LOGGER TESTS PASSED SUCCESSFULLY ---\n");
    return 0;
}