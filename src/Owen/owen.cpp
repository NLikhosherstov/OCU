#include "owen.h"

//Temperature sensor control pin's
#define DO  7 //SO
#define CS  8
#define CLK 9 //SCK
MAX6675 tSens(CLK, CS, DO);

Owen::Owen(){
    digitalWrite(LPWM, LOW);
    analogWrite(RPWM, LOW);

    pinMode(PUMP, OUTPUT); //подтягиваем к земле
    digitalWrite(PUMP, HIGH); //Высокий уровень, так как реле срабатывает по 0//твердотельное по 1

    pinMode(IGNT, OUTPUT);
    digitalWrite(IGNT, HIGH);

    m_currentSpaceT = 0;
    m_targetSpaceT = 0;

    m_currTemp = readEngineTemp();
}

double Owen::readEngineTemp(){
    m_newTemp = tSens.readCelsius();
    return m_newTemp;
}

double Owen::currTempSpeed() const{
	return m_currTempSpeed;
}

double Owen::currTemp() const{
    return m_currTemp;
}

void Owen::filtrateTemp(){
    if(utils::isNum(m_currTemp) == false)
    {
        m_currTemp = m_newTemp;
    }

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

void Owen::resetTemp(){
    if (utils::isNum(m_newTemp) == true){
        m_currTemp = m_newTemp;
        m_currTempSpeed = 0;
    }
}

void Owen::startEngine(){
    m_targetPWM = 150;
}

void Owen::stopEngine(){
    m_targetPWM = 0;
}

void Owen::upEngineSpeed(const int &dif){
    if(currentEngineSpeed() == 0 && dif < MIN_START_PWM){ //двигатель нормально стартует только с MIN_START_PWM
        setEngineSpeed(m_targetPWM + MIN_START_PWM);
    }else{
        setEngineSpeed(m_targetPWM + dif);
    }
}

void Owen::downEngineSpeed(const int &dif){
    setEngineSpeed(m_targetPWM - dif);
}

void Owen::setEngineSpeed(int pwm){
    if(pwm == m_targetPWM)
        return;

    if(pwm <= 0){
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
        m_pump = true;
        digitalWrite(PUMP, LOW);
        Serial.println(F("Pump started"));
	}
}

void Owen::stopPump(){
	if(m_pump == true) {
        m_pump = false;
        digitalWrite(PUMP, HIGH);
        Serial.println(F("Pump stopped"));
	}
}

void Owen::startIgnition(){
    if(m_ignition == false) {
        digitalWrite(IGNT, LOW);
		Serial.println(F("Ignition started"));
		m_ignition = true;
	}
}

void Owen::stopIgnition(){
    if(m_ignition == true) {
        digitalWrite(IGNT, HIGH);
		Serial.println(F("Ignition stoped"));
		m_ignition = false;
	}
}

bool Owen::active() const{
	return m_active;
}

void Owen::setActive(bool active){
    m_active = active;
    if(m_active)
        Serial.println(F("Owen active"));
    else
        Serial.println(F("Owen inactive"));
}

void Owen::checkIgnitionSafety(){
    if(ignition()){
        ++m_ignitionTimer;
        if(m_ignitionTimer >= m_ignitionMaxTick){
            m_ignitionTimer = 0;
            stopIgnition();
        }
    }
}

int8_t Owen::targetSpaceT() const{
    return m_targetSpaceT;
}

void Owen::setTargetSpaceT(const int8_t &targetSpaceT){
    m_targetSpaceT = targetSpaceT;
}

int16_t Owen::currentSpaceT() const{
    return m_currentSpaceT;
}

void Owen::setCurrentSpaceT(const int16_t &currentSpaceT){
    m_currentSpaceT = currentSpaceT;
}

int Owen::targetPWM() const{
    return m_targetPWM;
}

double Owen::currentFuelRate() const{
    return m_currentFuelRate;
}

bool Owen::ignition() const{
    return m_ignition;
}

bool Owen::pump() const{
    return m_pump;
}

void Owen::changeEngineSpeed()
{
    if(m_currentPWM == m_targetPWM)
        return;

    if(m_currentPWM < m_targetPWM){
        if(m_currentPWM == 0 && m_targetPWM >= 100) //меньший шим не даст
            m_currentPWM = m_targetPWM;             //запустить двигатель

        if( (m_currentPWM + m_pwmStep) < m_targetPWM )
            m_currentPWM += m_pwmStep;
        else
            m_currentPWM = m_targetPWM;

        analogWrite(RPWM, m_currentPWM);
    }else if(m_currentPWM > m_targetPWM){
        m_currentPWM = m_targetPWM;

        analogWrite(RPWM, m_currentPWM);
    }
    if(m_currentPWM > 0)
        m_targetPeriod = 1000/((PUMP_MAX_FLOW_SEC * map(currentEngineSpeed(), 0, 254, 0, 100)/100)/PUMP_SINGLE_ACTUATION);
    else
        m_targetPeriod = 0;

    Serial.println("Fuel period: " + String((int)m_targetPeriod));

    if(m_targetPeriod)
        m_currentFuelRate = ((PUMP_SINGLE_ACTUATION*(1000/m_targetPeriod))/1000)*60*60;
    else
        m_currentFuelRate = 0;
}

void Owen::pumpPulse(){
    if(m_targetPeriod && (millis() > m_targetPeriod) && millis()-m_targetPeriod > m_millis_pumpTimer){
        if(pump()){

        }else{

        }
        m_millis_pumpTimer = millis();
    }
}

int Owen::currentEngineSpeed() const{
	return m_currentPWM;
}
