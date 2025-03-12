#ifndef MONITOR_H
#define MONITOR_H

#include <SPI.h>
#define USE_MICRO_WIRE
#include <GyverOLED.h>

#include "Owen/owen.h"

class Monitor
{
public:
	Monitor();
	void start();

    void updateData(const Owen &owen);
    void showError(const String &str);

    void setTitleText(const String &str);

    bool menuMode() const;
    void setMenuMode(bool newMenuMode);

    void upItem();
    void downItem();

    void increaseValue(Owen &owen, bool fast = false);
    void decreaseValue(Owen &owen, bool fast = false);

    bool settingsChanged() const;
    void setSettingsChanged(bool newSettingsChanged);

    void setBtnConfigMode(bool newBtnConfigMode);

    bool btnConfigMode() const;

    bool constTempMode() const;
    void setConstTempMode(bool newConstTempMode);

    void restartTempCounter();

private:
    bool m_menuMode = false;
    bool m_btnConfigMode = false;
    bool m_settingsChanged = false;
    bool m_constTempMode = false;
    char m_currentItem = 0;
    char m_itemsCount = 12;

private:
    void showTempConstData();
    void showBtnConfigData();
    void showOwenData(const Owen &owen);
    void showMenuData();
};

#endif // MONITOR_H
