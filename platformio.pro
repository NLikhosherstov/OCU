win32 {
    HOMEDIR += $$(USERPROFILE)
}
else {
    HOMEDIR += $$(HOME)
}

INCLUDEPATH += "$$PWD/include"
INCLUDEPATH += "$$PWD/src"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/MAX6675 library"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/Adafruit Unified Sensor"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/DHT sensor library"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/Adafruit GFX Library"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/Adafruit BusIO"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/EncButton/src"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/GyverIO/src"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/microWire/src"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/GyverOLED/src"
INCLUDEPATH += "$$PWD/.pio/libdeps/nanoatmega328/GyverTimers/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduino-avr/libraries/Wire/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduino-avr/libraries/SPI/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduino-avr/cores/arduino"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduino-avr/variants/eightanaloginputs"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduino-avr/libraries/EEPROM/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduino-avr/libraries/HID/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduino-avr/libraries/SoftwareSerial/src"
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
           src/Owen/programstop.cpp \
           src/Owen/programlaunch2.cpp

HEADERS += src/main.h \
           src/monitor.h \
           src/buttons.h \
           src/common.h \
           src/Owen/owen.h \
           src/Owen/timer.h \
           src/Owen/programstop.h \
           src/Owen/programbase.h \
           src/Owen/programlaunch2.h \
           src/resource.h
