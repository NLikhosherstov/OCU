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

#define PUMP_SINGLE_ACTUATION 0.064  // ml per one phase 6.4ml/100
#define PUMP_MAX_FLOW         0.62  // ml/sec (0.6216 ml/sec = 2.23 l/h)
#define PUMP_ACTUATION_PERIOD 75    // msec 36
#define PUMP_ACTUATION_HALF_PERIOD PUMP_ACTUATION_PERIOD/2     // msec

class Owen
{
public:
	Owen();

	//ENGINE TEMPERATURE
	void	filtrateTemp();
    void    resetTemp();
    float	readEngineTemp();
    float   currTempSpeed () const;
    float   currTemp      () const;

	//ENGINE SPEED
    void tick();
	void startEngine();
	void stopEngine();
    void upEngineSpeed(const int &dif);
    void downEngineSpeed(const int &dif);
    void setEngineSpeed(int pwm);
    unsigned char currentEngineSpeed() const;
    unsigned long calcPumpPeriod(int fanPWM = -1);

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

    char currentSpaceT() const;
    void setCurrentSpaceT(const char &currentSpaceT);

    int targetPWM() const;

    float currentFuelRate() const;

    bool pumpActuated() const;
    void setPumpActuated(bool newPumpActuated);

    unsigned char currentPWM() const;
    void setCurrentPWM(unsigned char newCurrentPWM);

private:
    bool m_active      = false;
    bool m_engine      = false;
    bool m_pump        = false;
    bool m_ignition    = false;

    bool m_pumpActuated = false;

    int m_ignitionTimer   = 0;
    int m_ignitionMaxTick = 600;

    unsigned char m_currentPWM = 0; //текущяя скорость
    unsigned char m_targetPWM  = 0; //целевая скорость

    float m_currentFuelRate = 0; //текущий расход
    unsigned long m_millis_pumpTimer = 0;
    unsigned long m_targetPumpPeriod = 0;

    float m_newTemp;
    float m_currTemp;
    float m_currTempSpeed = 0;

private:
    const unsigned char m_pwmResolution = 255;
    const unsigned char m_pwmStep       = 1;

    //Temperature filtrate constant's
    const float m_k             =  0.8;
    const float m_dt            =  0.1; //sec
    const float m_diffThreshold = 30.0; // deg

    char m_currentSpaceT = 0;

private:
};

#endif // OWEN_H


