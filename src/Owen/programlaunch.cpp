#include "programlaunch.h"

void ProgramLaunch::execute(){
    if (state() == StandBy)
        set_state(FuelSupply);
}

void ProgramLaunch::update(Owen & owen){
    updateTimer();

    switch(state())
	{
        case StandBy    : m_startTemp = INVALID_TEMP;    break;
        case FuelSupply : handle_state_FuelSupply(owen); break;
        case Ignition   : handle_state_Ignition(owen);   break;
        case WarmingUp  : handle_state_WarmingUp(owen);  break;
        case Cooling    :                                break;
        case SilentCooling :                             break;
    }
}

void ProgramLaunch::handle_state_FuelSupply(Owen & owen){
    owen.startEngine();

    if(m_startTemp == INVALID_TEMP)
        m_startTemp = owen.currTemp();

    if(owen.currTemp() < 100.0) //при бОльшей температуре
        owen.startPump();       //топливо может немного взорваться
    else
        return;

    if (timeout() > (5 * 1000)) //перед поджигом ждем около 5 сек
        set_state(Ignition);
}

void ProgramLaunch::handle_state_Ignition(Owen & owen){
	owen.startIgnition();
    if (timeout() > (15 * 1000)){ //Зажигаем свечу на 15 сек
		owen.stopIgnition();
        set_state(WarmingUp);
	}
}

void ProgramLaunch::handle_state_WarmingUp(Owen & owen){
    if ( (owen.currTemp() - m_startTemp) >= 30){ //Успешным поджиг считать
        set_state(StandBy);                      //если начальная температура была меньше
        owen.setActive(true);                    //текущей на 30 градусов
	}else{
        if (timeout() > (15 * 1000)){            //Если за 15 секунд температура не выросла
            set_state(Ignition);                 //на 30 градусов, то вернуться на пред. шаг
		}
	}
}
