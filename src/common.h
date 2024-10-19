#ifndef COMMON_H
#define COMMON_H

#include <math.h>
#include <Arduino.h>
#include <EEPROM.h>

namespace utils
{
	///
	template<typename Value>
	inline bool isNum(const Value & value)
	{
		return (isnan(value) == false) && (isinf(value) == false);
	}

    #define DATA_FLAG 0xAA
    struct Settings{
        int     correction = 0;          //Общая коррекция расхода топлива
        double  owenMaxFlow = 1.75;      //Максимальный расход печки
        double  pumpMaxPerfomance = 6.4; //Призводительность насоса
        bool    embededPump = false;     //Встроенный насос
    };
    static Settings deviceSettings;

    inline void loadSettings(){
        if (EEPROM.read(0x0) == DATA_FLAG) {    // Проверяем наличие маркера
            EEPROM.get(0x1, deviceSettings);    // Если маркер есть, загружаем настройки из EEPROM
        } else {
            EEPROM.write(0x0, DATA_FLAG);       // Записываем маркер, указывающий наличие данных
            EEPROM.put(0x1, deviceSettings);    // Сохраняем данные в EEPROM, начиная с адреса 0x1
            Serial.println(F("Writing EEPROM set default settings"));
        }
    }

    inline void saveSettings(){
        EEPROM.write(0x0, DATA_FLAG);     // Записываем маркер, указывающий наличие данных
        EEPROM.put(0x1, deviceSettings);  // Сохраняем данные в EEPROM, начиная с адреса 0x1

        Serial.println(F("Writing EEPROM new settings"));
    }
}

#endif // COMMON_H
