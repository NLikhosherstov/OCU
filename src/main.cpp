#include "main.h"

Monitor monitor;
Buttons btns;

Owen           owen;
ProgramLaunch2 programLaunch;
ProgramStop    programStop;

void setup() {
    Serial.begin(115200);
    monitor.start();

    checkOwenTemperature();
    setupTimerInterrupt();
}

void loop() {
    switch(btns.button()){
        case Buttons::btn_noBtn:
            break;
        case Buttons::btn_resetT:
            owen.resetTemp();
            break;
        case Buttons::btn_left:
            break;
        case Buttons::btn_minus:
            Serial.println(F("MINUS_BTN"));
            owen.downEngineSpeed(14);
            break;
        case Buttons::btn_power:
            Serial.println(F("PWR_BTN"));
            if (owen.active() == false && programLaunch.state() == ProgramBase::StandBy){
                programStop.stop();
                programLaunch.execute();
            }else{
                programLaunch.stop();
                programStop.execute();
            }
            break;
        case Buttons::btn_plus:
            Serial.println(F("PLUS_BTN"));
            owen.upEngineSpeed(14);
            break;
        case Buttons::btn_right:
            break;
        case Buttons::btn_ignition:
            programLaunch.stop();
            programStop.stop();
            Serial.println(F("IGNT_BTN"));
            owen.ignition() ? owen.stopIgnition() : owen.startIgnition();
            break;
        case Buttons::btn_pump:
            programLaunch.stop();
            programStop.stop();
            Serial.println(F("PUMP_BTN"));
            owen.pump() ? owen.stopPump() : owen.startPump();
            break;
    }

    static unsigned long millis_prev200ms;
    if(millis()-200 > millis_prev200ms){
        owen.readEngineTemp();
        monitor.showOwenData(owen);
        millis_prev200ms = millis();
    }

    static unsigned long millis_prev1000ms;
    if(millis()-2000 > millis_prev1000ms){
        millis_prev1000ms = millis();
    }
}

void setupTimerInterrupt(void){
    cli();
    {
        TCCR1A  = 0;             // установить регистры в 0
        TCCR1B  = 0;

        TIMSK1 |= (1 << OCIE1A); // включение прерываний по совпадению
        TCCR1B |= (1 << WGM12 ); // включение в CTC режим

        OCR1A   = 1562;          // установка регистра совпадения
        TCCR1B |= (1 << CS10  ); // Установка битов CS10 на коэффициент деления 1024
        TCCR1B |= (1 << CS12  ); // Установка битов CS12 на коэффициент деления 1024
    }
    sei();
}

ISR(TIMER1_COMPA_vect){
    btns.keyBoardListener();
    owen.changeEngineSpeed();
    owen.checkIgnitionSafety();
    owen.filtrateTemp();
    programLaunch.update(owen);
    programStop.update(owen);
}

void checkOwenTemperature(){
    double value = owen.readEngineTemp();
    while(utils::isNum(value) == false){
        String str = "Owen t sensor: " + String(value);
        monitor.showError(str);

        delay(300);
        value = owen.readEngineTemp();
    }
    Serial.print(F("Owen temperature: "));
    Serial.println(value);
}
