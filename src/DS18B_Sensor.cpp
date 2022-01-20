#include "DS18B_Sensor.hpp"

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
const int nSensors = int(sensors.getDeviceCount());
DeviceAddress sensors_addr[2];

void setupOneWire()
{
    Serial.println(nSensors);
    byte i;
    byte addr[8];
    if (!oneWire.search(addr))
    {
        Serial.println(" No more addresses.");
        Serial.println();
        oneWire.reset_search();
        delay(250);
        return;
    }
    Serial.print(" ROM =");
    for (i = 0; i < 8; i++)
    {
        Serial.write(' ');
        Serial.print(addr[i], HEX);
    }
    // Start the DS18B20 sensor
    sensors.begin();
}

float getTemp()
{
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    // Serial.print(temperatureC);
    // Serial.print("ºC  -  ");
    return temperatureC;
}
