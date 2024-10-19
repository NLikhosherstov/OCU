#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>

#include "DHT.h"

#include "monitor.h"
#include "buttons.h"
#include "common.h"
#include "Owen/owen.h"
#include "Owen/programlaunch2.h"
#include "Owen/programstop.h"

void checkOwenTemperature(void);
void checkSpaceTemperature(void);
void setupTimerInterrupt(void);

void onBtnPwr();
void onEncoderPlus(bool fast = false);
void onEncoderMinus(bool fast = false);
void onBtnUp();
void onBtnDown();
void onBtnPumpPlus();
void onBtnPumpMinus();
void onBtnIgnition();
void onBtnPump();
void onEncoderClick();
void onEncoderLongClick();

#endif // MAIN_H
