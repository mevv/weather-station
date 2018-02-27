#!/bin/bash

CHIP="atmega328p"
TTY="/dev/ttyUSB0"
BAUD="57600"

avrdude -v -p $CHIP -c arduino -P $TTY -b $BAUD -D -U flash:w:build-nano/meteo.hex:i
