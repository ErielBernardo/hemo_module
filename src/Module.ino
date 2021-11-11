#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

/* Time Stamp */
#include <NTPClient.h>
#include <WiFiUdp.h>

#define NTP_OFFSET  -3  * 60 * 60 // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "0.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

// Module id
const int mod_id = 0;

// GPIO where the DS18B20 is connected to (23)
const int oneWireBus = 23;
// GPIO where the LDR is connected to
const int ldr_sensor = 22;
// GPIO where the Rele is connected to
const int Rele =  21;
// GPIO where the Buzzer is connected to
const int Buzzer =  19;
// GPIO where the LED is connected to
//int LED_BUILTIN = 2;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Wifi credentials
const char* ssid = "Bunker";
const char* password = "dosestudos";

//Your Domain name with URL path or IP address with path
String serverName = "https://fastapi-tcc.herokuapp.com/";
//const char*  serverName = "https://fastapi-tcc.herokuapp.com/";


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 5000;

// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTimeLight = 0;
// Set timer to 30 seconds (30000)
unsigned long timerDelayLight  = 5000;

String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
}

float getTemp() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.print("ºC  -  ");
  Serial.print(temperatureF);
  Serial.println("ºF");
  // delay(1000);
  return temperatureC;
}

void scanNetworks() {

  int numberOfNetworks = WiFi.scanNetworks();

  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);

  for (int i = 0; i < numberOfNetworks; i++) {

    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");

  }
}

void connectToNetwork() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }

  Serial.println("Connected to network");
}

void insert_temp(float temp) {

  HTTPClient http;

  String serverPath = serverName + "Insert_TEMP/?temp=" + String(temp) + "&mod_id=" + String(mod_id); // Endpoint + data for input in db
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

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void get_mod_temps(int mod_id) {
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

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void sound_alert(int ldr_status){
  Serial.println(ldr_status);
  if (ldr_status == HIGH) {
    Serial.println("Nao há luz. Porta fechada");
    digitalWrite(Rele, LOW);
  }
  else {
    Serial.println("Porta aberta");
    digitalWrite(Rele, HIGH);  // only test
    digitalWrite (Buzzer, HIGH); //turn buzzer on
    delay(500);
    digitalWrite (Buzzer, LOW);  //turn buzzer off
    delay(1000);
  }
}

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  // Start the DS18B20 sensor
  sensors.begin();

  // define o pino relativo ao Buzzer de saida
  pinMode(Buzzer, OUTPUT);
  // define o pino relativo ao Rele de saida
  pinMode(Rele, OUTPUT);
  // define o pino relativo ao sensor como entrada digital
  pinMode(ldr_sensor, INPUT);
  // define o pino relativo ao LED interno
  pinMode(LED_BUILTIN, OUTPUT);

  // wifi settings
  scanNetworks();
  connectToNetwork();
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());

  // RTC
  timeClient.begin();

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  
  timeClient.update();

  // 
  if ((millis() - lastTimeLight) > timerDelayLight) {
    // read ldr
    int ldr_status = digitalRead(ldr_sensor);

    lastTimeLight = millis();
  }
  
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    String formattedTime = timeClient.getFormattedTime();
    Serial.print(formattedTime);

    // Get temperature from sensor
    float temp = getTemp();
    //Serial.println(temp);

    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WIFI CONNECTED");
      //get_mod_temps(0);
      //insert_temp(temp);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
