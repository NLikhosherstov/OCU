#ifndef PROGRAMSTOP_H
#define PROGRAMSTOP_H

#include "programbase.h"

class ProgramStop : public ProgramBase
{
public:
    void execute(           );
    void stop   (Owen & owen);
    void update (Owen & owen);

private:
    void handle_Cooling(Owen &owen);
};

#endif // PROGRAMSTOP_H
