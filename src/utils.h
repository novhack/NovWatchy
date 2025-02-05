#pragma once
#include <string.h>

void copy_struct(void *destination, void *source, size_t size) {
    memcpy(destination, source, size);
}

bool compare_struct(void *oldState, void *newState, size_t size) {
    return memcmp(oldState, newState, size) != 0;
}
