#pragma once

#include <WiFiManager.h>

void setupWifi();
static void _configModeCallback(WiFiManager *myWiFiManager);
bool connectWiFi();
