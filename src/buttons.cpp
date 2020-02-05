#include "buttons.h"

//  2  5  8  11
//  1  4  7  10
//  0  3  6  3

static int key_values[12] = {1023  //0  RESET T
                            ,930   //1  -LEFT
                            ,867   //2
                            ,806   //3  -MINUS
                            ,760   //4  -POWER
                            ,720   //5  -PLUS
                            ,685   //6
                            ,657   //7  -RIGHT
                            ,632   //8
                            ,608   //9  -IGNITION
                            ,589   //10
                            ,572}; //11 -PUMP

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
