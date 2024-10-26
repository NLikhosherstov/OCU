#include "main.h"

#include <GyverTimers.h>
#include <EncButton.h>

#include "common.h"

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

    loadConfig();

    dht.begin();
    checkSpaceTemperature();

    monitor.start();
    checkOwenTemperature();

    Timer1.setFrequency(1000);
    Timer1.enableISR(CHANNEL_A);
}

void loop() {
    switch(btns.button()){
        case Buttons::btn_noBtn:                      break;
        case Buttons::btn_power:    onBtnPwr();       break;
        case Buttons::btn_up:       onBtnUp();        break;
        case Buttons::btn_pump:     onBtnLeft();      break;
        case Buttons::btn_ignition: onBtnRight();     break;
        case Buttons::btn_down:     onBtnDown();      break;
    }

    static unsigned long millis_d02 = 0;
    static unsigned long d1 = 200;
    if((millis() > d1) && millis()-d1 > millis_d02){
        owen.readEngineTemp();
        monitor.updateData(owen);
        millis_d02 = millis();
    }

    static unsigned long millis_d2 = 0;
    static unsigned long d2 = 2000;
    if((millis() > d2) && millis()-d2 > millis_d2){
        millis_d2 = millis();
        if(!monitor.menuMode())
            checkSpaceTemperature();
    }
}

ISR(TIMER1_A) {
    owen.pumpPulse();

    eb.tick();
    if (eb.left())       onEncoderMinus(eb.fast());
    else if (eb.right()) onEncoderPlus(eb.fast());
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
        delay(300);
        value = 0;
    }
}

void checkSpaceTemperature(){
    float t = dht.readTemperature();
    spaceT() = (char)t;
}

void onBtnUp(){
    if(monitor.menuMode()){
        monitor.upItem();
    }else{

    }
}

void onBtnDown(){
    owen.resetTemp();
    if(monitor.menuMode())
        monitor.downItem();
}

void onBtnPwr(){
    if(monitor.menuMode()){
    }else{
        if (owen.automatic() == false && programLaunch.state() == ProgramBase::StandBy){
            programStop.stop();
            programLaunch.execute();
        }else{
            programLaunch.stop();
            programStop.execute();
        }
    }
}

void onEncoderPlus(bool fast){
    if(monitor.menuMode()){
        monitor.increaseValue(owen, fast);
    }else{
        programLaunch.stop();
        programStop.stop();
        owen.setAutomatic(false);
        owen.upEngineSpeed(fast ? 20 : 1);
    }
}

void onEncoderMinus(bool fast){
    if(monitor.menuMode()){
        monitor.decreaseValue(owen, fast);
    }else{
        programLaunch.stop();
        programStop.stop();
        owen.setAutomatic(false);
        owen.downEngineSpeed(fast ? 20 : 1);
    }
}

void onBtnRight(){
    if(monitor.menuMode()){
        monitor.setMenuMode(false);
        monitor.setSettingsChanged(false);
    }else{
        programLaunch.stop();
        programStop.stop();
        owen.setAutomatic(false);
        owen.ignition() ? owen.stopIgnition() : owen.startIgnition();
    }
}

void onBtnLeft(){
    if(monitor.menuMode()){
        monitor.setMenuMode(false);
        monitor.setSettingsChanged(false);
    }else{
        programLaunch.stop();
        programStop.stop();
        owen.setAutomatic(false);
        owen.pump() ? owen.stopPump() : owen.startPump();
    }
}

void onEncoderClick(){
    monitor.setMenuMode(!monitor.menuMode());
    if(!monitor.menuMode() && monitor.settingsChanged()){
        saveConfig();
        monitor.setSettingsChanged(false);
    }
}

void onEncoderLongClick(){
    if(monitor.menuMode()){
//        owen.setFuelCorrection(0);
    }
}
