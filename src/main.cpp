#include "main.h"

#include <GyverTimers.h>

uint8_t targetT = 20;

Monitor monitor;
Buttons btns;

EncButton eb(10, 11, 12);

Owen           owen;
ProgramLaunch2 programLaunch;
ProgramStop    programStop;

#define DHTPIN 4
DHT dht(DHTPIN, DHT22);

#define DATA_FLAG 0xAA
struct Settings{
    int correction = 0; //Коррекция расхода топлива
} deviceSettings;


void loadSettings()
{
    if (EEPROM.read(0x0) == DATA_FLAG) {    // Проверяем наличие маркера
        EEPROM.get(0x1, deviceSettings);    // Если маркер есть, загружаем настройки из EEPROM
        owen.setFuelCorrection(deviceSettings.correction);
    } else {
        EEPROM.write(0x0, DATA_FLAG);       // Записываем маркер, указывающий наличие данных
        EEPROM.put(0x1, deviceSettings);    // Сохраняем данные в EEPROM, начиная с адреса 0x1
        Serial.println(F("Writing EEPROM set default correction"));
    }
}

void saveSettings()
{
    bool needSave = false;
    if(deviceSettings.correction != owen.fuelCorrection()){
        deviceSettings.correction = owen.fuelCorrection();
        needSave = true;
    }

    if(needSave){
        EEPROM.write(0x0, DATA_FLAG);     // Записываем маркер, указывающий наличие данных
        EEPROM.put(0x1, deviceSettings);  // Сохраняем данные в EEPROM, начиная с адреса 0x1

        Serial.println(F("Writing EEPROM new correction"));
    }
}

void setup() {
    Serial.begin(115200);
    eb.setEncReverse(1);

    owen.setTargetSpaceT(targetT);
    loadSettings();

    dht.begin();
    checkSpaceTemperature();

    monitor.start();
    checkOwenTemperature();

    Timer1.setFrequency(1000);
    Timer1.enableISR(CHANNEL_A);

//    setupTimerInterrupt();
}

void loop() {
    switch(btns.button()){
        case Buttons::btn_noBtn:                      break;
        case Buttons::btn_power:    onBtnPwr();       break;
        case Buttons::btn_space:                      break;
        case Buttons::btn_pump:     onBtnPump();      break;
        case Buttons::btn_ignition: onBtnIgnition();  break;
        case Buttons::btn_resetT:   onBtnResetT();    break;
    }

    static unsigned long millis_d02 = 0;
    static unsigned long d1 = 200;
    if((millis() > d1) && millis()-d1 > millis_d02){
        owen.readEngineTemp();
        monitor.showOwenData(owen);
        millis_d02 = millis();
    }

    static unsigned long millis_d2 = 0;
    static unsigned long d2 = 2000;
    if((millis() > d2) && millis()-d2 > millis_d2){
        millis_d2 = millis();
        checkSpaceTemperature();
    }
}

//void setupTimerInterrupt(void){
//    cli();
//    {
//        TCCR1A  = 0;             // установить регистры в 0
//        TCCR1B  = 0;

//        TIMSK1 |= (1 << OCIE1A); // включение прерываний по совпадению
//        TCCR1B |= (1 << WGM12 ); // включение в CTC режим

//        OCR1A   = 1562;          // установка регистра совпадения
//        TCCR1B |= (1 << CS10  ); // Установка битов CS10 на коэффициент деления 1024
//        TCCR1B |= (1 << CS12  ); // Установка битов CS12 на коэффициент деления 1024
//    }
//    sei();
//}

//ISR(TIMER1_COMPA_vect){
//    btns.keyBoardListener();
//    owen.changeEngineSpeed();
//    owen.checkIgnitionSafety();
//    owen.filtrateTemp();
//    programLaunch.update(owen);
//    programStop.update(owen);


//    Serial.println(F("TIMER1_COMPA_vect"));
//}

ISR(TIMER1_A) {
    owen.pumpPulse();

    eb.tick();
    if (eb.left())       onBtnMinus(eb.fast());
    else if (eb.right()) onBtnPlus(eb.fast());
    else if (eb.click()) onEncoderClick();
    else if (eb.hold())  onEncoderLongClick();

    static unsigned i = 0;
    if(i % 10 == 0)
        owen.changeEngineSpeed();

    if(i >= 99){
        btns.keyBoardListener();
        owen.checkIgnitionSafety();
        owen.filtrateTemp();
        programLaunch.update(owen);
        programStop.update(owen);
        i = 0;
    }else{
        i++;
    }
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

void onBtnPlus(bool fast){
    if(monitor.fuelRateSetting()){
        owen.addFuelCorrection(fast ? 5 : 1);
    }else{
        owen.upEngineSpeed(fast ? 20 : 3);
    }
}

void onBtnMinus(bool fast){
    if(monitor.fuelRateSetting()){
        owen.addFuelCorrection(fast ? -5 : -1);
    }else{
        owen.downEngineSpeed(fast ? 10 : 3);
    }
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

void onEncoderClick(){
    monitor.setFuelRateSetting(!monitor.fuelRateSetting());
    if(!monitor.fuelRateSetting())
        saveSettings();
}

void onEncoderLongClick()
{
    if(monitor.fuelRateSetting())
        owen.setFuelCorrection(0);
}
