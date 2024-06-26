#include "menu.h"
#include "state.h"
#include "hardware/hardware.h"
#include <Fonts/FreeMonoBold9pt7b.h>

#define MENU_ITEM_HEIGHT  25

const char *menuItems[] = {
  "Timer",
  "About",
  "Vibrate Motor",
  "Show Accelerometer",
  "Set Time",
  "Setup WiFi",
  "Update Firmware",
  "Sync NTP"
};

void show_menu(byte menuIndex, bool partialRefresh) {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  draw_menu(menuItems, 7, menuIndex);
  display.display(partialRefresh);
  set_gui_state(MAIN_MENU_STATE);
  alreadyInMenu = false;
}

void draw_menu(const char* menu_items[], uint8_t menu_length, uint8_t menu_index) {
  display.setFont(&FreeMonoBold9pt7b);

  int16_t x1, y1;
  uint16_t w, h;
  int16_t yPos;

  for (int i = 0; i < menu_length; i++) {
    yPos = MENU_ITEM_HEIGHT + (MENU_ITEM_HEIGHT * i);
    display.setCursor(0, yPos);
    if (i == menu_index) {
      display.getTextBounds(menu_items[i], 0, yPos, &x1, &y1, &w, &h);
      display.fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
      display.println(menu_items[i]);
    } else {
      display.setTextColor(GxEPD_WHITE);
      display.println(menu_items[i]);
    }
  }
}
