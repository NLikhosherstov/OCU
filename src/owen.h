#ifndef OWEN_H
#define OWEN_H

#include <Arduino.h>

class Owen
{
public:
	Owen();

	int16_t readEngineTemp();
	int8_t  currTempSpeed ();

public:
	bool m_engine;
	bool m_pump;
	bool m_ignition;

	int16_t m_currTemp;
	int8_t m_currTempSpeed;
};

class Timer
{
public:
	typedef uint64_t timestamp_t;
	Timer(){ this->reset(); }

	void reset  (void) { this->m_timestamp  = 0;   }
	void update (void) { this->m_timestamp += 1;   }
	timestamp_t timeout(void) { return this->m_timestamp; }

protected:
	timestamp_t m_timestamp = 0;
};

///
class ProgramLaunch
{
public:
	enum State{
		StandBy,
		State_1,
		State_2,
		State_3
	};

public:
	void execute();
	void update(Owen & owen);

protected:
	Timer			m_timer;
	State m_state = StandBy;

protected:
	void set_state(ProgramLaunch::State state);

	void handle_standby(void	   );
	void handle_state_1(Owen & owen);
	void handle_state_2(Owen & owen);
	void handle_state_3(Owen & owen);
};


#endif // OWEN_H


