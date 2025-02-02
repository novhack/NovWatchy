#pragma once

#include <Arduino.h>
#include "config.h"
#include "menu_items.h"

#define MAX_MENU_ITEMS_ON_SCREEN (DISPLAY_HEIGHT / MENU_ITEM_HEIGHT)

void draw_menu(menu_item_t menu_items[], uint8_t menu_index = -1, uint8_t y_offset = 0, uint8_t items_to_draw = MAX_MENU_ITEMS_ON_SCREEN);
