#ifndef _CONFIG_MOD_DEFINES_HPP_
#define _CONFIG_MOD_DEFINES_HPP_

// Module id
#define MOD_ID 667
#define TESTE true

// Wifi credentials
#define SSID_CONN "Bunker"
#define PASSWORD_CONN "dosestudos"

//Your Domain name with URL path or IP address with path
#define SERVERNAME "https://fastapi-tcc.herokuapp.com/"

// Temperatura alvo
#define BULLET_TEMP 5 // em °C

// Set timer values
#define timerDelay 10000 // miliseconds
#define timerDelayPost 10000 // miliseconds
#define timerDelayLight 5000 // miliseconds

#define shortPeriodAlert 50 // miliseconds
#define longPeriodAlert 2000 // miliseconds

#define taskPeriodWifi 120000 // miliseconds
#define taskPeriodTemperature 1000 // miliseconds
#define taskPeriodDisplay 250 // miliseconds
#define taskPeriodSoundAlert 100 // miliseconds

#define taskPeriodPostAPI 300000 // miliseconds - 300000 ms = 5 min
#define rateReadMinute 20 // quantity
#define rateReadPeriod 100 // Rate per taskPeriodPostAPI

// Timezone for DateTime library
#define TZ "<+03>-3" // GMT-3

#endif