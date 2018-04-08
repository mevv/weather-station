# Arduino Make file https://github.com/sudar/Arduino-Makefile

ARDUINO_DIR = $(HOME)/arduino-1.8.5
ARDMK_DIR = $(HOME)/source/arduino/Arduino-Makefile
AVR_TOOLS_DIR = /home/valera/arduino-1.8.5/hardware/tools/avr

BOARDS_TXT = ./boards.txt

USER_LIB_PATH = ./lib
ARDUINO_LIBS = SIM800L BH1750 BMP180 DHT Time SoftwareSerial Wire

BOARD_TAG = nano
#BOARD_SUB = atmega328

include $(HOME)/source/arduino/Arduino-Makefile/Arduino.mk
