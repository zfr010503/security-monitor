
KERNELDIR :=/lib/modules/$(shell uname -r)/build
#KERNELDIR :=/home/fengjunhui/kernel/linux-3.14
PWD  :=$(shell pwd)

#CROSS_COMPILE=arm-linux-
CROSS_COMPILE=

CC=$(CROSS_COMPILE)gcc
CP=cp

BOA_WWWDIR=/boa/www
BOA_BINDIR=/boa/cgi-bin

obj-m +=demox86.o 

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	$(CC) -o cgi_led.cgi -static cgi_led.c

install:
	sudo $(CP) meiyanfang.mp3 *.jpg led.html $(BOA_WWWDIR)
	sudo $(CP) cgi_led.cgi  $(BOA_BINDIR)

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf cgi_led.cgi


