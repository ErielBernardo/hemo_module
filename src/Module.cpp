#include "Module.hpp"

String serverName = SERVERNAME;

unsigned long lastTimeLight = 0;
unsigned long lastTimeAlertOn = 0;
unsigned long lastTimeAlertOff = 0;

unsigned short int buzzer_state = LOW;

void insert_temp(float storage_temp, int ldr)
{

    HTTPClient http;

    String serverPath = serverName + "Insert_TEMP/?storage_temp=" + String(storage_temp) + "&ldr=" + String(ldr);
    serverPath = serverPath + "&mod_id=" + String(MOD_ID) + "&timestamp=" + DateTime.toString();
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

void insert_temp_test(float storage_temp, float ambient_temp, int ldr)
{

    HTTPClient http;

    // Endpoint + data for input in db
    String serverPath = serverName + "Insert_TEMP_TEST/?ambient_temp=" + String(ambient_temp) + "&storage_temp=" + String(storage_temp);
    serverPath = serverPath + "&mod_id=" + String(MOD_ID) + "&ldr=" + String(ldr) + "&timestamp=" + DateTime.toString();
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
    unsigned long millis_var = millis();
    if (ldr_status == HIGH)
    {
        // Serial.println("Nao há luz. Porta fechada");
        buzzer_state = LOW; //turn buzzer off
    }
    else
    {
        if ((buzzer_state == LOW) && ((millis_var - lastTimeAlertOff) > longPeriodAlert))
        {
            buzzer_state = HIGH; // Turn buzzer on
            lastTimeAlertOn = millis_var;
            Serial.println("Buzzer ON at  " + String(millis_var) + " Tempo desligado " + String(millis_var - lastTimeAlertOff));
        }
        else if ((buzzer_state == HIGH) && ((millis_var - lastTimeAlertOn) > shortPeriodAlert))
        {
            buzzer_state = LOW; // Turn buzzer off
            lastTimeAlertOff = millis_var;
            Serial.println("Buzzer OFF at " + String(millis_var) + " Tempo ligado    " + String(millis_var - lastTimeAlertOn));
        }
    }
    digitalWrite(Buzzer, buzzer_state);
}

void sound_alert_test(int ldr_status, float storage_temp)
{
    unsigned long millis_var = millis();
    if (ldr_status == HIGH && (storage_temp > BULLET_TEMP))
    {
        buzzer_state = LOW; //turn buzzer off
    }
    else
    {
        if ((buzzer_state == LOW) && ((millis_var - lastTimeAlertOff) > longPeriodAlert))
        {
            buzzer_state = HIGH; // Turn buzzer on
            lastTimeAlertOn = millis_var;
            Serial.println("Buzzer ON at  " + String(millis_var) + " Tempo desligado " + String(millis_var - lastTimeAlertOff));
        }
        else if ((buzzer_state == HIGH) && ((millis_var - lastTimeAlertOn) > shortPeriodAlert))
        {
            buzzer_state = LOW; // Turn buzzer off
            lastTimeAlertOff = millis_var;
            Serial.println("Buzzer OFF at " + String(millis_var) + " Tempo ligado    " + String(millis_var - lastTimeAlertOn));
        }
    }
    digitalWrite(Buzzer, buzzer_state);
}

void sound_alert_OLD(int ldr_status)
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

void HandlerTemperature(void *pvParameters)
{
    for (;;)
    { // Loop infinito
        // Get temperature from DS18B sensor status and display celsius
        StorageTemp = getTemp(StorageSensors);
        AmbientTemp = getTemp(AmbientSensor);
    }
}

void HandlerWiFi(void *pvParameters)
{
    for (;;)
    { // Loop infinito
        //Check WiFi connection status
        checkNetwork();
    }
}

void HandlerPost(void *pvParameters)
{
    for (;;)
    { // Loop infinito
        // Send an HTTP POST request every timerDelayPost minutes
        insert_temp(StorageTemp, digitalRead(LDR_Sensor));
        // insert_temp_test(StorageTemp, AmbientTemp, ldr_status); // Post de testes
        delay(timerDelayPost);
    }
}