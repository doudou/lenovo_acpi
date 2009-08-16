ifneq ($(KERNELRELEASE),)
  obj-m := kill-ati.o
else
  KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $(EXTRA_FLAGS) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $(EXTRA_FLAGS) clean

endif

