#include "monitor.h"
#include "resource.h"

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
GyverOLED<SSD1306_128x64, OLED_BUFFER> display;

Monitor::Monitor(){
}

void Monitor::start(){
    display.init();
    display.clear();
    display.textMode(BUF_ADD);
    display.update();

    Serial.println(F("Monitor started"));
}

void Monitor::updateData(const Owen &owen){
    display.clear();

    /***HEADER***/
    /*********************************ACTIVITY****************************************/
        if(owen.active()){
            display.drawBitmap( 0, 0, auto_19x8, 19, 8, BITMAP_NORMAL, BUF_ADD);
        }
    /*********************************************************************************/
    /*********************************OWEN TEMP****************************************/
        display.drawBitmap(80 - PIC_WIDTH - 2, 0, tempOwen_7x7, PIC_WIDTH, PIC_HEIGHT, BITMAP_NORMAL, BUF_ADD);
        display.setScale(1);
        display.setCursorXY(82, 0);
        display.print((int)owen.currTemp());
    /******************************************************************************/
    /*********************************SPACE TEMP***************************************/
        display.drawBitmap(116 - PIC_WIDTH - 1, 0, tempSpace_7x7, PIC_WIDTH, PIC_HEIGHT, BITMAP_NORMAL, BUF_ADD);
        display.setScale(1);
        display.setCursorXY(117, 0);
        display.print((int)owen.currentSpaceT());
    /*********************************************************************************/

    if(menuMode()){
        showMenuData();
    }else{
        showOwenData(owen);
    }

    display.update();
}

void Monitor::showOwenData(const Owen &owen)
{
    /*********************************IGNITION****************************************/
        display.roundRect(101, 12, 127, 38, OLED_STROKE);
        if(owen.ignition())
            display.drawBitmap( 105, 15, ignition_20x20, IND_WIDTH, IND_HEIGHT, BITMAP_NORMAL, BUF_ADD);
    /*********************************************************************************/
    /*********************************PUMP********************************************/
        display.roundRect(101, 38, 127, 63, OLED_STROKE);
        if(owen.pump())
            display.drawBitmap( 105, 41, fire_20x20, IND_WIDTH, IND_HEIGHT, BITMAP_NORMAL, BUF_ADD);
    /*********************************************************************************/
    /*********************************OWEN SPEED**************************************/
        display.drawBitmap( 0, 12, fan_24x24, ICO_WIDTH, ICO_HEIGHT, BITMAP_NORMAL, BUF_ADD);
        display.setScale(2);
        display.setCursorXY(ICO_WIDTH+5, 16);
        display.print(String(map(owen.targetPWM(), 0, 254, 0, 100)) + "%");
    /*********************************************************************************/
    /*********************************FUEL RATE***************************************/
        display.drawBitmap( 0, 16+ICO_HEIGHT, pump_24x24, ICO_WIDTH, ICO_HEIGHT, BITMAP_NORMAL, BUF_ADD);

        display.setScale(2);
        display.setCursorXY(ICO_WIDTH+5, 16+ICO_HEIGHT+6);
        display.print(String(owen.currentFuelRate()) + "Lh");
        if(Cfg().data.correction){
            display.setScale(1);
            display.setCursorXY(ICO_WIDTH+5, 16+ICO_HEIGHT-3);
            display.print((Cfg().data.correction>0 ? "+" : "") + String(Cfg().data.correction));
        }
    /*********************************************************************************/
}

void Monitor::showMenuData(){
//    Serial.println(F("Menu mode"));
    display.setScale(1);
    static const char margin = 10;
    static const char spacing = 10;

    if(m_currentItem == 0) display.invertText(true);
    display.setCursorXY(0, margin);
    display.print("Correct.  " + String(Cfg().data.correction));
    display.invertText(false);

    if(m_currentItem == 1) display.invertText(true);
    display.setCursorXY(0, margin+spacing);
    display.print("Max flow  " + String(Cfg().data.owenMaxFlow) + " lh");
    display.invertText(false);

    if(m_currentItem == 2) display.invertText(true);
    display.setCursorXY(0, margin+spacing*2);
    display.print("Pump perf " + String(Cfg().data.pumpMaxPerfomance) + " ml/100");
    display.invertText(false);

    if(m_currentItem == 3) display.invertText(true);
    display.setCursorXY(0, margin+spacing*3);
    display.print("Emb.pump  " + String(Cfg().data.embededPump));
    display.invertText(false);
}

void Monitor::increaseValue(Owen &owen, bool fast){
    switch (m_currentItem) {
    case 0:
        Cfg().data.correction += fast ? 5 : 1;
        if(owen.currentPWM() == 0 && owen.pump()) owen.calcPumpPeriod(255);
        else                                      owen.calcPumpPeriod();
        setSettingsChanged(true);
        break;
    case 1:
        Cfg().data.owenMaxFlow += fast ? 0.1 : 0.05;
        setSettingsChanged(true);
        break;
    case 2:
        Cfg().data.pumpMaxPerfomance += fast ? 0.5 : 0.1;
        setSettingsChanged(true);
        break;
    case 3:
        Cfg().data.embededPump = !Cfg().data.embededPump;
        setSettingsChanged(true);
        break;
    default:
        break;
    }
}

void Monitor::decreaseValue(Owen &owen, bool fast){
    switch (m_currentItem) {
    case 0:
        Cfg().data.correction -= fast ? 5 : 1;
        if(owen.currentPWM() == 0 && owen.pump()) owen.calcPumpPeriod(255);
        else                                      owen.calcPumpPeriod();
        setSettingsChanged(true);
        break;
    case 1:
        Cfg().data.owenMaxFlow = max(0, Cfg().data.owenMaxFlow - (fast ? 0.1 : 0.05));
        setSettingsChanged(true);
        break;
    case 2:
        Cfg().data.pumpMaxPerfomance = max(0, Cfg().data.pumpMaxPerfomance - (fast ? 0.5 : 0.1));
        setSettingsChanged(true);
        break;
    case 3:
        Cfg().data.embededPump = !Cfg().data.embededPump;
        setSettingsChanged(true);
        break;
    default:
        break;
    }
}

bool Monitor::settingsChanged() const{
    return m_settingsChanged;
}

void Monitor::setSettingsChanged(bool newSettingsChanged){
    m_settingsChanged = newSettingsChanged;
}

void Monitor::showError(const String &/*str*/){
}

bool Monitor::menuMode() const{
    return m_menuMode;
}

void Monitor::setMenuMode(bool newMenuMode){
    m_menuMode = newMenuMode;
}

void Monitor::upItem(){
    m_currentItem--;
    if(m_currentItem < 0)
        m_currentItem = m_itemsCount-1;
}

void Monitor::downItem(){
    m_currentItem++;
    if(m_currentItem >= m_itemsCount)
        m_currentItem = 0;
}
