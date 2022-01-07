/* File foo.  */
#ifndef _CONFIG_PINS_H_
#define _CONFIG_PINS_H_

// GPIO where the DS18B20 is connected to (23)
const int oneWireBus = 14;
// GPIO where the LDR is connected to
const int ldr_sensor = 26;
// GPIO where the Rele is connected to
const int Rele = 5;
// GPIO where the Buzzer is connected to
const int Buzzer = 4;
// GPIO where the LED is connected to
//int LED_BUILTIN = 2;

#endif /* !_CONFIG_PINS_H_ */