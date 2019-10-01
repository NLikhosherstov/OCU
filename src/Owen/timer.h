#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

typedef uint64_t timestamp_t;

class Timer
{
public:
	Timer(){ this->reset(); }

	void reset  (void) { this->m_timestamp  = 0;   }
	void update (void) { this->m_timestamp += m_loopDuration;   }

    timestamp_t timeout(void) { return this->m_timestamp; }

	unsigned long loopDuration() const { return m_loopDuration; }
	void setLoopDuration(unsigned long loopDuration) { m_loopDuration = loopDuration; }

protected:
	timestamp_t	  m_timestamp = 0;
	unsigned long m_loopDuration = 100;
};

#endif // TIMER_H
