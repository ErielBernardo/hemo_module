/* File foo.  */
#ifndef _CONFIG_PINS_H_
#define _CONFIG_PINS_H_

// GPIO where the DS18B20 is connected to (23)
const int oneWireBus = 23;
// GPIO where the LDR is connected to
const int ldr_sensor = 18;
// GPIO where the Rele is connected to
const int Rele = 4;
// GPIO where the Buzzer is connected to
const int Buzzer = 5;
// GPIO where the LED is connected to
//int LED_BUILTIN = 2;

#endif /* !_CONFIG_PINS_H_ */