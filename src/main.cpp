#include "main.h"
#include "max6675.h"

void keyBoardListener(void);

//Monitor mnt;
Buttons btns;
Owen owen;

int thermoDO = 2;  //он же SO
int thermoCS = 3;
int thermoCLK = 4;  //он же SCK
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
	Serial.begin(9600);
//	mnt.start();
}

void loop() {
	if(btns.isUp()){
		Serial.println(F("UP_BTN"));
	}else if(btns.isDwn()){
		Serial.println(F("DWN_BTN"));
	}else if(btns.isPwr()){
		Serial.println(F("PWR_BTN"));
	}
}


