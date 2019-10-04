#ifndef PROGRAMLAUNCH_H
#define PROGRAMLAUNCH_H

#include "programbase.h"

#define INVALID_TEMP 1000.0

class ProgramLaunch : public ProgramBase
{
public:
    void execute(           );
    void stop   (Owen & owen);
    void update (Owen & owen);

private:
    double m_startTemp = INVALID_TEMP;

private:
    void handle_state_FuelSupply(Owen & owen);
    void handle_state_Ignition(Owen & owen);
    void handle_state_WarmingUp(Owen & owen);
};

#endif // PROGRAMLAUNCH_H
