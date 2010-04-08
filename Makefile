ifneq ($(KERNELRELEASE),)
  obj-m := lenovo_acpi.o
else
  KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $(EXTRA_FLAGS) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $(EXTRA_FLAGS) clean

install:
	mkdir -p /lib/modules/$(shell uname -r)/misc
	cp lenovo_acpi.ko /lib/modules/$(shell uname -r)/misc
	depmod -a

endif

