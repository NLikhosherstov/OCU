#ifndef PROGRAMLAUNCH2_H
#define PROGRAMLAUNCH2_H

#include "programbase.h"

class ProgramLaunch2 : public ProgramBase
{
public:
    void execute();
    void stop   ();
    void update (Owen & owen);

    bool isStopped() const;

private:
    void standBy(Owen & owen);
    void startEngine(Owen & owen);
    void ignition(Owen & owen);
    void fuelSupply(Owen & owen);
    void warmingUp(Owen & owen);

private:
    bool m_blockPwmControl = false;
    bool m_fullPower = false;
    bool m_stopped = true;
};

#endif // PROGRAMLAUNCH2_H
