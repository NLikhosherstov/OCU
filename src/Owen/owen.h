#ifndef OWEN_H
#define OWEN_H

#include <Arduino.h>

#include "max6675.h"

#include "common.h"

#define PUMP PIN_A0 //Pump relay pin
#define IGNT PIN_A1 //Ignition relay pin
#define RPWM PIN3   //engine control pin
#define LPWM PIN6   //engine control pin

#define MIN_START_PWM 153

#define PUMP_SINGLE_ACTUATION 0.022  // ml
#define PUMP_MAX_FLOW_SEC     0.6216 // ml/sec
#define PUMP_ACTUATION_PERIOD 18     // msec

class Owen
{
public:
	Owen();

	//ENGINE TEMPERATURE
	void	filtrateTemp();
    void    resetTemp();
    double	readEngineTemp();
    double  currTempSpeed () const;
    double  currTemp      () const;

	//ENGINE SPEED
    void tick();
	void startEngine();
	void stopEngine();
    void upEngineSpeed(const int &dif);
    void downEngineSpeed(const int &dif);
    void setEngineSpeed(int pwm);
    int currentEngineSpeed() const;

    //Call always in interrupt handler
    void changeEngineSpeed();

	//ENGINE PUMP
    void pumpPulse();
    bool pump() const;
	void startPump();
	void stopPump();

	//ENGINE IGNITION
    bool ignition() const;
	void startIgnition();
	void stopIgnition();

	//ENGINE STATUS
	bool active() const;
    void setActive(bool active);

    void checkIgnitionSafety(); ////проверка времени включения свечи

    int8_t targetSpaceT() const;
    void setTargetSpaceT(const int8_t &targetSpaceT);

    int16_t currentSpaceT() const;
    void setCurrentSpaceT(const int16_t &currentSpaceT);

    int targetPWM() const;

    double currentFuelRate() const;

private:
    bool m_active      = false;
    bool m_engine      = false;
    bool m_pump        = false;
    bool m_ignition    = false;

    int m_ignitionTimer   = 0;
    int m_ignitionMaxTick = 600;

    int m_currentPWM = 0; //текущяя скорость
    int m_targetPWM  = 0; //целевая скорость

    double m_currentFuelRate = 0; //текущий расход
    unsigned long m_millis_pumpTimer = 0;
    unsigned long m_targetPeriod = 0;

	double m_newTemp;
	double m_currTemp;
    double m_currTempSpeed = 0;

private:
    const int m_pwmResolution = 255;
    const int m_pwmStep       = 2;

    //Temperature filtrate constant's
    const double m_k             =  0.8;
    const double m_dt            =  0.1; //sec
    const double m_diffThreshold = 30.0; // deg

    int16_t m_currentSpaceT;
    int8_t  m_targetSpaceT;

private:
};

#endif // OWEN_H


