#ifndef MONITOR_H
#define MONITOR_H

#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#define USE_MICRO_WIRE
#include <GyverOLED.h>

#include "Owen/owen.h"

//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)#define NUMFLAKES     10 // Number of snowflakes in the animation example

//#define LOGO_HEIGHT   16
//#define LOGO_WIDTH    16
//#define NUMFLAKES 3

class Monitor
{
public:
	Monitor();
	void start();

    void showOwenData(const Owen &owen);
    void showError(const String &str);

    void setTitleText(const String &str);

    void test();

private:
    void testdrawline();
    void testdrawrect();
	void testfillrect();
	void testdrawcircle();
	void testfillcircle();
	void testdrawroundrect();
	void testfillroundrect();
	void testdrawtriangle();
	void testfilltriangle();
	void testdrawchar();
	void testdrawstyles();
	void testscrolltext();
	void testdrawbitmap();
	void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h);

private:
	String m_title;
	String m_body;
};

#endif // MONITOR_H
