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
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(0, 0);
    // Display static text
    display.println("Hello, world!");
    display.display();
}

void displayTemp(float temp, int mod_id)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println(DateTime.toString());
    display.println(String(temp) + "C");
    display.println("MOD " + String(mod_id));
    display.display();
}
