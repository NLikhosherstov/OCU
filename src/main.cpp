#include "main.h"

#include <GyverTimers.h>
#include <EncButton.h>

#include "common.h"

Monitor monitor;
Buttons btns;

EncButton eb(10, 11, 12);
static bool kbdConfigMode = false;

Owen           owen;
ProgramLaunch2 programLaunch;
ProgramStop    programStop;

#define DHTPIN 4
DHT dht(DHTPIN, DHT22);

void setup() {
    Serial.begin(115200);

    loadConfig();

    btns.updateBtnData();

    eb.setEncReverse(1);

    dht.begin();
    checkSpaceTemperature();

    monitor.start();
    checkOwenTemperature();

    Timer1.setFrequency(1000);
    Timer1.enableISR(CHANNEL_A);
}

void loop() {
    if(!kbdConfigMode){
        switch(btns.button()){
            case Buttons::btn_noBtn:                      break;
            case Buttons::btn_power:    onBtnPwr();       break;
            case Buttons::btn_up:       onBtnUp();        break;
            case Buttons::btn_pump:     onBtnLeft();      break;
            case Buttons::btn_ignition: onBtnRight();     break;
            case Buttons::btn_down:     onBtnDown();      break;
        }
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

    if(!monitor.btnConfigMode() && kbdConfigMode){
        kbdConfigMode = false;
        btns.setBtnConfigMode(false);
        saveConfig();
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
    if(monitor.constTempMode()){
        Cfg().constTemp += 1;
        monitor.restartTempCounter();
    }else if(monitor.menuMode()){
        monitor.upItem();
    }else{
        if(owen.pump() && !Cfg().embededPump){
            short period = owen.targetPumpPeriod() + Cfg().correction;
            if(period == Cfg().consumption1){
                owen.setTargetPumpPeriod(Cfg().consumption2);
            }else if(period == Cfg().consumption2){
                owen.setTargetPumpPeriod(Cfg().consumption3);
            }else if(period == Cfg().consumption3){
                owen.setTargetPumpPeriod(Cfg().consumption4);
            }
        }else{
            monitor.setConstTempMode(true);
        }
    }
}

void onBtnDown(){
    owen.resetTemp();

    if(monitor.constTempMode()){
        Cfg().constTemp -= 1;
        monitor.restartTempCounter();
    }else if(monitor.menuMode()){
        monitor.downItem();
    }else{
        if(owen.pump() && !Cfg().embededPump){
            short period = owen.targetPumpPeriod() + Cfg().correction;
            if(period == Cfg().consumption4){
                owen.setTargetPumpPeriod(Cfg().consumption3);
            }else if(period == Cfg().consumption3){
                owen.setTargetPumpPeriod(Cfg().consumption2);
            }else if(period == Cfg().consumption2){
                owen.setTargetPumpPeriod(Cfg().consumption1);
            }
        }else{
            monitor.setConstTempMode(true);
        }
    }
}

void onBtnPwr(){
    if(monitor.menuMode()){
    }else{
        if(int(owen.currTemp()) >= 40){
            if(programStop.state() == ProgramBase::Cooling){
                programStop.stop();
                programLaunch.execute();
            }else if(programLaunch.isStopped()){
                programStop.stop();
                programLaunch.execute();
            }else{
                programLaunch.stop();
                programStop.execute();
            }
        }else{
            if(programStop.state() == ProgramBase::Cooling && int(owen.currTemp()) == 0){
                programStop.stop();
                owen.stopEngine();
                owen.setAutomatic(false);
            }else{
                if(programLaunch.isStopped()){
                    programStop.stop();
                    programLaunch.execute();
                }else{
                    programLaunch.stop();
                    programStop.execute();
                }
            }
        }
    }
}

void onEncoderPlus(bool fast){
    if(monitor.constTempMode()){
        Cfg().constTemp += fast ? 5 : 1;
        monitor.restartTempCounter();
    }else if(kbdConfigMode){
        btnSettingNumber() += 1;
        if(btnSettingNumber() > 4)
            btnSettingNumber() = 0;
    }else if(monitor.menuMode()){
        monitor.increaseValue(owen, fast);
    }else{
        programLaunch.stop();
        programStop.stop();
        owen.setAutomatic(false);
        owen.upEngineSpeed(fast ? 20 : 1);
    }
}

void onEncoderMinus(bool fast){
    if(monitor.constTempMode()){
        Cfg().constTemp -= fast ? 5 : 1;
        monitor.restartTempCounter();
    }else if(kbdConfigMode){
        btnSettingNumber() -= 1;
        if(btnSettingNumber() < 0)
            btnSettingNumber() = 4;
    }else if(monitor.menuMode()){
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
    if(kbdConfigMode){
//        monitor.setBtnConfigMode(false);
    }else{
        monitor.setMenuMode(!monitor.menuMode());
        if(!monitor.menuMode() && monitor.settingsChanged()){
            saveConfig();
            monitor.setSettingsChanged(false);
        }
    }
}

void onEncoderLongClick(){
    if(kbdConfigMode){
        monitor.setBtnConfigMode(false);
    }else{
        //update kbd resistance
        kbdConfigMode = true;
        btnSettingNumber() = 0;
        monitor.setBtnConfigMode(true);
        btns.setBtnConfigMode(true);
    }
//    if(monitor.menuMode()){
//        owen.setFuelCorrection(0);
//    }else{

//    }
}
