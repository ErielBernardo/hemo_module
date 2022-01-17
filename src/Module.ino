#include "config_pins.hpp"
#include "config_mod_defines.hpp"
#include "config_display.hpp"
#include "config_DateTime.hpp"
#include "config_Wifi.hpp"

#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

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

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
const int nSensors = int(sensors.getDeviceCount());
DeviceAddress sensors_addr[2];

//Your Domain name with URL path or IP address with path
String serverName = "https://fastapi-tcc.herokuapp.com/";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long lastTimePost = 0;
unsigned long lastTimeLight = 0;

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

void insert_temp(float temp)
{

  HTTPClient http;

  String serverPath = serverName + "Insert_TEMP/?temp=" + String(temp) + "&mod_id=" + String(MOD_ID) + "&timestamp=" + DateTime.toString(); // Endpoint + data for input in db
  serverPath.replace(" ", "%20");
  Serial.print("serverPath = ");
  Serial.println(serverPath);

  // Your Domain name with URL path or IP address with path
  http.begin(serverPath);

  // Specify content-type header
  http.addHeader("Accept", "*/*");
  //http.addHeader("accept", "application/json");
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //http.addHeader("Accept-Encoding", "gzip, deflate, br");
  //http.addHeader("Content-Type", "application/json");

  // Data to send with HTTP POST
  //  String httpRequestData = "temp=" + String(temp) + "&mod_id=" + String(mod_id);
  //  Serial.print("httpRequestData = ");
  //  Serial.println(httpRequestData);

  // Send HTTP POST request
  int httpResponseCode = http.POST("");

  Serial.print("HTTP POST RESP: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void get_mod_temps(int mod_id)
{
  HTTPClient http;

  String serverPath = serverName;
  serverPath = serverPath + "read_mod/" + String(mod_id);
  Serial.print("serverPath = ");
  Serial.println(serverPath);

  // Your Domain name with URL path or IP address with path
  http.begin(serverPath.c_str());
  Serial.println("HTTP BEGIN");

  // Send HTTP GET request
  int httpResponseCode = http.GET();
  Serial.print("HTTP GET RESP: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void sound_alert(int ldr_status)
{
  if (ldr_status == HIGH)
  {
    Serial.println("Nao há luz. Porta fechada");
    digitalWrite(Rele, LOW);
  }
  else
  {
    Serial.println("Porta aberta");
    digitalWrite(Rele, HIGH);   // only test
    digitalWrite(Buzzer, HIGH); //turn buzzer on
    delay(500);
    digitalWrite(Buzzer, LOW); //turn buzzer off
  }
}

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
