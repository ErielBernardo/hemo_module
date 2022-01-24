#ifndef _CONFIG_DISPLAY_HPP_
#define _CONFIG_DISPLAY_HPP_

#include "config_pins.hpp"
#include "config_mod_defines.hpp"
#include "DS18B_Sensor.hpp"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* DateTime */
#include <ESPDateTime.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

void setupOLED();
void displayTemp();
void displayTempDEBUG();

#endif