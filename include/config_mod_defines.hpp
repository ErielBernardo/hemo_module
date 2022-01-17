
#ifndef _CONFIG_MOD_DEFINES_HPP_
#define _CONFIG_MOD_DEFINES_HPP_


// Module id
const int mod_id = 0;


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long lastTimePost = 0;
unsigned long lastTimeLight = 0;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 10000;
unsigned long timerDelayPost = 60000;
unsigned long timerDelayLight = 5000;

#endif