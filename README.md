# Weather Station
Arduino sketch for remote weather station. Collect weather data from sensors and send it to remote server via GSM-module.

# Components
* Arduino Nano or analog 
* 3.3V and 5.0V power supply (I used MB 102 for my breadboard), use powefull power adapter ~5.0V, 1.5A because of SIM800L high peak current (up to 2.0A)
* SIM800L - GSM-module
* DHT22 - temperature and humidity sensor
* BH1750 - luminosity sensor
* BMP180 - atmospheric pressure sensor
* Rain drop module - MH-RD with board
* 2 x 1k resistors

# Pinout
## Power
All devices power pins to power supply outputs, not to Arduino, it's not powerfull enough.
* Arduino VIN -> 5.0 VDC
* SIM800L VCC -> 5.0 VDC (actually 3.4-4.5 V, but 5.0 is ok on MB 102)
* DHT22 VCC -> 3.3 VDC
* BH1750 VCC -> 3.3 VDC
* BMP180 VIN -> 3.3 VDC
* MH-RD VCC -> 5.0 VDC
* All device GND pins -> power supply GND

## Arduino
* REF -> 5.0 VDC (to make rain drop sensor work correctly)
* A0 -> MH-RD A0
* A4 -> BH1750 SDA and BMP180 SDA
* A5 -> BH1750 SCL and BMP180 SCL (BH1750 and BMP180 are both [I2C](https://en.wikipedia.org/wiki/I%C2%B2C) devices and can be connected parallel)
* D2 -> DHT22 DAT
* D8 -> 2 x 1k resistor equal [voltage divider](https://en.wikipedia.org/wiki/Voltage_divider) -> SIM800L RXD (SIM800L takes 2.5V while arduino gives 5.0V on digital output)
* D9 -> SIM800L TXD

# Libs
Included directly to project `libs/`.
* [ArduinoSIM800L](https://github.com/carrascoacd/ArduinoSIM800L) - HTTP client library for Arduino and SIM800L (SIM800) board to perform GET and POST requests to a JSON API
* [SimpleDHT](https://github.com/winlinvip/SimpleDHT) - Simple, Stable and Fast Arduino Temp & Humidity Sensors for DHT11 and DHT22.
* [BMP180_Breakout_Arduino_Library](https://github.com/sparkfun/BMP180_Breakout_Arduino_Library) - Arduino libraries for the BMP180 pressure sensor breakout board
* [BH1750](https://github.com/claws/BH1750) -  An Arduino library for the digital light sensor breakout boards containing the BH1750FVI IC
* [Time](https://github.com/PaulStoffregen/Time) - Time library for Arduino http://playground.arduino.cc/code/time
* [Arduino MemoryFree](https://github.com/maniacbug/MemoryFree) - Arduino MemoryFree library http://www.arduino.cc/playground/Code/AvailableMemory

# Build and upload
Download [Arduino IDE](https://www.arduino.cc/download_handler.php)<br>
Unpack it to home directory<br>
Go to 'arduino-x.y.z/' directory and run `./install.sh` to install GUI Arduino IDE<br>
This project is designed to be built with [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile)<br>
Clone Arduino-Makefile project:```git clone https://github.com/sudar/Arduino-Makefile.git```<br>
Install avrdude: ```sudo apt-get install avrdude```<br>
Check and fix pathes in Makefile.<br>

To build binary run: `make`<br>
Build flags:
* VERBOSE - turn on output debug info to serial port (9600 bauds)
* MEMORY - turn on checking current free RAM
To use flags run: `make FLAGS="-DVERBOSE -DMEMORY"` or one of them.

To flash binary plug your Arduino via USB, make sure that it's OK: `ls -l /dev | grep ttyUSB`, usually it's `/dev/ttyUSB0`, if not, change it to actual value in `flash.sh`.<br>
Then run: `./flash.sh`

# Server side
Sketch send `application/json` content-type POST request with JSON string:
```
{
  "e": 0,
  "id": 1,
  "ts": 1532869808,
  "t": 29,
  "h": 59,
  "p": 994,
  "l": 305,
  "r": 1023
}
```
Legend:
* e - error code (look to 'error.h')
* id - id, if there are several clients
* ts - exact timestamp of measurement
* t - temperature, °C
* h - relative humidity, %
* p - atmospheric pressure, mbar
* l - luminosity, lux
* r - rain measure, 0-1023 (0 - max rain, 1023 - dry)
