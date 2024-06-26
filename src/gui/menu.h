#pragma once

#include <Arduino.h>

void show_menu(byte menuIndex, bool partialRefresh);
void draw_menu(const char* menu_items[], uint8_t menu_length, uint8_t menu_index);
