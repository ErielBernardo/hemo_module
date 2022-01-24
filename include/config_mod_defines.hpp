#ifndef _CONFIG_MOD_DEFINES_HPP_
#define _CONFIG_MOD_DEFINES_HPP_

// Module id
#define MOD_ID 0

//Your Domain name with URL path or IP address with path
#define SERVERNAME "https://fastapi-tcc.herokuapp.com/"

// Wifi credentials
#define SSID_CONN "Bunker"
#define PASSWORD_CONN "dosestudos"

// Temperatura alvo
#define BULLET_TEMP 5 // em °C

// Set timer values
#define timerDelay 10000
#define timerDelayPost 10000
#define timerDelayLight 5000

#define shortPeriodAlert 50
#define longPeriodAlert 2000

#define taskPeriodWifi 120000
#define taskPeriodTemperature 1000
#define taskPeriodDisplay 500
#define taskPeriodSoundAlert 100
#define taskPeriodPostAPI 10000

// Timezone for DateTime library
#define TZ "<+03>-3" // GMT-3

#endif