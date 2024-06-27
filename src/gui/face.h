#pragma once

#include "fonts/Seven_Segment10pt7b.h"
#include "fonts/DSEG7_Classic_Regular_15.h"
#include "fonts/DSEG7_Classic_Bold_25.h"
#include "fonts/DSEG7_Classic_Regular_39.h"
#include "icons.h"

void showWatchFace(bool partialRefresh);
void drawWatchFace();
void drawTime();
void drawDate();
void drawSteps();
void drawWeather();
void drawBattery();
