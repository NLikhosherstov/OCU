#ifndef PROGRAMTHERMOSTAT_H
#define PROGRAMTHERMOSTAT_H

#include "programbase.h"

class programThermostat : public ProgramBase
{
public:
    void execute();
    void stop   ();
    void update (Owen & owen);

private:
    void warming(Owen &owen);
};

#endif // PROGRAMTHERMOSTAT_H
