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
    digitalWrite(PUMP, LOW); //Высокий уровень, так как реле срабатывает по 0//твердотельное по 1

    pinMode(IGNT, OUTPUT);
    digitalWrite(IGNT, LOW);

    m_currTemp = readEngineTemp();
}

float Owen::readEngineTemp(){
    m_newTemp = tSens.readCelsius();
    return m_newTemp;
}

float Owen::currTempSpeed() const{
	return m_currTempSpeed;
}

float Owen::currTemp() const{
    return m_currTemp;
}

void Owen::filtrateTemp(){
    if(utils::isNum(m_currTemp) == false)
    {
        m_currTemp = m_newTemp;
    }

	if (utils::isNum(m_newTemp) == true)
	{
        float predictedTemp = m_currTemp + m_currTempSpeed * m_dt;
        if (abs(m_newTemp - m_currTemp) < m_diffThreshold)
		{
            float currTemp = m_currTemp;
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
    setEngineSpeed(Cfg().speed1);
}

void Owen::stopEngine(){
    setEngineSpeed(0);
}

void Owen::upEngineSpeed(const int &dif){
    if(Cfg().embededPump){
        if(currentEngineSpeed() == 0 && dif < MIN_START_PWM){ //двигатель нормально стартует только с MIN_START_PWM
            setEngineSpeed(m_targetPWM + MIN_START_PWM);
        }else{
            setEngineSpeed(m_targetPWM + dif);
        }
    }else{
        if(m_targetPWM < Cfg().speed1)
            setEngineSpeed(Cfg().speed1);
        else if(m_targetPWM < Cfg().speed2)
            setEngineSpeed(Cfg().speed2);
        else if(m_targetPWM < Cfg().speed3)
            setEngineSpeed(Cfg().speed3);
        else if(m_targetPWM < Cfg().speed4)
            setEngineSpeed(Cfg().speed4);
    }
}

void Owen::downEngineSpeed(const int &dif){
    if(Cfg().embededPump){
        setEngineSpeed(m_targetPWM - dif);
    }else{
        if(m_targetPWM >= Cfg().speed4)
            setEngineSpeed(Cfg().speed3);
        else if(m_targetPWM >= Cfg().speed3)
            setEngineSpeed(Cfg().speed2);
        else if(m_targetPWM >= Cfg().speed2)
            setEngineSpeed(Cfg().speed1);
        else if(m_targetPWM >= Cfg().speed1){
            if(pump() && (currTemp() < 60)){
                stopPump();
                setEngineSpeed(0);
            }else if(!pump()){
                setEngineSpeed(0);
            }else{
                return;
            }
        }
    }
}

void Owen::setEngineSpeed(int pwm){
    if(pwm != m_targetPWM){
        if(pwm <= 0){
            m_targetPWM = 0;
        }else if(pwm >= m_pwmResolution){
            m_targetPWM = m_pwmResolution-1;
        }else{
            m_targetPWM = pwm;
        }
    }

    m_engine = currentEngineSpeed();
    calcPumpPeriod(m_targetPWM);
}

void Owen::startPump(){
    if(m_pump == false) {
        m_pump = true;
        if(Cfg().embededPump){
            digitalWrite(PUMP, HIGH);
        }else{
            if(m_currentPWM == 0)
                calcPumpPeriod(Cfg().speed4);
        }
        Serial.println(F("Pump started "));
	}
}

void Owen::stopPump(){
	if(m_pump == true) {
        m_pump = false;
        digitalWrite(PUMP, LOW);
        if(m_currentPWM == 0)
            calcPumpPeriod(0);
        Serial.println(F("Pump stopped"));
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

bool Owen::automatic() const{
    return m_avtomatic;
}

void Owen::setAutomatic(bool newAutomatic){
    m_avtomatic = newAutomatic;
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

int Owen::targetPWM() const{
    return m_targetPWM;
}

float Owen::currentFuelRate() const{
    return m_currentFuelRate;
}

bool Owen::pumpActuated() const{
    return m_pumpActuated;
}

void Owen::setPumpActuated(bool newPumpActuated){
    m_pumpActuated = newPumpActuated;
}

unsigned char Owen::currentPWM() const{
    return m_currentPWM;
}

void Owen::setCurrentPWM(unsigned char newCurrentPWM){
    m_currentPWM = newCurrentPWM;
}

short Owen::targetPumpPeriod() const{
    return m_targetPumpPeriod;
}

void Owen::setTargetPumpPeriod(short newTargetPumpPeriod, bool corr){
    m_targetPumpPeriod = newTargetPumpPeriod;
    if(m_targetPumpPeriod > 0)
        m_targetPumpPeriod = max(0, newTargetPumpPeriod - (corr ? Cfg().correction : 0));
    m_currentFuelRate = utils::fuelRate(Cfg().pumpPerfomance, m_targetPumpPeriod);
}

unsigned long Owen::calcPumpPeriod(int fanPWM){
    if(Cfg().embededPump){
        if(fanPWM > 0)
            m_targetPumpPeriod = (1000/((Cfg().owenMaxFlow * map(fanPWM, 0, 254, 0, 100)/100)/Cfg().pumpPerfomance)) + Cfg().correction;
        else if(fanPWM == -1)
            m_targetPumpPeriod = (1000/((Cfg().owenMaxFlow * map(m_currentPWM, 0, 254, 0, 100)/100)/Cfg().pumpPerfomance)) + Cfg().correction;
        else
            m_targetPumpPeriod = 0;
    }else{
        int pwm = m_targetPWM;
        if(fanPWM > 0) pwm = fanPWM;
        if(pwm < Cfg().speed1)
            m_targetPumpPeriod = 0;
        else if(pwm < Cfg().speed2)
            m_targetPumpPeriod = Cfg().consumption1;
        else if(pwm < Cfg().speed3)
            m_targetPumpPeriod = Cfg().consumption2;
        else if(pwm < Cfg().speed4)
            m_targetPumpPeriod = Cfg().consumption3;
        else if(pwm >= Cfg().speed4)
            m_targetPumpPeriod = Cfg().consumption4;

        if(m_targetPumpPeriod > 0)
            m_targetPumpPeriod = max(0, m_targetPumpPeriod - Cfg().correction);
    }

    m_currentFuelRate = utils::fuelRate(Cfg().pumpPerfomance, m_targetPumpPeriod);

    return m_targetPumpPeriod;
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
        if( (m_currentPWM - m_pwmStep) > m_targetPWM )
            m_currentPWM -= m_pwmStep;
        else
            m_currentPWM = m_targetPWM;

        analogWrite(RPWM, m_currentPWM);
    }
}

void Owen::pumpPulse(){
    static unsigned int counter = 0;
    if(Cfg().embededPump) return;

    if(pump() && m_targetPumpPeriod && counter <= m_targetPumpPeriod){
        if(counter < PUMP_ACTUATION_HALF_PERIOD){
            if(!pumpActuated()){
                digitalWrite(PUMP, HIGH);
                setPumpActuated(true);
            }
        }else{
            if(pumpActuated()){
                digitalWrite(PUMP, LOW);
                setPumpActuated(false);
            }
        }
        counter++;
    }else{
        counter = 0;
    }
    if(!pump() && pumpActuated()){
        digitalWrite(PUMP, LOW);
        setPumpActuated(false);
    }
}

unsigned char Owen::currentEngineSpeed() const{
	return m_currentPWM;
}
