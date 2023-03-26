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

#define GPX2CON 0x11000c40
#define GPX2DAT 0x11000c44
#define GPX1CON 0x11000c20
#define GPX1DAT 0x11000c24
#define GPF3CON 0x114001e0
#define GPF3DAT 0x114001e4

void __iomem * gpx2con_vir;
void __iomem * gpx2dat_vir;
void __iomem * gpx1con_vir;
void __iomem * gpx1dat_vir;
void __iomem * gpf3con_vir;
void __iomem * gpf3dat_vir;


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
	int i;
	led_desc_t *led = (led_desc_t *)args;
	printk("---->%s--->%d\n",__func__,__LINE__);

	switch(cmd){
		case FS_LED_ON:
		i = led->led_num;
		printk("i= %d\n",i);
			if(i == 2){
				writel(readl(gpx2dat_vir) | (0x1 << 7),gpx2dat_vir);
			}
			else if(i == 3){
				writel(readl(gpx1dat_vir) | (0x1 << 0),gpx1dat_vir);
			}else if(i == 4){
				writel(readl(gpf3dat_vir) | (0x1 << 4),gpf3dat_vir);
			}else if(i == 5){
				writel(readl(gpf3dat_vir) | (0x1 << 5),gpf3dat_vir);
			}
			printk("FS_LED_ON. \n");
			break;
		case FS_LED_OFF:
		i = led->led_num;
		printk("i= %d\n",i);
			if(i == 2){
				writel(readl(gpx2dat_vir)&~(0x1 << 7),gpx2dat_vir);
			}else if(i == 3){
				writel(readl(gpx1dat_vir)&~(0x1 << 0),gpx1dat_vir);
			}else if(i == 4){
				writel(readl(gpf3dat_vir)&~(0x1 << 4),gpf3dat_vir);
			}else if(i == 5){
				writel(readl(gpf3dat_vir)&~(0x1 << 5),gpf3dat_vir);
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


void gpio_ioremap(void)
{
	gpx2con_vir = ioremap(GPX2CON,8);
	gpx2dat_vir = gpx2con_vir + 4;
	
	gpx1con_vir = ioremap(GPX1CON,8);
	gpx1dat_vir = gpx1con_vir + 4;
	
	gpf3con_vir = ioremap(GPF3CON,8);
	gpf3dat_vir = gpf3con_vir + 4;
	
	writel((readl(gpx2con_vir) & ~(0XF<< 28))| (0x1 << 28),gpx2con_vir);
	writel((readl(gpx1con_vir) & ~(0XF<< 0))| (0x1 << 0),gpx1con_vir);
	writel((readl(gpf3con_vir) & ~(0XF<< 16 ))| (0x1 << 16),gpf3con_vir);
	writel((readl(gpf3con_vir) & ~(0XF<< 20 ))| (0x1 << 20),gpf3con_vir);
	
}


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

	gpio_ioremap();
	return 0;
}

void viraddr_iounmap(void)
{
	iounmap(gpx2con_vir);
	iounmap(gpx1con_vir);
	iounmap(gpf3con_vir);
}

static void __exit demo_exit(void)
{
	printk("---->%s--->%d\n",__func__,__LINE__);
	viraddr_iounmap();
	device_destroy(cls,MKDEV(major,0));
	class_destroy(cls);
	unregister_chrdev(major,name);
}


module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

