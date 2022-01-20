#ifndef _CONFIG_DS18B_SENSOR_HPP_
#define _CONFIG_DS18B_SENSOR_HPP_

#include "config_pins.hpp"

#include <OneWire.h>
#include <DallasTemperature.h>

void setupOneWire();
float getTemp();

#endif