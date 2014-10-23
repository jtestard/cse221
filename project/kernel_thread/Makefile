ifneq ($(KERNELRELEASE),)
	obj-m := threads.o
else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR)  M=$(PWD) clean

endif
