#include "programlaunch.h"

void ProgramLaunch::execute(){
    if (state() == StandBy)
        set_state(FuelSupply);
}

void ProgramLaunch::stop()
{
    set_state(StandBy);
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

    if(owen.currTemp() < 100.0){ //при бОльшей температуре
        owen.startPump();        //топливо может немного взорваться
    }else{
        set_state(StandBy);
        return;
    }

    if (timeout() > (15 * 1000)) //перед поджигом ждем около 15 сек
        set_state(Ignition);
}

void ProgramLaunch::handle_state_Ignition(Owen & owen){
    if((owen.currTemp() - m_startTemp) >= 5){ //если начальная температура была меньше
        owen.stopIgnition();                  //текущей на 5 градусов
        set_state(StandBy);                   //то поджиг считаем успешным
        owen.setEngineSpeed(150);
        owen.setActive(true);
        return;
    }else{
        owen.startIgnition();
        if (timeout() > ((uint64_t)10 * (uint64_t)1000)){ //через 10 секунд увеличиваем обороты
            owen.setEngineSpeed(240);
        }
    }

    if (timeout() > ((uint64_t)90 * (uint64_t)1000)){  //если поджига не случилось за 1.5м
        owen.stopPump();                                //выключаем помпу
        owen.stopIgnition();                            //и свечу
        set_state(StandBy);                             //и ждем дальнейших указаний
    }
}

void ProgramLaunch::handle_state_WarmingUp(Owen & owen){
    if ( (owen.currTemp() - m_startTemp) >= 10){ //Успешным поджиг считать
        set_state(StandBy);                      //если начальная температура была меньше
        owen.setActive(true);                    //текущей на 30 градусов
        m_startTemp = INVALID_TEMP;              //сбрасываем стартовую температуру
	}else{
        if (timeout() > (15 * 1000)){            //Если за 15 секунд температура не выросла
            set_state(Ignition);                 //на 30 градусов, то вернуться на пред. шаг
		}
	}
}
