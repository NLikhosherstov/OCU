#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#include "monitor.h"
#include "buttons.h"
#include "common.h"
#include "Owen/owen.h"
#include "Owen/programlaunch.h"

void checkOwenTemperature(void);
void setupTimerInterrupt(void);

#endif // MAIN_H
