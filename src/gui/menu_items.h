#pragma once

#include "config.h"

#define MAX_MENU_ITEMS_ON_SCREEN (DISPLAY_HEIGHT / MENU_ITEM_HEIGHT)

/**
 * Struct for a menu item
 */
typedef struct {
    const char *title;
    void (*handler)();
    bool exit_after_handler;
    bool partial_refresh;
} menu_item_t;

/**
 * Array of menu items, defined in menu_items.cpp
 */
extern menu_item_t menu_items[];