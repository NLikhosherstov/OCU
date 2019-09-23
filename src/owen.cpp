#include "owen.h"

Owen::Owen(){
	m_engine = false;
	m_pump = false;
	m_ignition = false;

	m_currTemp = readEngineTemp();
	m_currTempSpeed = 0;
}

int16_t Owen::readEngineTemp(){
	return 0;
}

int8_t Owen::currTempSpeed(){
	return 0;
}

void ProgramLaunch::execute(){
	this->m_state = ProgramLaunch::State::State_1;
}

void ProgramLaunch::update(Owen & owen){
	this->m_timer.update();
	switch(this->m_state)
	{
		case StandBy :                             break;
		case State_1 : this->handle_state_1(owen); break;
		case State_2 : this->handle_state_2(owen); break;
		case State_3 : this->handle_state_3(owen); break;
	}
}

void ProgramLaunch::set_state(ProgramLaunch::State state){
	this->m_timer.reset();
	this->m_state = state;
}

void ProgramLaunch::handle_state_1(Owen & owen){
	owen.m_engine = true;
	owen.m_pump   = true;
	if (this->m_timer.timeout() > (5 * 1000))
	{
		this->set_state(ProgramLaunch::State::State_2);
	}
}

void ProgramLaunch::handle_state_2(Owen & owen){
	owen.m_ignition = true;
	if (this->m_timer.timeout() > (15 * 1000))
	{
		this->set_state(ProgramLaunch::State::State_3);
	}
}

void ProgramLaunch::handle_state_3(Owen & owen){
	owen.m_ignition = false;
	if (owen.currTempSpeed() > 2)
	{
		this->set_state(ProgramLaunch::State::StandBy);
	}
	else
	{
		if (this->m_timer.timeout() > (15 * 1000))
		{
			this->set_state(ProgramLaunch::State::State_2);
		}
	}
}
