#include "Display.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupOLED()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    delay(2000);
    display.clearDisplay();
    display.setRotation(2);
    display.setTextSize(2);
    display.setTextColor(WHITE);

    display.setCursor(10, 0);
    display.println("Hellow, world!");
    display.display();
}

void displayTemp()
{
    DateTimeParts p = DateTime.getParts();
    char Time[50];
    sprintf(Time, "%02d:%02d:%02d", p.getHours(), p.getMinutes(), p.getSeconds());

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print(" ");
    display.println(Time);
    display.print(String(StorageTemp) + "C");
    display.setTextSize(1);
    display.println("  Storage\n");
    display.setTextSize(2);
    display.print(String(AmbientTemp) + "C");
    display.setTextSize(1);
    display.println("  Ambient\n");
    display.print("MOD " + String(MOD_ID));
    switch (rele_state)
    {
    case (LOW):
        display.println("         R OFF");
        break;
    case (HIGH):
        display.println("         R  ON");
        break;
    }
    switch (digitalRead(LDR_Sensor))
    {
    case (LOW):
        display.println("     PORTA ABERTA");
        break;
    case (HIGH):
        display.println("     PORTA FECHADA");
        break;
    default:
        display.println("    PORTA INDEFINIDA");
        break;
    }
    display.display();

    // Serial.println("StorageTemp " + String(StorageTemp) + "°C - AmbientSensor " + String(AmbientTemp) + "°C");
}

void displayTempDEBUG()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println(String(millis()));
    display.println(" Ambient\n");
    display.println("MOD " + String(MOD_ID));
    display.display();
}
