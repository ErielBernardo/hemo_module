#ifndef _WIFI_HPP_
#define _WIFI_HPP_

#include <WiFi.h>
#include "config_mod_defines.hpp"
#include "config_pins.hpp"

String translateEncryptionType(wifi_auth_mode_t encryptionType);
void scanNetworks();
void connectToNetwork();
void setupWifi();

#endif