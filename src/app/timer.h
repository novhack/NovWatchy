#pragma once

#include <GxEPD2_BW.h>

/**
 * Main app function
*/
void timer_app_main();

/**
 * Displays timer countdown
*/
void timer_start(uint32_t seconds);

/**
 * Converts seconds to mm:ss string
 */
void seconds_to_string(uint32_t seconds, char* mmss_string);
