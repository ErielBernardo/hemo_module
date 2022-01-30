#ifndef _MODULE_HPP_
#define _MODULE_HPP_

#include <HTTPClient.h>
// #include <ArduinoJson.h>

#include "config_pins.hpp"
#include "config_mod_defines.hpp"
#include "DateTime.hpp"
#include "DS18B_Sensor.hpp"
#include "Wifi_Module.hpp"
#include "Display.hpp"

extern unsigned short int rele_state;

void insert_temp(float storage_temp, int ldr);
void insert_temp_test(int ldr);
void insert_temp_multi();

void get_mod_temps(int mod_id);

void sound_alert(int ldr_status);
void sound_alert_test(int ldr_status);

void HandlerTemperature(void *pvParameters);
void HandlerWiFi(void *pvParameters);
void HandlerDisplay(void *pvParameters);
void HandlerSoundAlert(void *pvParameters);

struct ModuleDataPost;
void serializeModuleDataPostBuffer();
void HandlerPost(void *pvParameters);
void HandlerPostMulti(void *pvParameters);

#endif