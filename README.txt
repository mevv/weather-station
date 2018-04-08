# METEO

This is adrduino firmware for autonomous meteorological stations.

# BUILD AND FLASH

Linux

Download Arduino IDE (https://www.arduino.cc/download_handler.php)
Unpack it to home directory
Go to 'arduino-x.y.z/' directory and run './install.sh' to install GUI Arduino IDE
Now you can use to open '*.ino' project file.

This project is designed to be built with Arduino-Makefile
Clone Arduino-Makefile project: 'git clone https://github.com/sudar/Arduino-Makefile.git'
Install avrdude from repo: 'sudo apt-get install avrdude' for Debian-like systems or analog for others
Go to project directory and build it: 'make'
Plug your Arduino Nano based test stand via USB, make sure that it's OK: 'ls -l /dev | grep ttyUSB', usually it's '/dev/ttyUSB0', if not, change it to actual value in 'flash.sh' and 'serial_monitor.sh'
Flash your binary to board: 'sudo ./flash.sh'
Now you can check serial port output using 'serial_monitor.sh': 'sudo ./serial_monitor.sh' (don't forget to shutdown it, if you want to reflash binary)

Some issues

Using built-in Arduino IDE serial monitor:
In '$(HOME)/.arduino15/preferences.txt' change serial port option to 'serial.port=/dev/ttyUSB*', if it's wrong (don't forget to close Arduino IDE before editing this file)
Set up permissions: 'http://playground.arduino.cc/Linux/All#Permission', relogin to your system after that

