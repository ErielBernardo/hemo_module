#include "config_pins.hpp"
#include "config_mod_defines.hpp"
#include "Module.hpp"

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long lastTimePost = 0;

TaskHandle_t TaskHandlerTemperature;
TaskHandle_t TaskHandlerWiFi;
TaskHandle_t TaskHandlerPost;
TaskHandle_t TaskHandlerDisplay;
TaskHandle_t TaskHandlerSoundAlert;

void setup()
{
    // Start the Serial Monitor
    Serial.begin(115200);
    // define o pino relativo ao Buzzer de saida
    pinMode(Buzzer, OUTPUT);
    // define o pino relativo ao Rele de saida
    pinMode(Rele, OUTPUT);
    digitalWrite(Rele, LOW); // Turn off rele
    // define o pino relativo ao sensor como entrada digital
    pinMode(LDR_Sensor, INPUT);
    // define o pino relativo ao LED interno
    pinMode(LED_BUILTIN, OUTPUT);

    setupOLED();
    setupOneWire();
    setupWifi();
    setupDateTime();

    Serial.println("\nCreating WiFi Handler");
    xTaskCreatePinnedToCore(
        HandlerWiFi,      // Função a ser chamada
        "WiFi Handler",   // Nome da tarefa
        1000,             // Tamanho (bytes) 1000
        NULL,             // Parametro a ser passado
        0,                // Prioridade da Tarefa 0
        &TaskHandlerWiFi, // Task handle
        0                 // Core
    );
    Serial.println("WiFi Handler Created");

    delay(500);
    Serial.println("\nCreating Temperature Handler");
    xTaskCreatePinnedToCore(
        HandlerTemperature,      // Função a ser chamada
        "Temperature Handler",   // Nome da tarefa
        10000,                   // Tamanho (bytes) 10000
        NULL,                    // Parametro a ser passado
        10,                      // Prioridade da Tarefa 10
        &TaskHandlerTemperature, // Task handle
        1                        // Core
    );
    Serial.println("Temperature Handler Created");

    delay(500);
    Serial.println("\nCreating Display Handler");
    xTaskCreatePinnedToCore(
        HandlerDisplay,      // Função a ser chamada
        "Display Handler",   // Nome da tarefa
        50000,               // Tamanho (bytes)
        NULL,                // Parametro a ser passado
        20,                  // Prioridade da Tarefa
        &TaskHandlerDisplay, // Task handle
        1                    // Core
    );
    Serial.println("Display Handler Created");

    delay(500);
    Serial.println("\nCreating Sound Alert Handler");
    xTaskCreatePinnedToCore(
        HandlerSoundAlert,      // Função a ser chamada
        "Sound Alert Handler",  // Nome da tarefa
        1000,                   // Tamanho (bytes)
        NULL,                   // Parametro a ser passado
        1,                      // Prioridade da Tarefa
        &TaskHandlerSoundAlert, // Task handle
        0);
    Serial.println("Display Sound Alert Created");

    delay(5000);
    Serial.println("\nCreating Post API Handler");
    xTaskCreatePinnedToCore(
        HandlerPostMulti,   // Função a ser chamada
        "Post API Handler", // Nome da tarefa
        10000,              // Tamanho (bytes)
        NULL,               // Parametro a ser passado
        4,                  // Prioridade da Tarefa
        &TaskHandlerPost,   // Task handle
        1);
    Serial.println("Post API Handler Created");
}

void loop()
{
}
