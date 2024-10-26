#include "monitor.h"
#include "resource.h"

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
GyverOLED<SSD1306_128x64, /*OLED_NO_BUFFER*/OLED_BUFFER> display;

Monitor::Monitor(){
}

void Monitor::start(){
    display.init();
    display.clear();
    display.textMode(BUF_ADD);
    display.update();
}

void Monitor::updateData(const Owen &owen){
    display.clear();

    /***HEADER***/
    /*********************************ACTIVITY****************************************/
        if(owen.automatic()){
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
        display.print((int)spaceT());
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
        short currentPercantage = map(owen.targetPWM(), 0, 254, 0, 100);
        short spacing = 2;
        short margin = ICO_WIDTH+5;
        short rectWidth = 15;
        short pen = OLED_FILL;

        if(currentPercantage < 55) pen = OLED_STROKE;
        display.roundRect(margin, 28, margin+rectWidth, 34, pen);

        margin = margin + rectWidth + spacing;
        if(currentPercantage < 70) pen = OLED_STROKE;
        display.roundRect(margin, 24, margin+rectWidth, 34, pen);

        margin = margin + rectWidth + spacing;
        if(currentPercantage < 85) pen = OLED_STROKE;
        display.roundRect(margin, 19, margin+rectWidth, 34, pen);

        margin = margin + rectWidth + spacing;
        if(currentPercantage < 99) pen = OLED_STROKE;
        display.roundRect(margin, 14, margin+rectWidth, 34, pen);
    /*********************************************************************************/
    /*********************************FUEL RATE***************************************/
        display.drawBitmap( 0, 16+ICO_HEIGHT, pump_24x24, ICO_WIDTH, ICO_HEIGHT, BITMAP_NORMAL, BUF_ADD);

        if(Cfg().correction){
            display.setScale(1);
            display.setCursorXY(ICO_WIDTH+5, 16+ICO_HEIGHT-3);
            display.print((Cfg().correction>0 ? "+" : "") + String(Cfg().correction));
        }
        display.setScale(2);
        display.setCursorXY(ICO_WIDTH+5, 16+ICO_HEIGHT+4);
        if(owen.pump()) display.print(String(owen.currentFuelRate()) + "Lh");
        else            display.print("-.--Lh");
    /*********************************************************************************/
}

void Monitor::showMenuData(){
    String s;
//    Serial.println(F("Menu mode"));
    display.setScale(1);
    static const char margin = 10;
    static const char spacing = 11;

    if(m_currentItem < 4){
        s = F("Correct.  ");
        if(m_currentItem == 0) display.invertText(true);
        display.setCursorXY(0, margin);
        display.print(s + String(Cfg().correction));
        display.invertText(false);

        s = F("Max flow  ");
        if(m_currentItem == 1) display.invertText(true);
        display.setCursorXY(0, margin+spacing);
        display.print(s + String(Cfg().owenMaxFlow*3.6) + " lh");
        display.invertText(false);

        s = F("Pump perf ");
        if(m_currentItem == 2) display.invertText(true);
        display.setCursorXY(0, margin+spacing*2);
        display.print(s + String(Cfg().pumpPerfomance*100) + " ml/100");
        display.invertText(false);

        s = F("Emb.pump  ");
        if(m_currentItem == 3) display.invertText(true);
        display.setCursorXY(0, margin+spacing*3);
        display.print(s + String(Cfg().embededPump));
        display.invertText(false);
    }else if(m_currentItem < 8){
        s = F("Speed 1:  ");
        if(m_currentItem == 4) display.invertText(true);
        display.setCursorXY(0, margin);
        display.print(s + String(map(Cfg().speed1, 0, 254, 0, 100)) + "%");
        display.invertText(false);

        s = F("Speed 2:  ");
        if(m_currentItem == 5) display.invertText(true);
        display.setCursorXY(0, margin+spacing);
        display.print(s + String(map(Cfg().speed2, 0, 254, 0, 100)) + "%");
        display.invertText(false);

        s = F("Speed 3:  ");
        if(m_currentItem == 6) display.invertText(true);
        display.setCursorXY(0, margin+spacing*2);
        display.print(s + String(map(Cfg().speed3, 0, 254, 0, 100)) + "%");
        display.invertText(false);

        s = F("Speed 4:  ");
        if(m_currentItem == 7) display.invertText(true);
        display.setCursorXY(0, margin+spacing*3);
        display.print(s + String(map(Cfg().speed4, 0, 254, 0, 100)) + "%");
        display.invertText(false);
    }else if(m_currentItem < 12){
        s = F("Cons. 1:  ");
        if(m_currentItem == 8) display.invertText(true);
        display.setCursorXY(0, margin);
        display.print(s + String(utils::fuelRate(Cfg().pumpPerfomance, Cfg().consumption1)) + " lh");
        display.invertText(false);

        s = F("Cons. 2:  ");
        if(m_currentItem == 9) display.invertText(true);
        display.setCursorXY(0, margin+spacing);
        display.print(s + String(utils::fuelRate(Cfg().pumpPerfomance, Cfg().consumption2)) + " lh");
        display.invertText(false);

        s = F("Cons. 3:  ");
        if(m_currentItem == 10) display.invertText(true);
        display.setCursorXY(0, margin+spacing*2);
        display.print(s + String(utils::fuelRate(Cfg().pumpPerfomance, Cfg().consumption3)) + " lh");
        display.invertText(false);

        s = F("Cons. 4:  ");
        if(m_currentItem == 11) display.invertText(true);
        display.setCursorXY(0, margin+spacing*3);
        display.print(s + String(utils::fuelRate(Cfg().pumpPerfomance, Cfg().consumption4)) + " lh");
        display.invertText(false);
    }
}

void Monitor::increaseValue(Owen &owen, bool fast){
    switch (m_currentItem) {
    case 0:
        Cfg().correction += fast ? 5 : 1;
        if(owen.currentPWM() == 0 && owen.pump()) owen.calcPumpPeriod(255);
        else                                      owen.calcPumpPeriod();
        break;
    case 1:
        Cfg().owenMaxFlow += fast ? 0.028 : 0.0028;
        break;
    case 2:
        Cfg().pumpPerfomance += fast ? 0.001 : 0.0001;
        break;
    case 3:
        Cfg().embededPump = !Cfg().embededPump;
        break;
    case 4:
        Cfg().speed1 = min(254, Cfg().speed1 + (fast ? 6 : 2));
        break;
    case 5:
        Cfg().speed2 = min(254, Cfg().speed2 + (fast ? 6 : 2));
        break;
    case 6:
        Cfg().speed3 = min(254, Cfg().speed3 + (fast ? 6 : 2));
        break;
    case 7:
        Cfg().speed4 = min(254, Cfg().speed4 + (fast ? 6 : 2));
        break;
    case 8:
        Cfg().consumption1 = max(0, Cfg().consumption1 - (fast ? 6 : 2));
        break;
    case 9:
        Cfg().consumption2 = max(0, Cfg().consumption2 - (fast ? 6 : 2));
        break;
    case 10:
        Cfg().consumption3 = max(0, Cfg().consumption3 - (fast ? 6 : 2));
        break;
    case 11:
        Cfg().consumption4 = max(0, Cfg().consumption4 - (fast ? 6 : 2));
        break;
    default:
        break;
    }
    setSettingsChanged(true);
}

void Monitor::decreaseValue(Owen &owen, bool fast){
    switch (m_currentItem) {
    case 0:
        Cfg().correction -= fast ? 5 : 1;
        if(owen.currentPWM() == 0 && owen.pump()) owen.calcPumpPeriod(255);
        else                                      owen.calcPumpPeriod();
        break;
    case 1:
        Cfg().owenMaxFlow = max(0, Cfg().owenMaxFlow - (fast ? 0.028 : 0.0028));
        break;
    case 2:
        Cfg().pumpPerfomance = max(0, Cfg().pumpPerfomance - (fast ? 0.001 : 0.0001));
        break;
    case 3:
        Cfg().embededPump = !Cfg().embededPump;
        break;
    case 4:
        Cfg().speed1 = max(0, Cfg().speed1 - (fast ? 6 : 2));
        break;
    case 5:
        Cfg().speed2 = max(0, Cfg().speed2 - (fast ? 6 : 2));
        break;
    case 6:
        Cfg().speed3 = max(0, Cfg().speed3 - (fast ? 6 : 2));
        break;
    case 7:
        Cfg().speed4 = max(0, Cfg().speed4 - (fast ? 6 : 2));
        break;
    case 8:
        Cfg().consumption1 = min(400, Cfg().consumption1 + (fast ? 6 : 2));
        break;
    case 9:
        Cfg().consumption2 = min(400, Cfg().consumption2 + (fast ? 6 : 2));
        break;
    case 10:
        Cfg().consumption3 = min(400, Cfg().consumption3 + (fast ? 6 : 2));
        break;
    case 11:
        Cfg().consumption4 = min(400, Cfg().consumption4 + (fast ? 6 : 2));
        break;
    default:
        break;
    }
    setSettingsChanged(true);
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
