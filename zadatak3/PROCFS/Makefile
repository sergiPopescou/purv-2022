# The same Makefile without the comments
MYY_KERNEL_DIR ?= ~/rpi-kernel/linux
ARCH ?= arm

INSTALL_MOD_PATH ?= /tmp/RockMyy-Build
INSTALL_PATH     ?= $(INSTALL_MOD_PATH)/boot
INSTALL_HDR_PATH ?= $(INSTALL_MOD_PATH)/usr

ccflags-y += -I${src}/include -DMY_SPECIAL_MACRO_NAME=1

my-driver-objs := my_source_file.o my_other_source_file.o
my-driver-objs += yet_another_source_file.o

obj-m += proces.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) -C $(MYY_KERNEL_DIR) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) -C $(MYY_KERNEL_DIR) clean
