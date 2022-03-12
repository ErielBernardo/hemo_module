#include "Module.hpp"

String serverName = SERVERNAME;

unsigned long lastTimeLight = 0;
unsigned long lastTimeAlertOn = 0;
unsigned long lastTimeAlertOff = 0;

unsigned short int buzzer_state = LOW;
unsigned short int rele_state = LOW;

unsigned short int delayRead = int(taskPeriodPostAPI / rateReadPeriod);

struct ModuleDataPost
{
    float AmbientTemperature = -127;
    float StorageTemperature = -127;
    short int LDRStatus = -1;
    short int ReleState = -1;
    short int ModuleID = MOD_ID;
    String Timestamp = "";
};

ModuleDataPost ModuleDataPostBuffer[rateReadPeriod];

String postStringSerialize = "";

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
    // http.addHeader("accept", "application/json");
    // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // http.addHeader("Accept-Encoding", "gzip, deflate, br");
    // http.addHeader("Content-Type", "application/json");

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

void insert_temp_test(int ldr)
{
    HTTPClient http;

    // Endpoint + data for input in db
    String serverPath = serverName + "Insert_TEMP_TEST/?ambient_temp=" + String(AmbientTemp) + "&storage_temp=" + String(StorageTemp);
    serverPath = serverPath + "&mod_id=" + String(MOD_ID) + "&ldr=" + String(ldr) + "&timestamp=" + DateTime.toString();
    serverPath.replace(" ", "%20");
    Serial.print("\nposting = ");
    Serial.println(serverPath);

    // Your Domain name with URL path or IP address with path
    http.begin(serverPath);

    // Specify content-type header
    http.addHeader("Accept", "*/*");
    // http.addHeader("accept", "application/json");
    // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // http.addHeader("Accept-Encoding", "gzip, deflate, br");
    // http.addHeader("Content-Type", "application/json");

    // Data to send with HTTP POST
    //  String httpRequestData = "temp=" + String(temp) + "&mod_id=" + String(mod_id);
    //  Serial.print("httpRequestData = ");
    //  Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST("");
    if (httpResponseCode > 0)
    {
        // String payload = http.getString();
        // Serial.println(payload);
        Serial.print("HTTP POST RESP: ");
    }
    else
    {
        Serial.print("Error code: ");
    }
    Serial.println(httpResponseCode);

    // Free resources
    http.end();
}
// TODO Desenvolver
void insert_temp_multi()
{
    HTTPClient http;

    // Endpoint + data for input in db
    String serverPath = serverName + "Insert_MULTI_TEMP/?teste=" + String(TESTE);
    Serial.println("\nposting :");
    Serial.println(serverPath);

    // Your Domain name with URL path or IP address with path
    http.begin(serverPath);

    // Specify content-type header
    http.addHeader("accept", "application/json");
    http.addHeader("Content-Type", "application/json");

    // Data to send with HTTP POST
    serializeModuleDataPostBuffer();
    Serial.println("httpRequestData:");

    // Send HTTP POST request
    int httpResponseCode = http.POST(postStringSerialize);
    if (httpResponseCode > 0)
    {
        // String payload = http.getString();
        // Serial.println(payload);
        Serial.print("HTTP POST RESP: ");
    }
    else
    {
        Serial.print("Error code: ");
    }
    Serial.println(httpResponseCode);

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
        buzzer_state = LOW; // Turn buzzer off
    }
    else
    {
        if ((buzzer_state == LOW) && ((millis_var - lastTimeAlertOff) > longPeriodAlert))
        {
            buzzer_state = HIGH; // Turn buzzer on
            lastTimeAlertOn = millis_var;
            Serial.println(String(StorageTemp) + "°C " + "Buzzer ON at  " + String(millis_var) + " Tempo desligado " + String(millis_var - lastTimeAlertOff));
        }
        else if ((buzzer_state == HIGH) && ((millis_var - lastTimeAlertOn) > shortPeriodAlert))
        {
            buzzer_state = LOW; // Turn buzzer off
            lastTimeAlertOff = millis_var;
            Serial.println(String(StorageTemp) + "°C " + "Buzzer OFF at " + String(millis_var) + " Tempo ligado    " + String(millis_var - lastTimeAlertOn));
        }
    }
    digitalWrite(Buzzer, buzzer_state);
}

void sound_alert_test(int ldr_status)
{
    unsigned long millis_var = millis();
    if (ldr_status == HIGH && (StorageTemp > BULLET_TEMP))
    {
        buzzer_state = LOW; // Turn buzzer off
    }
    else
    {
        if ((buzzer_state == LOW) && ((millis_var - lastTimeAlertOff) > longPeriodAlert))
        {
            buzzer_state = HIGH; // Turn buzzer on
            lastTimeAlertOn = millis_var;
            Serial.println(String(StorageTemp) + "°C " + "Buzzer ON at  " + String(millis_var) + " Tempo desligado " + String(millis_var - lastTimeAlertOff));
        }
        else if ((buzzer_state == HIGH) && ((millis_var - lastTimeAlertOn) > shortPeriodAlert))
        {
            buzzer_state = LOW; // Turn buzzer off
            lastTimeAlertOff = millis_var;
            Serial.println(String(StorageTemp) + "°C " + "Buzzer OFF at " + String(millis_var) + " Tempo ligado    " + String(millis_var - lastTimeAlertOn));
        }
    }
    digitalWrite(Buzzer, buzzer_state);
}

void HandlerWiFi(void *pvParameters)
{
    for (;;)
    { // Loop infinito
        // Check WiFi connection status
        checkNetwork();
        vTaskDelay(taskPeriodWifi / portTICK_PERIOD_MS);
    }
}

void HandlerTemperature(void *pvParameters)
{
    for (;;)
    {                  // Loop infinito
        updateTemps(); // Get temperature from DS18B sensors status
        if (StorageTemp < BULLET_TEMP)
        {
            rele_state = LOW; // Turn off rele
        }
        else if (StorageTemp > BULLET_TEMP + 0.5)
        {
            rele_state = HIGH; // Turn on rele
        }
        digitalWrite(Rele, rele_state); // Define state rele
        vTaskDelay(taskPeriodTemperature / portTICK_PERIOD_MS);
    }
}

void HandlerDisplay(void *pvParameters)
{
    for (;;)
    { // Loop infinito
        displayTemp();
        // displayTempDEBUG();
        vTaskDelay(taskPeriodDisplay / portTICK_PERIOD_MS);
    }
}

void HandlerPost(void *pvParameters)
{
    for (;;)
    { // Loop infinito
        // Send an HTTP POST request every timerDelayPost minutes
        // insert_temp(StorageTemp, digitalRead(LDR_Sensor));
        insert_temp_test(digitalRead(LDR_Sensor)); // Post de testes
        vTaskDelay(taskPeriodPostAPI / portTICK_PERIOD_MS);
    }
}

void HandlerPostMulti(void *pvParameters)
{
    for (;;)
    { // Loop infinito
        for (int i = 0; i < rateReadPeriod; i++)
        {
            String Timestamp = DateTime.toString();
            // Timestamp.replace(" ", "%20");
            ModuleDataPostBuffer[i].AmbientTemperature = AmbientTemp;
            ModuleDataPostBuffer[i].StorageTemperature = StorageTemp;
            ModuleDataPostBuffer[i].Timestamp = Timestamp;
            ModuleDataPostBuffer[i].LDRStatus = digitalRead(LDR_Sensor);
            ModuleDataPostBuffer[i].ReleState = rele_state;
            vTaskDelay(delayRead / portTICK_PERIOD_MS);
        }
        unsigned long millisvar = millis();
        insert_temp_multi(); // Post de testes
        Serial.println("Posting time " + String(millis() - millisvar) + " ms");
    }
}

void HandlerSoundAlert(void *pvParameters)
{
    for (;;)
    { // Loop infinito
        sound_alert(digitalRead(LDR_Sensor));
        // sound_alert_test(digitalRead(LDR_Sensor));
        vTaskDelay(taskPeriodSoundAlert / portTICK_PERIOD_MS);
    }
}

void serializeModuleDataPostBuffer()
{
    unsigned long millisvar = millis();
    postStringSerialize = "[";
    for (int i = 0; i < rateReadPeriod; i++)
    {
        postStringSerialize = postStringSerialize + "{\"AmbientTemperature\": " + String(ModuleDataPostBuffer[i].AmbientTemperature) + ",";
        postStringSerialize = postStringSerialize + "\"StorageTemperature\": " + String(ModuleDataPostBuffer[i].StorageTemperature) + ",";
        postStringSerialize = postStringSerialize + "\"LDRStatus\": " + String(ModuleDataPostBuffer[i].LDRStatus) + ",";
        postStringSerialize = postStringSerialize + "\"ReleState\": " + String(ModuleDataPostBuffer[i].ReleState) + ",";
        postStringSerialize = postStringSerialize + "\"ModuleID\": " + String(ModuleDataPostBuffer[i].ModuleID) + ",";
        postStringSerialize = postStringSerialize + "\"Timestamp\": \"" + ModuleDataPostBuffer[i].Timestamp + "\"}";
        if (i < (rateReadPeriod - 1))
        {
            postStringSerialize = postStringSerialize + ",";
        }
        else
        {
            postStringSerialize = postStringSerialize + "]";
        }
    }
    Serial.println("Serializing time " + String(millis() - millisvar) + " ms");
}