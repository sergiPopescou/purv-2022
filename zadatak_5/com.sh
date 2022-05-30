#!/bin/bash
if [ $1 -eq 0 ]
then
    gcc procesi.c -Wall -o proces
else
    arm-linux-gnueabihf-gcc procesi.c -lrt -Wall -o proces_rpi
fi