#include "menu.h"
#include "gui.h"
#include "menu_items.h"
#include "hardware/hardware.h"
#include "hardware/rtc_sram.h"
#include <Fonts/FreeMonoBold9pt7b.h>

void show_menu(byte menuIndex, bool partialRefresh) {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  draw_menu(menuIndex);
  display.display(partialRefresh);
  gui_state = MAIN_MENU_STATE;
  alreadyInMenu = false;
}

void draw_menu(uint8_t menu_index) {
  display.setFont(&FreeMonoBold9pt7b);

  int16_t x1, y1;
  uint16_t w, h;
  int16_t yPos;

  static uint8_t top_window = 0;
  static uint8_t bottom_window = MAX_MENU_ITEMS_ON_SCREEN;

  if (menu_index < top_window) {
    top_window = menu_index;
    bottom_window = top_window + MAX_MENU_ITEMS_ON_SCREEN;
  } else if (menu_index >= bottom_window) {
    bottom_window = menu_index + 1;
    top_window = bottom_window - MAX_MENU_ITEMS_ON_SCREEN;
  }

  for (int i = 0; i < MAX_MENU_ITEMS_ON_SCREEN; i++) {
    if (i + top_window >= MENU_LENGTH) {
      break;
    }

    uint8_t m_index = i + top_window;
    
    yPos = MENU_ITEM_HEIGHT * i;
    display.setCursor(0, yPos);
    display.getTextBounds(menu_items[m_index].title, 0, yPos, &x1, &y1, &w, &h);

    if (m_index == menu_index) {
      display.fillRect(0, yPos, DISPLAY_WIDTH, MENU_ITEM_HEIGHT, GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
    } else {
      display.setTextColor(GxEPD_WHITE);
    }

    display.setCursor(0, yPos + MENU_ITEM_HEIGHT - (h / 2));
    display.println(menu_items[m_index].title);
  }
}
