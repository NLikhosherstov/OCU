#include "buttons.h"

//--------------
//     | 0 |
//--------------
//     | 1 |
//--------------
// | 2 |   | 3 |
//--------------
//     | 4 |

static int key_values[12] = {
                            758    //0  -POWER
                            ,530   //1
                            ,355   //2  -IGNITION
                            ,167   //3  -PUMP
                            ,17    //4  -RESET T
                            };

volatile int value = -1;
volatile int old_value = -1;
volatile int m_currentButton = -1;
volatile int oldKeyValue;
volatile int innerKeyValue;

Buttons::Buttons()
{
//	attachInterrupt(0, Buttons::keyBoardListener, FALLING);
    pinMode(BTN_PIN, INPUT_PULLUP);
}

void Buttons::keyBoardListener(void){
    int raw = analogRead(BTN_PIN);
    int actualKeyValue = get_key(raw);

    if (innerKeyValue != actualKeyValue) {  // Пришло значение отличное от предыдущего
        innerKeyValue = actualKeyValue;     // Запоминаем новое значение
    }

    if (actualKeyValue != oldKeyValue) {
        oldKeyValue = actualKeyValue;         // Запоминаем новое значение
        m_currentButton = actualKeyValue;
    }
}

Buttons::Button Buttons::button(){
    int btn = m_currentButton;
    m_currentButton = -1;
    innerKeyValue = -1;
    return static_cast<Button>(btn);
}

int Buttons::get_key(int value){
    int error = 10;
    for (int i(11); i >= 0; --i) {
      // Если значение в заданном диапазоне values[i]+/-error - считаем, что кнопка определена
      if ( (value <= (key_values[i] + error)) && (value >= (key_values[i] - error)))
          return i;
    }
    return -1;
}
