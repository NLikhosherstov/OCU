#include "programlaunch2.h"

void ProgramLaunch2::execute(){
    if (state() == StandBy)
        set_state(Cooling);
}

void ProgramLaunch2::stop(){
    set_state(StandBy);
}

void ProgramLaunch2::update(Owen &owen){
    updateTimer();

    switch(state())
    {
        case StandBy       : standBy(owen);     break;
        case FuelSupply    : fuelSupply(owen);  break;
        case Ignition      : ignition(owen);    break;
        case WarmingUp     : warmingUp(owen);   break;
        case Cooling       : startEngine(owen); break;
        case SilentCooling :                    break;
    }
}

void ProgramLaunch2::standBy(Owen &owen){
    if(!m_blockPwmControl){
        if(owen.currentSpaceT() >= 25){
            if(owen.currentEngineSpeed() == 255){
                owen.setEngineSpeed(255/2);
                m_blockPwmControl = true;
            }
        }
    }
}

void ProgramLaunch2::startEngine(Owen &owen){
    owen.startEngine();
    if (timeout() > (5000)) //ждем 5 сек на разгон двигателя
        set_state(Ignition);
}

void ProgramLaunch2::ignition(Owen &owen){
    owen.startIgnition();
    if (timeout() > (7000)) //перед подачей топлива ждем около 7 сек
        set_state(FuelSupply);  //для разогрева свечи
}

void ProgramLaunch2::fuelSupply(Owen &owen){
    owen.startPump();
    if (timeout() > (30000)){ //ждем 30 сек на поджиг
        owen.stopIgnition();
        owen.setActive(true);
        owen.upEngineSpeed(28);
        set_state(WarmingUp);
    }
}

void ProgramLaunch2::warmingUp(Owen &owen){
    if (timeout() > ((uint64_t)60 * (uint64_t)1000)){ //через 60 сек увеличиваем скорость
        owen.setEngineSpeed(255);                     //до максимума
        set_state(StandBy);
    }
}
