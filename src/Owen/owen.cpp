#include "owen.h"

int thermoDO = 7;  //он же SO
int thermoCS = 8;
int thermoCLK = 9;  //он же SCK
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Owen::Owen(){
	m_active   = false;
	m_engine   = false;
	m_pump     = false;
	m_ignition = false;

	digitalWrite(m_LPWM, LOW);
	analogWrite(m_RPWM, 0);

    m_currTemp      = thermocouple.readCelsius();
	m_currTempSpeed = 0;
}

double Owen::readEngineTemp(){
    m_newTemp = thermocouple.readCelsius();
    return m_newTemp;
}

double Owen::currTempSpeed(){
	return m_currTempSpeed;
}

double Owen::currTemp(){
	return m_currTemp;
}

void Owen::filtrateTemp()
{
	double k             =   0.8;
	double dt            =   0.1;
	double diffThreshold =  30.0; // deg

    if(utils::isNum(m_currTemp) == false)
        m_currTemp = m_newTemp;

	if (utils::isNum(m_newTemp) == true)
	{
		double predictedTemp = m_currTemp + m_currTempSpeed * dt;
		if (abs(m_newTemp - m_currTemp) < diffThreshold)
		{
			double currTemp = m_currTemp;
			m_currTemp      = (predictedTemp * k) + (m_newTemp * (1-k));
			m_currTempSpeed = (m_currTemp - currTemp) / dt;
		}
		else
		{
			m_currTemp = predictedTemp;
		}
    }
}

void Owen::startEngine(){
	setEngineSpeed(255);
}

void Owen::stopEngine(){
	setEngineSpeed(0);
}

void Owen::upEngineSpeed(const uint8_t &dif){
	if( (m_currentPWM + dif) < m_pwmResolution){
		setEngineSpeed(m_currentPWM + dif);
	}else{
		setEngineSpeed(m_pwmResolution - 1);
	}
}

void Owen::downEngineSpeed(const uint8_t &dif){
	if( (m_currentPWM - dif) >= 0){
		setEngineSpeed(m_currentPWM - dif);
	}else{
		setEngineSpeed(0);
	}
}

void Owen::setEngineSpeed(uint8_t pwm){
    if(pwm == m_currentPWM)
        return;

	m_currentPWM = pwm;
	m_engine = pwm > 0;
	analogWrite(m_RPWM, pwm);
}

void Owen::startPump(){
	if(m_pump == false) {
		Serial.println(F("Pump started"));
		m_pump = true;
	}
}

void Owen::stopPump(){
	if(m_pump == true) {
		Serial.println(F("Pump stopped"));
		m_pump = false;
	}
}

void Owen::startIgnition(){
	if(m_ignition == false) {
		Serial.println(F("Ignition started"));
		m_ignition = true;
	}
}

void Owen::stopIgnition(){
	if(m_ignition == true) {
		Serial.println(F("Ignition stoped"));
		m_ignition = false;
	}
}

bool Owen::active() const{
	return m_active;
}

void Owen::setActive(bool active){
	m_active = active;
}

int8_t Owen::currentEngineSpeed() const{
	return m_currentPWM;
}
