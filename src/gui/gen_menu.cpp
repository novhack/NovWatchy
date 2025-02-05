#include "gen_menu.h"
#include "gui.h"
#include "hardware/hardware.h"
#include "hardware/rtc_sram.h"
#include <Fonts/FreeMonoBold9pt7b.h>

void draw_menu(menu_item_t menu_items[], uint8_t menu_index, uint8_t y_offset, uint8_t items_to_draw) {
  display.setFont(&FreeMonoBold9pt7b);

  int16_t x1, y1;
  uint16_t w, h;
  int16_t yPos;

  static uint8_t top_window = 0;
  static uint8_t bottom_window = items_to_draw;

  if (menu_index < top_window) {
    top_window = menu_index;
    bottom_window = top_window + items_to_draw;
  } else if (menu_index >= bottom_window) {
    bottom_window = menu_index + 1;
    top_window = bottom_window - items_to_draw;
  }

  for (int i = 0; i < items_to_draw; i++) {
    if (i + top_window >= MENU_LENGTH) {
      break;
    }

    uint8_t m_index = i + top_window;

    yPos = MENU_ITEM_HEIGHT * i + y_offset;
    display.setCursor(5, yPos);
    display.getTextBounds(menu_items[m_index].title, 0, yPos, &x1, &y1, &w, &h);

    if (m_index == menu_index) {
      display.fillRect(0, yPos, DISPLAY_WIDTH, MENU_ITEM_HEIGHT, GxEPD_WHITE);
      display.setTextColor(GxEPD_BLACK);
    } else {
      display.setTextColor(GxEPD_WHITE);
    }

    display.setCursor(5, yPos + MENU_ITEM_HEIGHT - (h / 2));
    display.println(menu_items[m_index].title);
  }
}
