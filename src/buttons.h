#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define UP_BTN 10
#define DOWN_BTN 11
#define PWR_BTN 12

class Buttons
{
public:
	Buttons();

public:
	bool isUp();
	bool isDwn();
	bool isPwr();

private:
	static void keyBoardListener(void);
};

#endif // BUTTONS_H
