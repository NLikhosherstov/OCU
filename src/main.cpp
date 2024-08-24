#include "main.h"

uint8_t targetT = 20;

//GyverOLED<SSD1306_128x64> monitor;
Monitor monitor;
Buttons btns;

EncButton eb(10, 11, 12);

Owen           owen;
ProgramLaunch2 programLaunch;
ProgramStop    programStop;

#define DHTPIN 4
DHT dht(DHTPIN, DHT22);

void setup() {
    Serial.begin(115200);
    eb.setEncReverse(1);

    if(EEPROM.read(0) == 255){
        EEPROM.write(0, targetT);
        Serial.println(F("Writing EEPROM Target temp 20"));
    }else{
        targetT = EEPROM.read(0);
    }
    owen.setTargetSpaceT(targetT);

    dht.begin();
    checkSpaceTemperature();

    monitor.start();

    checkOwenTemperature();
    setupTimerInterrupt();
}

void loop() {
    owen.pumpPulse();
    eb.tick();

    int pwmDelta = 3;
    if(eb.fast())
        pwmDelta = 10;

    if (eb.left())       onBtnMinus(pwmDelta);
    else if (eb.right()) onBtnPlus(pwmDelta);

    switch(btns.button()){
        case Buttons::btn_noBtn:                      break;
        case Buttons::btn_power:    onBtnPwr();       break;
        case Buttons::btn_space:                      break;
        case Buttons::btn_pump:     onBtnPump();      break;
        case Buttons::btn_ignition: onBtnIgnition();  break;
        case Buttons::btn_resetT:   onBtnResetT();    break;
    }

    static unsigned long millis_d02;
    static unsigned long d1 = 200;
    if((millis() > d1) && millis()-d1 > millis_d02){
        owen.readEngineTemp();
        monitor.showOwenData(owen);
        millis_d02 = millis();
    }

    static unsigned long millis_d2;
    static unsigned long d2 = 2000;
    if((millis() > d2) && millis()-d2 > millis_d2){
        millis_d2 = millis();
        checkSpaceTemperature();
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
    if(utils::isNum(value) == false){
        String str = "Owen t sensor: " + String(value);
        monitor.showError(str);
        Serial.println(str);

        delay(300);
        value = 0;
    }
    Serial.print(F("Owen temperature: "));
    Serial.println(value);
}

void checkSpaceTemperature(){
    float t = dht.readTemperature();
    owen.setCurrentSpaceT((int16_t)t);
}

void onBtnResetT(){
    owen.resetTemp();
}

void onBtnPwr(){
    Serial.println(F("PWR_BTN"));
    if (owen.active() == false && programLaunch.state() == ProgramBase::StandBy){
        programStop.stop();
        programLaunch.execute();
    }else{
        programLaunch.stop();
        programStop.execute();
    }
}

void onBtnPlus(int d){
    owen.upEngineSpeed(d);
//    Serial.println(owen.targetPWM());
}

void onBtnMinus(int d){
    owen.downEngineSpeed(d);
//    Serial.println(owen.targetPWM());
}

void onBtnLeft(){
    Serial.println(F("LEFT_BTN"));
    if(targetT > 15){
        --targetT;
        EEPROM.update(0, targetT);
        owen.setTargetSpaceT(targetT);
    }
}

void onBtnRight(){
    Serial.println(F("RIGHT_BTN"));
    if(targetT < 25){
        ++targetT;
        EEPROM.update(0, targetT);
        owen.setTargetSpaceT(targetT);
    }
}

void onBtnIgnition(){
    programLaunch.stop();
    programStop.stop();
    Serial.println(F("IGNT_BTN"));
    owen.ignition() ? owen.stopIgnition() : owen.startIgnition();
}

void onBtnPump(){
    programLaunch.stop();
    programStop.stop();
    Serial.println(F("PUMP_BTN"));
    owen.pump() ? owen.stopPump() : owen.startPump();
}
