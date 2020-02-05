#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define BTN_PIN PIN_A2

class Buttons
{
public:
    enum Button : int8_t{
        btn_noBtn    = -1,
        btn_resetT   = 0,
        btn_left     = 1,
        btn_minus    = 3,
        btn_power    = 4,
        btn_plus     = 5,
        btn_right    = 7,
        btn_ignition = 9,
        btn_pump     = 11
    };

public:
    Buttons();
    void keyBoardListener(void);

public:
    Button button();

private:

private:
    int get_key(int value);
};

#endif // BUTTONS_H
