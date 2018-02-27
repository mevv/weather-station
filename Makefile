# Arduino Make file https://github.com/sudar/Arduino-Makefile

ARDUINO_DIR = /home/valera/Downloads/arduino-1.8.5
ARDMK_DIR = /home/valera/Desktop/arduino/Arduino-Makefile
AVR_TOOLS_DIR = /home/valera/Downloads/arduino-1.8.5/hardware/tools/avr

USER_LIB_PATH = /home/valera/Desktop/arduino/ArduinoIDE/meteo/lib
ARDUINO_LIBS = SIM800L BH1750 BMP180 DHT Time SoftwareSerial Wire

BOARD_TAG = nano

include /home/valera/Desktop/arduino/Arduino-Makefile/Arduino.mk
