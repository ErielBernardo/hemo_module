#ifndef _MODULE_HPP_
#define _MODULE_HPP_

#include "config_mod_defines.hpp"
#include "config_pins.hpp"
#include "DateTime.hpp"

#include <HTTPClient.h>

void insert_temp(float storage_temp, int ldr);
void insert_temp_test(float storage_temp, float ambient_temp, int ldr);

void get_mod_temps(int mod_id);

void sound_alert(int ldr_status);
void sound_alert_OLD(int ldr_status);

#endif