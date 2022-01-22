#ifndef _CONFIG_DS18B_SENSOR_HPP_
#define _CONFIG_DS18B_SENSOR_HPP_

#include "config_pins.hpp"

#include <OneWire.h>
#include <DallasTemperature.h>

extern DallasTemperature sensorsAT;
extern DallasTemperature sensors;

void setupOneWire();
float getTemp(DallasTemperature sensors);

#endif