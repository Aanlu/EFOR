#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "core/path_utils.h"

void test_navigate_path_back_normal(void) {
    char path[256] = "C:\\Users\\Documentos";
    bool success = navigate_path_back(path);
    
    assert(success == true);
    assert(strcmp(path, "C:\\Users") == 0);
}

void test_navigate_path_back_to_root(void) {
    char path[256] = "C:\\Users";
    bool success = navigate_path_back(path);
    
    assert(success == true);
    assert(strcmp(path, "C:\\") == 0);
}

void test_navigate_path_back_already_root(void) {
    char path[256] = "C:\\";
    bool success = navigate_path_back(path);
    
    assert(success == false);
    assert(strcmp(path, "C:\\") == 0); 
}

int main(void) {
    printf("--- EJECUTANDO PRUEBAS UNITARIAS DE RUTAS ---\n");
    test_navigate_path_back_normal();
    test_navigate_path_back_to_root();
    test_navigate_path_back_already_root();
    printf("--- TODAS LAS PRUEBAS PASARON CON EXITO ---\n");
    return 0;
}