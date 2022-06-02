CR_C := arm-linux-gnueabihf-
MODULES := /home/nikola/Raspberrypi_tools/rpi-kernel/linux
obj-m := GoL.o

all:
	make ARCH=arm CROSS_COMPILE=$(CR_C) -C $(MODULES) M=$(shell pwd) modules

clean:
	make ARCH=arm CROSS_COMPILE=$(CR_C) -C $(MODULES) M=$(shell pwd) clean