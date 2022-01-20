#include "config_pins.hpp"
#include "config_mod_defines.hpp"
#include "Module.hpp"
#include "Display.hpp"
#include "DateTime.hpp"
#include "Wifi_Module.hpp"
#include "DS18B_Sensor.hpp"

#include <Arduino_JSON.h>

/* Time Stamp 
#include <NTPClient.h>
#include <WiFiUdp.h>

#define NTP_OFFSET -3 * 60 * 60 // In seconds
#define NTP_INTERVAL 60 * 1000  // In miliseconds
#define NTP_ADDRESS "0.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
*/


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long lastTimePost = 0;
unsigned long lastTimeLight = 0;

void setup()
{
    // Start the Serial Monitor
    Serial.begin(115200);
    delay(20050);
    // define o pino relativo ao Buzzer de saida
    pinMode(Buzzer, OUTPUT);
    // define o pino relativo ao Rele de saida
    pinMode(Rele, OUTPUT);
    // define o pino relativo ao sensor como entrada digital
    pinMode(ldr_sensor, INPUT);
    // define o pino relativo ao LED interno
    pinMode(LED_BUILTIN, OUTPUT);

    setupOneWire();

    setupOLDE();

    // RTC
    // timeClient.begin();
    setupDateTime();

    setupWifi();
}

void loop()
{
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on
    }
    else
    {
        Serial.println("WiFi Disconnected");
        digitalWrite(LED_BUILTIN, LOW); // turn the LED off
        connectToNetwork();
    }

    /* // RTC 
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  Serial.println(formattedTime);*/

    String timestamp = DateTime.toString();
    // Serial.println(timestamp);

    // read ldr sensor status
    int ldr_status = digitalRead(ldr_sensor);

    // Get temperature from DS18B sensor status and display celsius
    float temp = getTemp();
    displayTemp(temp, MOD_ID);

    unsigned long millis_var = millis();

    //
    if ((millis_var - lastTimeLight) > timerDelayLight)
    {
        // read ldr
        sound_alert(ldr_status);
        lastTimeLight = millis();
    }

    //Send an HTTP POST request every 10 minutes
    if ((millis_var - lastTimePost) > timerDelayPost)
    {
        insert_temp(temp);
        lastTimePost = millis();
    }
}
