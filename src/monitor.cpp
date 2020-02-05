#include "monitor.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

Monitor::Monitor(){
}

void Monitor::start(){
	if(!display.begin(SSD1306_SWITCHCAPVCC)) {
	  Serial.println(F("SSD1306 allocation failed"));
	  for(;;); // Don't proceed, loop forever
	}
	// Show initial display
	display.display();
    delay(500);
	display.clearDisplay();

    Serial.println(F("Monitor started"));
}

void Monitor::showOwenData(const Owen &owen){
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.cp437(true);

    String temperature = "T:" + String((int)owen.currTemp()) + "     dT:" + String((int)owen.currTempSpeed());
    display.setCursor(0, 0);
    display.print(temperature);

    String equipment = "Pump:  " + String(owen.pump()?1:0) + "   Ign:" + String(owen.ignition()?1:0);
    display.setCursor(0, 10);
    display.print(equipment);

    String engine = "Active:" + String(owen.active()?1:0) + "   PWM:" + String(map(owen.currentEngineSpeed(), 0, 254, 0, 100) );
    display.setCursor(0, 20);
    display.print(engine);

    display.display();
}

void Monitor::showTemperatures(int16_t owen, int16_t space)
{
	display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.cp437(true);
	display.print(F("CurrentData:"));

	String t = "t:" + String(owen) + "  St:" + String(space);
	display.setTextSize(2);
	display.setCursor(0, 10);
	display.print(t);

    display.display();
}

void Monitor::showError(const String &str)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
    display.print(F("ERROR:"));

    display.setTextSize(1);
    display.setCursor(0, 10);
    display.print(str);

    display.display();
}

void Monitor::setTemperatures(int16_t owen, int16_t space){
}

void Monitor::test(){
	testdrawline();      // Draw many lines
	testdrawrect();      // Draw rectangles (outlines)
	testfillrect();      // Draw rectangles (filled)
	testdrawcircle();    // Draw circles (outlines)
	testfillcircle();    // Draw circles (filled)
	testdrawroundrect(); // Draw rounded rectangles (outlines)
	testfillroundrect(); // Draw rounded rectangles (filled)
	testdrawtriangle();  // Draw triangles (outlines)
	testfilltriangle();  // Draw triangles (filled)
	testdrawchar();      // Draw characters of the default font
	testdrawstyles();    // Draw 'stylized' characters
	testscrolltext();    // Draw scrolling text
	testdrawbitmap();    // Draw a small bitmap image
	testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}

void Monitor::testdrawline() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
	display.drawLine(0, 0, i, display.height()-1, WHITE);
	display.display(); // Update screen with each newly-drawn line
	delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
	display.drawLine(0, 0, display.width()-1, i, WHITE);
	display.display();
	delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
	display.drawLine(0, display.height()-1, i, 0, WHITE);
	display.display();
	delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
	display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
	display.display();
	delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
	display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
	display.display();
	delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
	display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
	display.display();
	delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
	display.drawLine(display.width()-1, 0, 0, i, WHITE);
	display.display();
	delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
	display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
	display.display();
	delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void Monitor::testdrawrect() {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
	display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
	display.display(); // Update screen with each newly-drawn rectangle
	delay(1);
  }

  delay(2000);
}

void Monitor::testfillrect() {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
	// The INVERSE color is used so rectangles alternate white/black
	display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
	display.display(); // Update screen with each newly-drawn rectangle
	delay(1);
  }

  delay(2000);
}

void Monitor::testdrawcircle() {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
	display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
	display.display();
	delay(1);
  }

  delay(2000);
}

void Monitor::testfillcircle() {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
	// The INVERSE color is used so circles alternate white/black
	display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
	display.display(); // Update screen with each newly-drawn circle
	delay(1);
  }

  delay(2000);
}

void Monitor::testdrawroundrect() {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
	display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
	  display.height()/4, WHITE);
	display.display();
	delay(1);
  }

  delay(2000);
}

void Monitor::testfillroundrect() {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
	// The INVERSE color is used so round-rects alternate white/black
	display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
	  display.height()/4, INVERSE);
	display.display();
	delay(1);
  }

  delay(2000);
}

void Monitor::testdrawtriangle() {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
	display.drawTriangle(
	  display.width()/2  , display.height()/2-i,
	  display.width()/2-i, display.height()/2+i,
	  display.width()/2+i, display.height()/2+i, WHITE);
	display.display();
	delay(1);
  }

  delay(2000);
}

void Monitor::testfilltriangle() {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
	// The INVERSE color is used so triangles alternate white/black
	display.fillTriangle(
	  display.width()/2  , display.height()/2-i,
	  display.width()/2-i, display.height()/2+i,
	  display.width()/2+i, display.height()/2+i, INVERSE);
	display.display();
	delay(1);
  }

  delay(2000);
}

void Monitor::testdrawchar() {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
	if(i == '\n') display.write(' ');
	else          display.write(i);
  }

  display.display();
  delay(2000);
}

void Monitor::testdrawstyles() {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void Monitor::testscrolltext() {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

void Monitor::testdrawbitmap() {
  display.clearDisplay();

  display.drawBitmap(
	(display.width()  - LOGO_WIDTH ) / 2,
	(display.height() - LOGO_HEIGHT) / 2,
	logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void Monitor::testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
	icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
	icons[f][YPOS]   = -LOGO_HEIGHT;
	icons[f][DELTAY] = random(1, 6);
	Serial.print(F("x: "));
	Serial.print(icons[f][XPOS], DEC);
	Serial.print(F(" y: "));
	Serial.print(icons[f][YPOS], DEC);
	Serial.print(F(" dy: "));
	Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
	display.clearDisplay(); // Clear the display buffer

	// Draw each snowflake:
	for(f=0; f< NUMFLAKES; f++) {
	  display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
	}

	display.display(); // Show the display buffer on the screen
	delay(100);        // Pause for 1/10 second

	// Then update coordinates of each flake...
	for(f=0; f< NUMFLAKES; f++) {
	  icons[f][YPOS] += icons[f][DELTAY];
	  // If snowflake is off the bottom of the screen...
	  if (icons[f][YPOS] >= display.height()) {
		// Reinitialize to a random position, just off the top
		icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
		icons[f][YPOS]   = -LOGO_HEIGHT;
		icons[f][DELTAY] = random(1, 6);
	  }
	}
  }
}
