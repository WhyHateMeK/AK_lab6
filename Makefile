ifneq ($(KERNELRELEASE),)
# kbuild part of Makefile
obj-m := hello.o
ccflags-y += -g
else
# Normal Makefile
KDIR ?= /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello.ko hello.ko.unstripped
	$(CROSS_COMPILE)strip -g hello.ko

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

%.s %.i: %.c
	$(MAKE) -C $(KDIR) M=$$PWD $@
endif
