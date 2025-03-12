#ifndef COMMON_H
#define COMMON_H

#include <math.h>
#include <Arduino.h>
#include <EEPROM.h>

#define MIN_START_PWM 153

namespace utils
{
	///
	template<typename Value>
	inline bool isNum(const Value & value)
	{
		return (isnan(value) == false) && (isinf(value) == false);
	}

    inline float fuelRate(const double &pumpPerfomance, const short &period){
        float rate = 0;
        if(period > 0)
            rate = ((pumpPerfomance*(1000.0/period))/1000.0)*3600;

        return rate;
    }
}

namespace Config{
    struct ConfigStruct{
        short  correction = 0;          //Общая коррекция расхода топлива
        double owenMaxFlow = 0.42;      //Максимальный расход печки ml/sec (0.42 ml/sec = 1.51 l/h)
        double pumpPerfomance = 0.064;  //Призводительность насоса ml per one phase 6.4ml/100
        bool   embededPump = false;     //Встроенный насос
        short  speed1 = MIN_START_PWM;  //1я скорость PWM-255
        short  speed2 = 180;            //2я скорость PWM-255
        short  speed3 = 216;            //3я скорость PWM-255
        short  speed4 = 254;            //4я скорость PWM-255
        short  consumption1 = 328;      //Интервал работы насоса на 1й скорости в мс
        short  consumption2 = 252;      //Интервал работы насоса на 2й скорости в мс
        short  consumption3 = 190;      //Интервал работы насоса на 3й скорости в мс
        short  consumption4 = 153;      //Интервал работы насоса на 4й скорости в мс

        int btnPower    = 0;
        int btnUp       = 0;
        int btnIgnition = 0;
        int btnPump     = 0;
        int btnDown     = 0;

        short constTemp = 23;
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
        char m_currentSpaceT = 0;
        short m_currentBtnSetting = -1;

      private:
        Singleton() { }
        ~Singleton() { }

        Singleton(Singleton const&);
        Singleton& operator= (Singleton const&);
    };
}

#define DATA_FLAG 0xAC
inline Config::ConfigStruct& Cfg(){ return Config::Singleton::inst().data; }
inline short& btnSettingNumber(){ return Config::Singleton::inst().m_currentBtnSetting; }
inline char& spaceT(){ return Config::Singleton::inst().m_currentSpaceT; }
inline void loadConfig(){
    if (EEPROM.read(0x0) == DATA_FLAG) {    // Проверяем наличие маркера
        EEPROM.get(0x1, Cfg());    // Если маркер есть, загружаем настройки из EEPROM
//        Serial.println(F("Load EEPROM settings"));
//        Serial.print(F("Correction: ")); Serial.println(Cfg().correction);
//        Serial.print(F("OwenMaxFlow: ")); Serial.println(Cfg().owenMaxFlow);
//        Serial.print(F("PumpMaxPerfomance: ")); Serial.println(Cfg().pumpPerfomance);
//        Serial.print(F("EmbededPump: ")); Serial.println(Cfg().embededPump);
//        Serial.print(F("Speed1: ")); Serial.println(Cfg().speed1);
//        Serial.print(F("Speed2: ")); Serial.println(Cfg().speed2);
//        Serial.print(F("Speed3: ")); Serial.println(Cfg().speed3);
//        Serial.print(F("Speed4: ")); Serial.println(Cfg().speed4);
//        Serial.print(F("Consumption1: ")); Serial.println(Cfg().consumption1);
//        Serial.print(F("Consumption2: ")); Serial.println(Cfg().consumption2);
//        Serial.print(F("Consumption3: ")); Serial.println(Cfg().consumption3);
//        Serial.print(F("Consumption4: ")); Serial.println(Cfg().consumption4);
    } else {
        EEPROM.write(0x0, DATA_FLAG);       // Записываем маркер, указывающий наличие данных
        EEPROM.put(0x1, Cfg());    // Сохраняем данные в EEPROM, начиная с адреса 0x1
        Serial.println(F("Writing EEPROM set default settings"));
    }
}

inline void saveConfig(){
    EEPROM.write(0x0, DATA_FLAG);     // Записываем маркер, указывающий наличие данных
    EEPROM.put(0x1, Cfg());  // Сохраняем данные в EEPROM, начиная с адреса 0x1

    Serial.println(F("Writing EEPROM new settings"));
//    Serial.print(F("Correction: ")); Serial.println(Cfg().correction);
//    Serial.print(F("OwenMaxFlow: ")); Serial.println(Cfg().owenMaxFlow);
//    Serial.print(F("PumpMaxPerfomance: ")); Serial.println(Cfg().pumpPerfomance);
//    Serial.print(F("EmbededPump: ")); Serial.println(Cfg().embededPump);
//    Serial.print(F("Speed1: ")); Serial.println(Cfg().speed1);
//    Serial.print(F("Speed2: ")); Serial.println(Cfg().speed2);
//    Serial.print(F("Speed3: ")); Serial.println(Cfg().speed3);
//    Serial.print(F("Speed4: ")); Serial.println(Cfg().speed4);
//    Serial.print(F("Consumption1: ")); Serial.println(Cfg().consumption1);
//    Serial.print(F("Consumption2: ")); Serial.println(Cfg().consumption2);
//    Serial.print(F("Consumption3: ")); Serial.println(Cfg().consumption3);
//    Serial.print(F("Consumption4: ")); Serial.println(Cfg().consumption4);
}

#endif // COMMON_H
