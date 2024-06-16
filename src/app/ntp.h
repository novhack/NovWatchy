#pragma once

#include <Arduino.h>

void showSyncNTP();
bool syncNTP();
bool syncNTP(long gmt);
bool syncNTP(long gmt, String ntpServer);
