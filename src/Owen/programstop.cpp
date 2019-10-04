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

void ProgramStop::stop(Owen &/*owen*/)
{

}

void ProgramStop::handle_Cooling(Owen &owen){
    owen.stopPump();
    if(owen.currTemp() < 50.0){
        owen.stopEngine();
        owen.setActive(false);
        set_state(StandBy);
    }
}
