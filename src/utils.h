#pragma once
#include <string.h>
#include <stdio.h>

/**
 * Copy structure from source to destination.
 */
void copy_struct(void *destination, void *source, size_t size) {
    memcpy(destination, source, size);
}

/**
 * If two struct are identical return true.
 */
bool compare_struct(void *oldState, void *newState, size_t size) {
    return memcmp(oldState, newState, size) == 0;
}

void seconds_to_mm_ss_string(uint32_t seconds, char* mmss_string) {
  uint32_t minutes = seconds / 60;
  sprintf(mmss_string, "%02d:%02d", minutes, seconds % 60);
}

void seconds_to_HH_mm_string(uint32_t seconds, char* mmss_string) {
  uint32_t minutes = seconds / 60;
  sprintf(mmss_string, "%02d:%02d", minutes / 60, minutes % 60);
}
