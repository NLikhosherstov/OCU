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



//    Settings &Cfg().data{
//        static Settings s_deviceSettings;
//        return s_deviceSettings;
//    }
}

namespace Config{
    struct ConfigStruct{
        int     correction = 0;          //Общая коррекция расхода топлива
        double  owenMaxFlow = 1.75;      //Максимальный расход печки
        double  pumpMaxPerfomance = 6.4; //Призводительность насоса
        bool    embededPump = false;     //Встроенный насос
    };

    class Singleton
    {
      public:
        static Singleton& inst()
        {
            static Singleton s;
            return s;
        }

        ConfigStruct data = ConfigStruct();

      private:
        Singleton() { }
        ~Singleton() { }

        Singleton(Singleton const&);
        Singleton& operator= (Singleton const&);
    };
}

#define DATA_FLAG 0xAC
inline Config::Singleton& Cfg(){ return Config::Singleton::inst(); }
inline void loadConfig(){
    if (EEPROM.read(0x0) == DATA_FLAG) {    // Проверяем наличие маркера
        EEPROM.get(0x1, Cfg().data);    // Если маркер есть, загружаем настройки из EEPROM
        Serial.println(F("Load EEPROM settings"));
        Serial.print(F("Correction:")); Serial.println(Cfg().data.correction);
        Serial.print(F("OwenMaxFlow:")); Serial.println(Cfg().data.owenMaxFlow);
        Serial.print(F("PumpMaxPerfomance:")); Serial.println(Cfg().data.pumpMaxPerfomance);
        Serial.print(F("EmbededPump:")); Serial.println(Cfg().data.embededPump);
    } else {
        EEPROM.write(0x0, DATA_FLAG);       // Записываем маркер, указывающий наличие данных
        EEPROM.put(0x1, Cfg().data);    // Сохраняем данные в EEPROM, начиная с адреса 0x1
        Serial.println(F("Writing EEPROM set default settings"));
    }
}

inline void saveConfig(){
    EEPROM.write(0x0, DATA_FLAG);     // Записываем маркер, указывающий наличие данных
    EEPROM.put(0x1, Cfg().data);  // Сохраняем данные в EEPROM, начиная с адреса 0x1

    Serial.println(F("Writing EEPROM new settings"));
    Serial.print(F("Correction:")); Serial.println(Cfg().data.correction);
    Serial.print(F("OwenMaxFlow:")); Serial.println(Cfg().data.owenMaxFlow);
    Serial.print(F("PumpMaxPerfomance:")); Serial.println(Cfg().data.pumpMaxPerfomance);
    Serial.print(F("EmbededPump:")); Serial.println(Cfg().data.embededPump);
}

#endif // COMMON_H
