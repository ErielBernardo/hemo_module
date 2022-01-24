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

TaskHandle_t TaskHandlerTemperature;
TaskHandle_t TaskHandlerWiFi;
TaskHandle_t TaskHandlerPost;

void setup()
{
    // Start the Serial Monitor
    Serial.begin(115200);
    // define o pino relativo ao Buzzer de saida
    pinMode(Buzzer, OUTPUT);
    // define o pino relativo ao Rele de saida
    pinMode(Rele, OUTPUT);
    // define o pino relativo ao sensor como entrada digital
    pinMode(LDR_Sensor, INPUT);
    // define o pino relativo ao LED interno
    pinMode(LED_BUILTIN, OUTPUT);

    setupOLDE();
    setupOneWire();
    setupWifi();
    setupDateTime();

    // xTaskCreate(
    //     HandlerWiFi,        // Função a ser chamada
    //     "Handler WiFi",     // Nome da tarefa
    //     1000,               // Tamanho (bytes)
    //     NULL,               // Parametro a ser passado
    //     3,                  // Prioridade da Tarefa
    //     &TaskHandlerWiFi    // Task handle
    // );

    // xTaskCreate(
    //     HandlerTemperature,     // Função a ser chamada
    //     "Temperature Handler ", // Nome da tarefa
    //     1000,                   // Tamanho (bytes)
    //     NULL,                   // Parametro a ser passado
    //     2,                      // Prioridade da Tarefa
    //     &TaskHandlerTemperature // Task handle
    // );

    // xTaskCreate(
    //     HandlerPost,        // Função a ser chamada
    //     "Handler Post API", // Nome da tarefa
    //     10000,              // Tamanho (bytes)
    //     NULL,               // Parametro a ser passado
    //     4,                  // Prioridade da Tarefa
    //     &TaskHandlerPost    // Task handle
    // );

}

void loop()
{
    unsigned short int ldr_status = digitalRead(LDR_Sensor); // Read LDR sensor status
    // displayTemp(StorageTemp, AmbientTemp);
    sound_alert(ldr_status);
}
