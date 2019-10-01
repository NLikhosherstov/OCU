#ifndef PROGRAMBASE_H
#define PROGRAMBASE_H

#include "owen.h"
#include "timer.h"

class ProgramBase
{
public:
    enum State : uint8_t{
        StandBy,        //Режим простоя
        FuelSupply,     //Подача топлива
        Ignition,       //Зажигание
        WarmingUp,      //Нагрев
        Cooling,        //Продувка
        SilentCooling   //Продувка на малых оборотах
    };

public:
    virtual void execute() {}
    virtual void update (Owen & /*owen*/) {}

    State		state()		  { return m_state;			  }
    timestamp_t timeout(void) { return m_timer.timeout(); }
    void        updateTimer() { return m_timer.update();  }
    void        resetTimer()  { return m_timer.reset();   }

protected:
    void set_state(State state){
        m_timer.reset();
        m_state = state;
    }

private:
    Timer m_timer;
    State m_state = StandBy;
};

#endif // PROGRAMBASE_H
