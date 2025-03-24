#include "programlaunch2.h"

void ProgramLaunch2::execute(){
    m_stopped = false;
    if (state() == StandBy)
        set_state(Cooling);
}

void ProgramLaunch2::stop(){
    m_stopped = true;
    set_state(StandBy);
}

void ProgramLaunch2::update(Owen &owen){
    updateTimer();

    switch(state())
    {
        case StandBy       : standBy(owen);         break;
        case FuelSupply    : fuelSupply(owen);      break;
        case Ignition      : ignition(owen);        break;
        case WarmingUp     : warmingUp(owen);       break;
        case Cooling       : startEngine(owen);     break;
        case SilentCooling :                        break;
        case Thermostating : thermostating(owen);   break;
    }
}

void ProgramLaunch2::thermostating(Owen &owen){
    if(owen.automatic() && !m_stopped){
        if(spaceT() >= Cfg().constTemp){
            if(owen.currentEngineSpeed() != Cfg().speed2){
                owen.setEngineSpeed(Cfg().speed2);
            }
        }else if(spaceT() <= Cfg().constTemp-2){
            if(owen.currentEngineSpeed() != Cfg().speed3){
                owen.setEngineSpeed(Cfg().speed3);
            }
        }
    }
}

void ProgramLaunch2::standBy(Owen &owen){
    if(owen.automatic() && !m_stopped){
        if(owen.currTemp() == 0){
            if(timeout() > ((uint64_t)60 * (uint64_t)1000)){ //через 1м00сек на максимальную мощность
                owen.setEngineSpeed(Cfg().speed4);
                if(spaceT() >= Cfg().constTemp)
                    set_state(Thermostating);
            }
        }else if(owen.currTemp() >= 45){
            owen.setEngineSpeed(Cfg().speed4);
            if(spaceT() >= Cfg().constTemp)
                set_state(Thermostating);
        }else if(timeout() > ((uint64_t)90 * (uint64_t)1000)){
            owen.calcPumpPeriod(Cfg().speed3);
        }
    }
}

void ProgramLaunch2::startEngine(Owen &owen){
    owen.startEngine();
    owen.setAutomatic(true);
    if (timeout() > (3000)){ //ждем 3 сек на разгон двигателя
        owen.startIgnition();
        set_state(Ignition);
    }
}

void ProgramLaunch2::ignition(Owen &owen){
    if (timeout() > (5000)){     //перед подачей топлива ждем около 5 сек для разогрева свечи
        owen.startPump();
        owen.calcPumpPeriod(Cfg().speed2);
        set_state(FuelSupply);
    }
}

void ProgramLaunch2::fuelSupply(Owen &owen){
    if (timeout() > (30000)){ //ждем 30 сек на поджиг
        owen.stopIgnition();
        owen.calcPumpPeriod(Cfg().speed1);
        set_state(WarmingUp);
    }
}

void ProgramLaunch2::warmingUp(Owen &owen){
    if (timeout() > ((uint64_t)40 * (uint64_t)1000)){ //через 40 сек увеличиваем скорость
        owen.setEngineSpeed(Cfg().speed4);            //до максимума
        owen.calcPumpPeriod(Cfg().speed1); //топлива не доливаем
        set_state(StandBy);
    }
}

bool ProgramLaunch2::isStopped() const{
    return m_stopped;
}
