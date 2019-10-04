#include "main.h"

Monitor monitor;
Buttons btns;

Owen          owen;
ProgramLaunch programLaunch;
ProgramStop   programStop;

void setup() {
    Serial.begin(115200);
    monitor.start();

    checkOwenTemperature();
    setupTimerInterrupt();
}

void loop() {
    if(btns.isPwr()){
        Serial.println(F("PWR_BTN"));
        if (owen.active() == false && programLaunch.state() == ProgramBase::StandBy)
            programLaunch.execute();
        else
            programStop.execute();
    }else if(btns.isUp()){
        Serial.println(F("UP_BTN"));
        owen.upEngineSpeed(20);
    }else if(btns.isDwn()){
        Serial.println(F("DWN_BTN"));
        owen.downEngineSpeed(20);
    }

    static unsigned long millis_prev;
    if(millis()-200 > millis_prev)
    {
        owen.readEngineTemp();
        monitor.showTemperatures(owen.currTemp(), programLaunch.state());
        millis_prev = millis();
    }
}

void setupTimerInterrupt(void){
    cli();
    {
        TCCR1A  = 0;             // установить регистры в 0
        TCCR1B  = 0;

        TIMSK1 |= (1 << OCIE1A); // включение прерываний по совпадению
        TCCR1B |= (1 << WGM12 ); // включение в CTC режим

        OCR1A   = 1562;         // установка регистра совпадения
        TCCR1B |= (1 << CS10  ); // Установка битов CS10 на коэффициент деления 1024
        TCCR1B |= (1 << CS12  ); // Установка битов CS12 на коэффициент деления 1024
    }
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    owen.changeEngineSpeed();
    owen.filtrateTemp();
    programLaunch.update(owen);
    programStop.update(owen);
}

void checkOwenTemperature()
{
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
