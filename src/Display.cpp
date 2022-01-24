#include "Display.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupOLDE()
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
    display.println("Hello, world!");
    display.display();
}

void displayTemp(float storage_temp, float ambient_temp)
{
    DateTimeParts p = DateTime.getParts();
    char Time[50];
    sprintf(Time, "%02d:%02d:%02d", p.getHours(), p.getMinutes(), p.getSeconds());
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println(Time);
    display.print(String(storage_temp) + "C");
    display.setTextSize(1);
    display.println(" Camara\n");
    display.setTextSize(2);
    display.print(String(ambient_temp) + "C");
    display.setTextSize(1);
    display.println(" Ambiente\n");
    display.println("MOD " + String(MOD_ID));
    display.display();
}
