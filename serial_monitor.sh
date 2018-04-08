#!/bin/bash

BAUD="9600"
DEVICE="/dev/ttyUSB0"

# Port setting
stty -F $DEVICE cs8 $BAUD ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts

# Loop
while (true)
do
    read INPUT < $DEVICE
    echo $INPUT
done
