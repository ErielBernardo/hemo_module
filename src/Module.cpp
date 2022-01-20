#include "Module.hpp"

String serverName = SERVERNAME;

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
