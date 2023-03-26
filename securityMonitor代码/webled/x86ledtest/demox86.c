#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "chardev.h"

const char *name = "chrdev";
unsigned int  major ;
struct class *cls;
struct device *dev;


char kbuf[] = {'1','2','3','4'};

loff_t demo_llseek(struct file *filp, loff_t offset, int cnt)
{

	printk("---->%s--->%d\n",__func__,__LINE__);
	return 0;
}

ssize_t demo_read(struct file *filp, char __user *usrbuf, size_t count, loff_t *offset)
{
	int bytes = 0;
	printk("---->%s--->%d\n",__func__,__LINE__);

	bytes =	copy_to_user(usrbuf,kbuf,4);
	if(bytes > 0){
		printk("copy_to_user failed!\n");
	}

	return 0;
}

ssize_t demo_write(struct file *filp, const char __user *usrbuf, size_t size, loff_t *offset)
{
	int bytes = 0;
	printk("---->%s--->%d\n",__func__,__LINE__);

	bytes = copy_from_user(kbuf,usrbuf,4);
	if(bytes > 0){
		printk("copy_from_user failed\n");
		return -1;
	}
	printk("copy_from_user usrbuf:%c\n",kbuf[0]);
	return 0;
}


long demo_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	int i,ret;
	led_desc_t led;
	printk("---->%s--->%d\n",__func__,__LINE__);
	ret = copy_from_user((void *)&led,(void *)args,_IOC_SIZE(cmd));
	if (ret) {
		printk("copy_from_user failed\n");
		return ret;
	}
		
	switch(cmd){
		case FS_LED_ON:
		i = led.led_num;
		printk("i= %d\n",i);
			if(i == 2){
				printk("led2 ---->on.\n");
			}
			else if(i == 3){
				printk("led3 ---->on.\n");
			}else if(i == 4){
				printk("led4 ---->on.\n");
			}else if(i == 5){
				printk("led5 ---->on.\n");
			}
			printk("FS_LED_ON. \n");
			break;
		case FS_LED_OFF:
		i = led.led_num;
		printk("i= %d\n",i);
			if(i == 2){
				printk("led2 ---->off.\n");
			}else if(i == 3){
				printk("led3 ---->off.\n");
			}else if(i == 4){
				printk("led4 ---->off.\n");
			}else if(i == 5){
				printk("led5 ---->off.\n");
			}
			printk("FS_LED_OFF. \n");
			break;
		default:
			printk("default :....\n");
			break;
	}

	return 0;
}


int demo_open(struct inode *inode, struct file *filp)
{
	//硬件的初始化工作--收发数据的初始化
	printk("---->%s--->%d\n",__func__,__LINE__);
	return 0;
}

int demo_close(struct inode *inode, struct file *filp)
{
	
	printk("---->%s--->%d\n",__func__,__LINE__);
	return 0;
}

const struct file_operations fops = {
	.open=demo_open,
	.llseek=demo_llseek,
	.read=demo_read,
	.write=demo_write,
	.unlocked_ioctl=demo_ioctl,
	.release=demo_close,
};



static int __init demo_init(void)
{
	printk("---->%s--->%d\n",__func__,__LINE__);
	
	major = register_chrdev(0,name,&fops); 
	if(major <= 0){
		printk("register_chrdev failed!\n");
	}
	printk("register_chrdev success .major: %d\n",major);
	
	cls = class_create(THIS_MODULE,name);
	if(cls == NULL){
		printk("class_create failed!\n");
	}
	dev = device_create(cls, NULL,MKDEV(major,0),NULL,"chrdev0");
	if(dev == NULL){	
		printk("device_create failed!\n");
	}
	printk("class device_create success .\n");
	
	return 0;
}

static void __exit demo_exit(void)
{
	printk("---->%s--->%d\n",__func__,__LINE__);
	
	device_destroy(cls,MKDEV(major,0));
	class_destroy(cls);
	unregister_chrdev(major,name);
}


module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

