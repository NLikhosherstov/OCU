#include "buttons.h"

volatile bool up_btn = false;
volatile bool dn_btn = false;
volatile bool pwr_btn = false;

Buttons::Buttons()
{
	pinMode(UP_BTN, INPUT_PULLUP);
	pinMode(DOWN_BTN, INPUT_PULLUP);
	pinMode(PWR_BTN, INPUT_PULLUP);

	attachInterrupt(0, Buttons::keyBoardListener, FALLING);
}

void Buttons::keyBoardListener(void){
	static unsigned long millis_prev;
	if(millis()-300 > millis_prev){
		if(digitalRead(UP_BTN) == LOW){
			up_btn = true;
		}else if(digitalRead(DOWN_BTN) == LOW){
			dn_btn = true;
		}else if(digitalRead(PWR_BTN) == LOW){
			pwr_btn = true;
		}
		millis_prev = millis();
	}
}

bool Buttons::isUp(){
	if(up_btn){
		up_btn = false;
		return true;
	}
	return false;
}

bool Buttons::isDwn(){
	if(dn_btn){
		dn_btn = false;
		return true;
	}
	return false;
}

bool Buttons::isPwr(){
	if(pwr_btn){
		pwr_btn = false;
		return true;
	}
	return false;
}
