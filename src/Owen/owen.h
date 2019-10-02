#ifndef OWEN_H
#define OWEN_H

#include <Arduino.h>
#include "max6675.h"

#include "common.h"

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
	bool m_active;

	bool m_engine;
	bool m_pump;
	bool m_ignition;

	double m_newTemp;
	double m_currTemp;
	double m_currTempSpeed;

	int m_pwmResolution = 255;
	int m_RPWM			= 3;
	int m_LPWM			= 6;
	int m_currentPWM	= 0;
};

#endif // OWEN_H


