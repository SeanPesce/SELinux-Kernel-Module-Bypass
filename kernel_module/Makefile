# Author: Sean Pesce

PWD := $(shell pwd)
obj-m += disable_selinux.o

all:
		make -C $(KERNEL) SUBDIRS=$(PWD) modules
cross:
		make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) -C $(KERNEL) SUBDIRS=$(PWD) modules
clean:
		make -C $(KERNEL) SUBDIRS=$(PWD) clean
