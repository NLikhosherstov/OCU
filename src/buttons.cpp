#include "buttons.h"

#include "common.h"
//--------------
//     | 0 |
//--------------
//     | 1 |
//--------------
// | 2 |   | 3 |
//--------------
//     | 4 |

static int key_values[5] = {
                            758    //0  -POWER
                            ,530   //1  -UP
                            ,355   //2  -IGNITION
                            ,167   //3  -PUMP
                            ,17    //4  -DOWN
                            };

volatile int value = -1;
volatile int old_value = -1;
volatile int m_currentButton = -1;
volatile int oldKeyValue;
volatile int innerKeyValue;

Buttons::Buttons(){
    pinMode(BTN_PIN, INPUT_PULLUP);
}

void Buttons::keyBoardListener(void){
    int raw = analogRead(BTN_PIN);
    int actualKeyValue = get_key(raw);
    if(raw < 1020){
//        Serial.print(actualKeyValue); Serial.print(" "); Serial.println(raw);
    }

    if(raw < 1000 && m_btnConfigMode && btnSettingNumber() >= 0){
        if(get_keyFromCfg(raw) == -1 || btnSettingNumber() == get_keyFromCfg(raw)){
            if(btnSettingNumber() == 0){
                key_values[btnSettingNumber()] = raw;
                Cfg().btnPower = raw;
            }else if(btnSettingNumber() == 1){
                key_values[btnSettingNumber()] = raw;
                Cfg().btnUp = raw;
            }else if(btnSettingNumber() == 2){
                key_values[btnSettingNumber()] = raw;
                Cfg().btnIgnition = raw;
            }else if(btnSettingNumber() == 3){
                key_values[btnSettingNumber()] = raw;
                Cfg().btnPump = raw;
            }else if(btnSettingNumber() == 4){
                key_values[btnSettingNumber()] = raw;
                Cfg().btnDown = raw;
            }
        }
    }else{
        if (innerKeyValue != actualKeyValue) {  // Пришло значение отличное от предыдущего
            innerKeyValue = actualKeyValue;     // Запоминаем новое значение
        }

        if (actualKeyValue != oldKeyValue) {
            oldKeyValue = actualKeyValue;         // Запоминаем новое значение
            m_currentButton = actualKeyValue;
        }
    }
}

Buttons::Button Buttons::button(){
    int btn = m_currentButton;
    m_currentButton = -1;
    innerKeyValue = -1;
    return static_cast<Button>(btn);
}

void Buttons::updateBtnData(){
    key_values[0] = Cfg().btnPower;
    key_values[1] = Cfg().btnUp;
    key_values[2] = Cfg().btnIgnition;
    key_values[3] = Cfg().btnPump;
    key_values[4] = Cfg().btnDown;
}

void Buttons::setBtnConfigMode(bool newBtnConfigMode){
    m_btnConfigMode = newBtnConfigMode;
}

int Buttons::get_key(int value){
    int error = 8;
    for (int i(4); i >= 0; --i) {
      // Если значение в заданном диапазоне values[i]+/-error - считаем, что кнопка определена
      if ( (value <= (key_values[i] + error)) && (value >= (key_values[i] - error)))
          return i;
    }
    return -1;
}

int Buttons::get_keyFromCfg(int value){
    int error = 10;
    for (int i(4); i >= 0; --i) {
      if (i == 0 && (value <= (Cfg().btnPower + error)) && (value >= (Cfg().btnPower - error)))
          return i;
      if (i == 1 && (value <= (Cfg().btnUp + error)) && (value >= (Cfg().btnUp - error)))
          return i;
      if (i == 2 && (value <= (Cfg().btnIgnition + error)) && (value >= (Cfg().btnIgnition - error)))
          return i;
      if (i == 3 && (value <= (Cfg().btnPump + error)) && (value >= (Cfg().btnPump - error)))
          return i;
      if (i == 4 && (value <= (Cfg().btnDown + error)) && (value >= (Cfg().btnDown - error)))
          return i;
    }
    return -1;
}
