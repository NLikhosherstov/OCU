#include "programthermostat.h"

void programThermostat::execute(){
    if (state() == StandBy)
        set_state(Cooling);
}

void programThermostat::stop(){
    set_state(StandBy);
}

void programThermostat::update(Owen &owen){
    updateTimer();

    switch(state())
    {
        case StandBy    :                break;
        case FuelSupply :                break;
        case Ignition   :                break;
        case WarmingUp  : warming(owen); break;
        case Cooling    :                break;
        case SilentCooling :             break;
    }
}

void programThermostat::warming(Owen &owen){

}
