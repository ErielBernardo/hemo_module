#include "DS18B_Sensor.hpp"

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWireAT(oneWireBusAT);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensorsAT(&oneWireAT);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
// Number of temperature devices found
const int nSensors = int(sensors.getDeviceCount());
DeviceAddress sensors_addr[2];

void setupOneWire()
{
    // Start the DS18B20 sensor
    sensors.begin();
    sensorsAT.begin();

    Serial.print("nSensors = ");
    Serial.println(nSensors);
    byte i;
    byte addr[8];
    // Loop through each device, print out address
    for (int j = 0; j < nSensors; j++)
    {
        if (!oneWire.search(addr))
        {
            Serial.println(" No more addresses.");
            Serial.println();
            oneWire.reset_search();
            return;
        }
        Serial.print(" ROM =");
        for (i = 0; i < 8; i++)
        {
            Serial.write(' ');
            Serial.print(addr[i], HEX);
        }
    }
}

float getTemp(DallasTemperature sensors)
{
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);

    return temperatureC;
}

float getTempOLD()
{
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);

    return temperatureC;
}