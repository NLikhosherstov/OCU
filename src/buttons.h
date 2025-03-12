#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define BTN_PIN PIN_A2

class Buttons
{
public:
    enum Button : int8_t{
        btn_noBtn     = -1,
        btn_power     = 0,
        btn_up     = 1,
        btn_ignition  = 2,
        btn_pump      = 3,
        btn_down      = 4
    };

public:
    Buttons();
    void keyBoardListener(void);

public:
    Button button();
    void updateBtnData();
    void setBtnConfigMode(bool newBtnConfigMode);

private:
    bool m_btnConfigMode;

private:
    int get_key(int value);
    int get_keyFromCfg(int value);
};

#endif // BUTTONS_H
