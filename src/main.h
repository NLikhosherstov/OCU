#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>

#include "DHT.h"

#include "monitor.h"
#include "buttons.h"
#include "common.h"
#include "Owen/owen.h"
#include "Owen/programlaunch.h"
#include "Owen/programlaunch2.h"
#include "Owen/programstop.h"


void checkOwenTemperature(void);
void checkSpaceTemperature(void);
void setupTimerInterrupt(void);

void onBtnPwr();
void onBtnPlus();
void onBtnMinus();
void onBtnResetT();
void onBtnLeft();
void onBtnRight();
void onBtnPumpPlus();
void onBtnPumpMinus();
void onBtnIgnition();
void onBtnPump();

#endif // MAIN_H
