#include "about.h"
#include "hardware/hardware.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include "hardware/rtc_sram.h"

void about_app_main() {
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(0, 20);

  display.print("Firm: ");
  display.println(FIRMWARE_NAME);

  display.print("Version: ");
  display.println(FIRMWARE_VERSION);

  display.print("Board: v");
  display.println(get_board_revision());

  display.print("Battery: ");
  float voltage = get_battery_voltage();
  display.print(voltage);
  display.println("V");

  display.print("Uptime: ");
  rtc.read(current_time);
  time_t b = makeTime(bootTime);
  time_t c = makeTime(current_time);
  int totalSeconds = c-b;
  int minutes = (totalSeconds % 3600) / 60;
  int hours = (totalSeconds % 86400) / 3600;
  int days = (totalSeconds % (86400 * 30)) / 86400;
  display.print(days);
  display.print("d");
  display.print(hours);
  display.print("h");
  display.print(minutes);
  display.println("m");

  display.display(true); // full refresh
}
