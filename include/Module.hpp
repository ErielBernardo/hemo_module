#ifndef _MODULE_HPP_
#define _MODULE_HPP_

#include "config_pins.hpp"
#include "config_mod_defines.hpp"
#include "DateTime.hpp"
#include "DS18B_Sensor.hpp"
#include "Wifi_Module.hpp"
#include "Display.hpp"

#include <HTTPClient.h>

void insert_temp(float storage_temp, int ldr);
void insert_temp_test(int ldr);

void get_mod_temps(int mod_id);

void sound_alert(int ldr_status);
void sound_alert_test(int ldr_status);

void HandlerTemperature(void *pvParameters);
void HandlerWiFi(void *pvParameters);
void HandlerPost(void *pvParameters);
void HandlerDisplay(void *pvParameters);
void HandlerSoundAlert(void *pvParameters);

#endif