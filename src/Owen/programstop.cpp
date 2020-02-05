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

void ProgramStop::stop()
{
    set_state(StandBy);
}

void ProgramStop::handle_Cooling(Owen &owen){
    owen.stopPump();
    owen.stopIgnition();

    if (timeout() < (3 * 1000)){ //через 3 секунды можно регулировать вручную
        owen.setEngineSpeed(255);//макс скорость двигателя
    }

    if(owen.currTemp() < 40.0){
        owen.stopEngine();
        owen.setActive(false);
        set_state(StandBy);
    }
}
