#include "monitor.h"
#include "resource.h"

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
GyverOLED<SSD1306_128x64, OLED_BUFFER> display;

Monitor::Monitor(){
}

void Monitor::start(){
    display.init();        // инициализация
    display.clear();       // очистка
//    display.setScale(3);   // масштаб текста (1..4)
//    display.home();        // курсор в 0,0
//    display.print("Привет!");
    display.update();
    Serial.println(F("Monitor started"));
}

void Monitor::showOwenData(const Owen &owen){
    display.clear();
    static double owenPumpL = 1.6;

/*********************************OWEN TEMP****************************************/
    display.drawBitmap(80 - PIC_WIDTH - 2, 0, tempOwen_7x7, PIC_WIDTH, PIC_HEIGHT, BITMAP_NORMAL, BUF_ADD);
    display.setScale(1);
    display.setCursorXY(82, 0);
    display.print(String((int)owen.currTemp()));
/******************************************************************************/
/*********************************SPACE TEMP***************************************/
    display.drawBitmap(116 - PIC_WIDTH - 1, 0, tempSpace_7x7, PIC_WIDTH, PIC_HEIGHT, BITMAP_NORMAL, BUF_ADD);
    display.setScale(1);
    display.setCursorXY(117, 0);
    display.print(String((int)owen.currentSpaceT()));
/*********************************************************************************/
/*********************************IGNITION****************************************/
    display.roundRect(101, 12, 127, 38, OLED_STROKE);
    if(owen.ignition())
        display.drawBitmap( 105, 15, ignition_20x20, IND_WIDTH, IND_HEIGHT, BITMAP_NORMAL, BUF_ADD);
/*********************************************************************************/
/*********************************FIRE********************************************/
    display.roundRect(101, 38, 127, 63, OLED_STROKE);
    if(1){
        display.drawBitmap( 105, 41, fire_20x20, IND_WIDTH, IND_HEIGHT, BITMAP_NORMAL, BUF_ADD);
    }
/*********************************************************************************/
/*********************************ACTIVITI****************************************/
    if(owen.active()){
        display.drawBitmap( 0, 0, auto_19x8, 19, 8, BITMAP_NORMAL, BUF_ADD);
    }
/*********************************************************************************/
/*********************************OWEN SPEED**************************************/
    display.drawBitmap( 0, 12, fan_24x24, ICO_WIDTH, ICO_HEIGHT, BITMAP_NORMAL, BUF_ADD);
    display.setScale(2);
    display.setCursorXY(ICO_WIDTH+5, 16);
    display.print(String(map(owen.currentEngineSpeed(), 0, 254, 0, 100)) + "%");
/*********************************************************************************/
/*********************************FUEL RATE***************************************/
    display.drawBitmap( 0, 16+ICO_HEIGHT, pump_24x24, ICO_WIDTH, ICO_HEIGHT, BITMAP_NORMAL, BUF_ADD);
    display.setScale(2);
    display.setCursorXY(ICO_WIDTH+5, 16+ICO_HEIGHT+8);
    display.print(String((double)owen.currentFuelRate()) + "Lh");
/*********************************************************************************/

    display.update();
}

void Monitor::showError(const String &str)
{
//    display.clearDisplay();

//    display.setTextSize(1);
//    display.setTextColor(WHITE);
//    display.setCursor(0, 0);
//    display.cp437(true);
//    display.print(F("ERROR:"));

//    display.setTextSize(1);
//    display.setCursor(0, 10);
//    display.print(str);

//    display.display();
}
