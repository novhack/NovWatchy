#include "face.h"

#include <floatToString.h>

#include "hardware/hardware.h"
#include "hardware/bma.h"
#include "gui/fonts/DSEG7_Classic_Bold_53.h"
#include "gui/fonts/Seven_Segment10pt7b.h"
#include "gui/fonts/DSEG7_Classic_Bold_25.h"
#include "gui/fonts/DSEG7_Classic_Regular_39.h"
#include "app/weather.h"
#include "gui/icons.h"

#define DARKMODE true

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

void drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
    display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if (BLE_CONFIGURED) {
        display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void drawTime(){
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53+5);
    int displayHour;
    if (HOUR_12_24 == 12) {
        displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
        displayHour = currentTime.Hour;
    }
    if (displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);
    display.print(":");
    if (currentTime.Minute < 10) {
        display.print("0");
    }
    display.println(currentTime.Minute);
}

void drawDate(){
    display.setFont(&Seven_Segment10pt7b);

    int16_t  x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
    if (currentTime.Wday == 4) {
        w = w - 5;
    }
    display.setCursor(85 - w, 85);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(85 - w, 110);
    display.println(month);

    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(5, 120);
    if(currentTime.Day < 10){
    display.print("0");
    }
    display.println(currentTime.Day);
}

void drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(35, 190);
    display.println(stepCount);
}

void drawBattery(){
    float voltage = get_battery_voltage();

    display.setFont(&Seven_Segment10pt7b);
    display.setCursor(150, 95);

    char voltage_string[5];
    floatToString(voltage, voltage_string, sizeof(voltage_string), 3);
    display.print(voltage_string);
}

void drawWeather(){

    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    display.setFont(&DSEG7_Classic_Regular_39);
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if (159 - w - x1 > 87) {
        display.setCursor(159 - w - x1, 150);
    } else {
        display.setFont(&DSEG7_Classic_Bold_25);
        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.setCursor(159 - w - x1, 136);
    }
    display.println(temperature);
    display.drawBitmap(165, 110, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if (WIFI_CONFIGURED) {
        if (weatherConditionCode > 801) {
            //Cloudy
            weatherIcon = cloudy;
        } else if(weatherConditionCode == 801) {
            //Few Clouds
            weatherIcon = cloudsun;
        } else if(weatherConditionCode == 800) {
            //Clear
            weatherIcon = sunny;
        } else if(weatherConditionCode >= 700) {
            //Atmosphere
            weatherIcon = atmosphere;
        } else if(weatherConditionCode >= 600) {
            //Snow
            weatherIcon = snow;
        } else if(weatherConditionCode >= 500) {
            //Rain
            weatherIcon = rain;
        } else if(weatherConditionCode >= 300) {
            //Drizzle
            weatherIcon = drizzle;
        } else if(weatherConditionCode >= 200) {
            //Thunderstorm
            weatherIcon = thunderstorm;
        } else
        return;
    } else {
        weatherIcon = cputemp;
    }
    display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}
