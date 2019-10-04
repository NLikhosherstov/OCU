#ifndef OWEN_H
#define OWEN_H

#include <Arduino.h>

#include "max6675.h"

#include "common.h"

#define PUMP PIN_A0 //Pump relay pin
#define IGNT PIN_A1 //Ignition relay pin
#define RPWM PIN3   //engine control pin
#define LPWM PIN6   //engine control pin

class Owen
{
public:
	Owen();

	//ENGINE TEMPERATURE
	void	filtrateTemp();
    double	readEngineTemp();
	double  currTempSpeed ();
	double  currTemp();

	//ENGINE SPEED
	void startEngine();
	void stopEngine();
	void upEngineSpeed(const uint8_t &dif);
	void downEngineSpeed(const uint8_t &dif);
	void setEngineSpeed(uint8_t pwm);
	int8_t currentEngineSpeed() const;

    //Call always in interrupt handler
    void changeEngineSpeed();

	//ENGINE PUMP
	void startPump();
	void stopPump();

	//ENGINE IGNITION
	void startIgnition();
	void stopIgnition();

	//ENGINE STATUS
	bool active() const;
	void setActive(bool active);

private:
    bool m_active   = false;
    bool m_engine   = false;
    bool m_pump     = false;
    bool m_ignition = false;

    int m_currentPWM = 0; //текущяя скорость
    int m_targetPWM  = 0; //целевая скорость

	double m_newTemp;
	double m_currTemp;
    double m_currTempSpeed = 0;

private:
    const int m_pwmResolution = 255;
    const int m_pwmStep = 10;

    //Temperature filtrate constant's
    const double m_k             =  0.8;
    const double m_dt            =  0.1; //sec
    const double m_diffThreshold = 30.0; // deg

private:
};

#endif // OWEN_H


