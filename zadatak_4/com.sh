#!/bin/bash
if [ $1 -eq 0 ]
then
    gcc periodic_tasks.c -lpthread -lrt -Wall -o task
else
    arm-linux-gnueabihf-gcc periodic_tasks.c -lpthread -lrt -Wall -o task_rpi
fi

