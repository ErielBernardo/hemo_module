#ifndef _MODULE_HPP_
#define _MODULE_HPP_

#include "config_mod_defines.hpp"
#include "config_pins.hpp"
#include "DateTime.hpp"

#include <HTTPClient.h>

void insert_temp(float temp);
void get_mod_temps(int mod_id);
void sound_alert(int ldr_status);

#endif