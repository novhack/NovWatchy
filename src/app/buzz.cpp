#include "buzz.h"
#include "hardware/hardware.h"
#include "hardware/motor.h"
#include "gui/menu.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include "gui/gui.h"

void showBuzz() {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(70, 80);
  display.println("Buzz!");
  display.display(false); // full refresh
  motor_vibrate();
  showMenu(menuIndex, false);
}
