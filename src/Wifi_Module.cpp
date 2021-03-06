#include "Wifi_Module.hpp"

// Wifi credentials
const char *ssid = SSID_CONN;
const char *password = PASSWORD_CONN;

String translateEncryptionType(wifi_auth_mode_t encryptionType)
{
    switch (encryptionType)
    {
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
    default:
        return "";
    }
}

void scanNetworks()
{
    int numberOfNetworks = WiFi.scanNetworks();

    Serial.print("Number of networks found: ");
    Serial.println(numberOfNetworks);

    for (int i = 0; i < numberOfNetworks; i++)
    {
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

void connectToNetwork()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, LOW); // turn the LED off
        Serial.println("Establishing connection to WiFi..");
        delay(500);
    }
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on

    Serial.println("Connected to network");
}

void checkNetwork()
{
    Serial.println("Check WiFi connection status");
    //Check WiFi connection status
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi Disconnected");
        connectToNetwork();
    }
}

void setupWifi()
{
    // wifi settings
    digitalWrite(LED_BUILTIN, LOW); // turn the LED off
    scanNetworks();
    connectToNetwork();
    Serial.println(WiFi.macAddress());
    Serial.println(WiFi.localIP());
}