win32 {
    HOMEDIR += $$(USERPROFILE)
}
else {
    HOMEDIR += $$(HOME)
}

INCLUDEPATH += "$${HOMEDIR}/Projects/Arduino/OCU/include"
INCLUDEPATH += "$${HOMEDIR}/Projects/Arduino/OCU/src"
INCLUDEPATH += "$${HOMEDIR}/Projects/Arduino/OCU/.pio/libdeps/nanoatmega328/Adafruit SSD1306_ID135"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/Wire/src"
INCLUDEPATH += "$${HOMEDIR}/Projects/Arduino/OCU/.pio/libdeps/nanoatmega328/Adafruit GFX Library_ID13"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/SPI/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoavr/cores/arduino"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoavr/variants/eightanaloginputs"
INCLUDEPATH += "$${HOMEDIR}/Projects/Arduino/OCU/.pio/libdeps/nanoatmega328/MAX6675 library_ID558"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/EEPROM/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/HID/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/SoftwareSerial/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-atmelavr/avr/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-atmelavr/lib/gcc/avr/5.4.0/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-atmelavr/lib/gcc/avr/5.4.0/include-fixed"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/tool-unity"

DEFINES += "PLATFORMIO=40100"
DEFINES += "ARDUINO_AVR_NANO"
DEFINES += "F_CPU=16000000L"
DEFINES += "ARDUINO_ARCH_AVR"
DEFINES += "ARDUINO=10805"
DEFINES += "__AVR_ATmega328P__"

OTHER_FILES += platformio.ini

SOURCES += src/main.cpp \
           src/monitor.cpp \
           src/buttons.cpp \
           src/Owen/owen.cpp \
           src/Owen/programlaunch.cpp \
    src/Owen/programstop.cpp \
    src/Owen/programlaunch2.cpp

HEADERS += src/main.h \
           src/monitor.h \
           src/buttons.h \
           src/common.h \
           src/Owen/owen.h \
           src/Owen/programlaunch.h \
           src/Owen/timer.h \
    src/Owen/programstop.h \
    src/Owen/programbase.h \
    src/Owen/programlaunch2.h
