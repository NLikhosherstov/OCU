#include "owen.h"

//Temperature sensor control pin's
#define DO  7 //SO
#define CS  8
#define CLK 9 //SCK
MAX6675 tSens(CLK, CS, DO);

Owen::Owen(){
    digitalWrite(LPWM, LOW);
    analogWrite(RPWM, LOW);

    digitalWrite(PUMP,LOW);
    digitalWrite(IGNT,LOW);

    m_currTemp = readEngineTemp();
}

double Owen::readEngineTemp(){
    m_newTemp = tSens.readCelsius();
    return m_newTemp;
}

double Owen::currTempSpeed(){
	return m_currTempSpeed;
}

double Owen::currTemp(){
	return m_currTemp;
}

void Owen::filtrateTemp(){
    if(utils::isNum(m_currTemp) == false)
        m_currTemp = m_newTemp;

	if (utils::isNum(m_newTemp) == true)
	{
        double predictedTemp = m_currTemp + m_currTempSpeed * m_dt;
        if (abs(m_newTemp - m_currTemp) < m_diffThreshold)
		{
			double currTemp = m_currTemp;
            m_currTemp      = (predictedTemp * m_k) + (m_newTemp * (1-m_k));
            m_currTempSpeed = (m_currTemp - currTemp) / m_dt;
		}
		else
		{
			m_currTemp = predictedTemp;
		}
    }
}

void Owen::startEngine(){
    m_targetPWM = 200;
}

void Owen::stopEngine(){
    m_targetPWM = 0;
}

void Owen::upEngineSpeed(const uint8_t &dif){
    setEngineSpeed(m_targetPWM + dif);
}

void Owen::downEngineSpeed(const uint8_t &dif){
    setEngineSpeed(m_targetPWM - dif);
}

void Owen::setEngineSpeed(uint8_t pwm){
    if(pwm == m_targetPWM)
        return;

    if(pwm < 0){
        m_targetPWM = 0;
    }else if(pwm >= m_pwmResolution){
        m_targetPWM = m_pwmResolution-1;
    }else{
        m_targetPWM = pwm;
    }

    m_engine = currentEngineSpeed();
}

void Owen::startPump(){
	if(m_pump == false) {
        digitalWrite(PUMP, HIGH);
        Serial.println(F("Pump started"));
		m_pump = true;
	}
}

void Owen::stopPump(){
	if(m_pump == true) {
        digitalWrite(PUMP, LOW);
        Serial.println(F("Pump stopped"));
		m_pump = false;
	}
}

void Owen::startIgnition(){
	if(m_ignition == false) {
        digitalWrite(IGNT, HIGH);
		Serial.println(F("Ignition started"));
		m_ignition = true;
	}
}

void Owen::stopIgnition(){
	if(m_ignition == true) {
        digitalWrite(IGNT, LOW);
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

void Owen::changeEngineSpeed()
{
    if(m_currentPWM < m_targetPWM){
        if( (m_currentPWM + m_pwmStep) < m_targetPWM )
            m_currentPWM += m_pwmStep;
        else
            m_currentPWM = m_targetPWM;
        analogWrite(RPWM, m_currentPWM);
    }else if(m_currentPWM > m_targetPWM){
        m_currentPWM = m_targetPWM;
        analogWrite(RPWM, m_currentPWM);
    }
}

int8_t Owen::currentEngineSpeed() const{
	return m_currentPWM;
}
