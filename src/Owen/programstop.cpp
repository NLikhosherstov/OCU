#include "programstop.h"

void ProgramStop::execute(){
    if (state() == StandBy)
        set_state(Cooling);
}

void ProgramStop::update(Owen &owen){
    updateTimer();

    switch(state())
    {
        case StandBy    :                       break;
        case FuelSupply :                       break;
        case Ignition   :                       break;
        case WarmingUp  :                       break;
        case Cooling    : handle_Cooling(owen); break;
        case SilentCooling :                    break;
    }
}

void ProgramStop::handle_Cooling(Owen &owen){
    owen.stopPump();
    if(owen.currTemp() < 50.0){
        owen.stopEngine();
        set_state(StandBy);
    }
}
