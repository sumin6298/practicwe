#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>

struct cdev my_cdev;
dev_t devno;
static char *msg = NULL;
int major;

module_param(major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major, "major");

int simple_open (struct inode *pinode, struct file *pfile)
{
	printk("Open simple char drv\n");
	return 0;
}

ssize_t simple_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk("Read simple char drv\n");
	return 0;
}

ssize_t simple_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	printk("Write simple char drv\n");
	return length;
}

int simple_close (struct inode *pinode, struct file *pfile)
{
	printk("Release simple char drv\n");
	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = simple_open, 
	.read = simple_read,
	.write = simple_write,
 	.release = simple_close,
};

int __init simpe2.init(void)
{
	int err;
	
	if(major){
		devno = MKDEV(major, 0);
		register_chrdev_region(devno, 1, "Simple char drv");
	}
	else {
		alloc_chrdev_region(&devno, 0, 1, "Simple char drv");
		major = MAJOR(devno);
	}
	
	cdev_init(&my_cdev, &fop);
	my_cdev.owner = THIS_MODULE;
	err = cdev_add(&my_cdev, devno, 1);
	
	if(err<0) {
		printk("Device and error\n");	
		return -1;
	}
	
	printk("Hello World. This is my first char dev.\n");
	printk("sudo mknod -m 666 /dev/simple_chr_drv c %d 0\n", MAJOR(devno));
	
	msg = (char *)kmalloc(32, GFP_KERNEL);
	if(msg != NULL)
		printk("malloc allocator address : 0x%p\n", msg);
	return 0;
}

void __exit simple2_exit(void)
{
	printk("Goodbye\n");
	
	if(msg){
		kfree(msg);
	}
	unregister_chrdev_region(devno, 1);
	cdev_del(&my_cdev);
	
}

module_init(simple2_init);
module_exit(simple2_exit);
	













































