#include "DS18B_Sensor.hpp"

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWireAmbient(oneWireBusAmbient);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature AmbientSensor(&oneWireAmbient);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWireStorage(oneWireBusStorage);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature StorageSensors(&oneWireStorage);
// Number of temperature devices found
const int nSensors = int(StorageSensors.getDeviceCount());
DeviceAddress sensors_addr[2];

float StorageTemp = 0;
float AmbientTemp = 0;

void setupOneWire()
{
    // Start the DS18B20 sensor
    StorageSensors.begin();
    AmbientSensor.begin();

    Serial.print("nSensors = ");
    Serial.println(nSensors);
    byte i;
    byte addr[8];
    // Loop through each device, print out address
    for (int j = 0; j < nSensors; j++)
    {
        if (!oneWireStorage.search(addr))
        {
            Serial.println(" No more addresses.");
            Serial.println();
            oneWireStorage.reset_search();
            return;
        }
        Serial.print(" ROM =");
        for (i = 0; i < 8; i++)
        {
            Serial.write(' ');
            Serial.print(addr[i], HEX);
        }
    }
    StorageTemp = getTemp(StorageSensors);
    AmbientTemp = getTemp(AmbientSensor);
}

float getTemp(DallasTemperature sensors)
{
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);

    return temperatureC;
}

void updateTemps()
{
    AmbientSensor.requestTemperatures();
    AmbientTemp = AmbientSensor.getTempCByIndex(0);
    StorageSensors.requestTemperatures();
    StorageTemp = StorageSensors.getTempCByIndex(0);
    // Serial.println("StorageTemp " + String(StorageSensors.getTempCByIndex(0)) + "°C - AmbientSensor " + String(AmbientSensor.getTempCByIndex(0)) + "°C\n");
}