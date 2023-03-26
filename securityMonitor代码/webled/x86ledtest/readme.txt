
具体操作看Makefile，Makefile中有详细的解释。


Makefile :for x86
demox86.c

make 
make install
sudo insmod demox86.ko
sudo dmesg -C
demsg 
[177789.671881] register_chrdev success .major: 249
[177789.672084] class device_create success .
[182378.055610] ---->demo_open--->101
[182378.056422] ---->demo_ioctl--->57
[182378.056427] i= 5
[182378.056429] led5 ---->on.
[182378.056430] FS_LED_ON. 
[182378.056436] ---->demo_close--->108
[182453.470875] ---->demo_open--->101
[182453.470967] ---->demo_ioctl--->57
[182453.470969] i= 4
[182453.470970] led4 ---->off.
[182453.470971] FS_LED_OFF. 
[182453.470974] ---->demo_close--->108http://127.0.0.1/led.html
input 3 1
press sure in led.html

