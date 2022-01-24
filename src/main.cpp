#include "config_pins.hpp"
#include "config_mod_defines.hpp"
#include "Module.hpp"
#include "Display.hpp"
#include "DateTime.hpp"
#include "Wifi_Module.hpp"
#include "DS18B_Sensor.hpp"

#include <Arduino_JSON.h>

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long lastTimePost = 0;

void setup()
{
    // Start the Serial Monitor
    Serial.begin(115200);
    // define o pino relativo ao Buzzer de saida
    pinMode(Buzzer, OUTPUT);
    // define o pino relativo ao Rele de saida
    pinMode(Rele, OUTPUT);
    // define o pino relativo ao sensor como entrada digital
    pinMode(ldr_sensor, INPUT);
    // define o pino relativo ao LED interno
    pinMode(LED_BUILTIN, OUTPUT);

    setupOLDE();
    setupOneWire();
    setupWifi();
    setupDateTime();
}

void loop()
{
    //Check WiFi connection status
    checkNetwork();

    unsigned short int ldr_status = digitalRead(ldr_sensor); // Read LDR sensor status

    // Get temperature from DS18B sensor status and display celsius
    StorageTemp = getTemp(StorageSensors);
    AmbientTemp = getTemp(AmbientSensor);

    displayTemp(StorageTemp, AmbientTemp);
    sound_alert(ldr_status);

    unsigned long millis_var = millis();
    // Send an HTTP POST request every timerDelayPost minutes
    if ((millis_var - lastTimePost) > timerDelayPost)
    {
        insert_temp(StorageTemp, ldr_status);
        // insert_temp_test(StorageTemp, AmbientTemp, ldr_status); // Post de testes
        lastTimePost = millis();
    }
}
