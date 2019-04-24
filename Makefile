MODULE_NAME  = OS_PJ1
obj-m       := $(MODULE_NAME).o   

all:
	gcc main.c proc_sort.c -o user.o
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean