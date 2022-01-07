#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

/* DateTime */
#include <ESPDateTime.h>

/* Time Stamp */
#include <NTPClient.h>
#include <WiFiUdp.h>

#define NTP_OFFSET  -3  * 60 * 60 // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "0.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

// Timezone for DateTime library
const char* TZ = "<+03>-3"; // GMT-3

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Module id
const int mod_id = 0;

#include "config_pins.hpp"

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
const int nSensors= int(sensors.getDeviceCount());
DeviceAddress sensors_addr[2];

// Wifi credentials
const char* ssid = "Bunker";
const char* password = "dosestudos";

//Your Domain name with URL path or IP address with path
String serverName = "https://fastapi-tcc.herokuapp.com/";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long lastTimePost = 0;
unsigned long lastTimeLight = 0;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 10000;
unsigned long timerDelayPost = 60000;
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

void setupDateTime() {
  // setup this after wifi connected
  // you can use custom timeZone,server and timeout
  // DateTime.setTimeZone("CST-8");
  // DateTime.setServer("asia.pool.ntp.org");
  // DateTime.begin(15 * 1000);
  // from
  /** changed from 0.2.x **/
  DateTime.setTimeZone(TZ);
  // this method config ntp and wait for time sync
  // default timeout is 10 seconds
  DateTime.begin(/* timeout param */);
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
}

void setupOLDE() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 10);
  // Display static text
  display.println("Hello, world!");
  display.display(); 
}

void setupOneWire() {
  Serial.println(nSensors);
  byte i;
  byte addr[8];
  if (!oneWire.search(addr)) {
    Serial.println(" No more addresses.");
    Serial.println();
    oneWire.reset_search();
    delay(250);
    return;
  }
  Serial.print(" ROM =");
  for (i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
  // Start the DS18B20 sensor
  sensors.begin();
}

void displayTemp(float temp) {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(2);
  display.println(DateTime.toString());
  display.println(String(temp) + " C");
  display.display(); 
}

float getTemp() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  // Serial.print(temperatureC);
  // Serial.print("ºC  -  ");
  return temperatureC;
}

void insert_temp(float temp) {

  HTTPClient http;

  String serverPath = serverName + "Insert_TEMP/?temp=" + String(temp) + "&mod_id=" + String(mod_id) + "&timestamp=" + DateTime.toString(); // Endpoint + data for input in db
  String sub1 = " ";
  String sub2 = "%20";
  serverPath.replace(sub1, sub2);
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
  }
}

void setup() {
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

  // wifi settings
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  scanNetworks();
  connectToNetwork();
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());

  // RTC
  timeClient.begin();
  setupDateTime();

  setupOLDE();

}

void loop() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  }
  else {
    Serial.println("WiFi Disconnected");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
    connectToNetwork();
  }

  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  // Serial.println(formattedTime);
  String timestamp = DateTime.toString();
  // Serial.println(timestamp);
  
  // read ldr sensor status
  int ldr_status = digitalRead(ldr_sensor);

  // Get temperature from DS18B sensor status and display celsius 
  float temp = getTemp();
  displayTemp(temp);

  unsigned long millis_var = millis();

  // 
  if ((millis_var - lastTimeLight) > timerDelayLight) {
    // read ldr
    sound_alert(ldr_status);
    lastTimeLight = millis();
  }
  
  //Send an HTTP POST request every 10 minutes
  if ((millis_var - lastTimePost) > timerDelayPost) {
    insert_temp(temp);
    lastTimePost = millis();
  }
}
