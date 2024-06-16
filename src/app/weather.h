#pragma once

#include <Arduino.h>
#include <TimeLib.h>

typedef struct weatherData {
  int8_t temperature;
  int16_t weatherConditionCode;
  bool isMetric;
  String weatherDescription;
  bool external;
  tmElements_t sunrise;
  tmElements_t sunset;
} weatherData;

weatherData getWeatherData();
weatherData _getWeatherData(String cityID, String lat, String lon, String units, String lang, String url, String apiKey, uint8_t updateInterval);
