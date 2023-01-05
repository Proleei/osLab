#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
//#include <stdlib.h>

#define NAME "osLab4"
#define BUF_SIZE 64
MODULE_LICENSE("GPL");

static int a=0, b=0;

static int myOpen(struct inode *inode, struct file *file)
{
	printk("%s:\t\tThis device has been opened.\n", __func__);
	return 0;
}

static ssize_t myRead(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	printk("%s:\t\ta + b = %d\n", __func__, a + b);
	return 0;
}

static ssize_t myWrite(struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
	char buf2[BUF_SIZE];
	count = count % (BUF_SIZE + 1); // control size
	if(copy_from_user(buf2, buf, count)) // copy to kernel from user failed.
	{
		return -1;
	}
	sscanf(buf2, "%d %d", &a, &b);
	printk("%s:\t\ta = %d, b = %d\n", __func__, a, b);
	return count;
}

static int myRelease(struct inode *inode, struct file *file)
{
    printk("%s:\tThis drive releases successfully.\n", __func__);
	return 0;
}

static const struct file_operations myDrvOps = {
	.owner = THIS_MODULE,
	.open = myOpen,
	.read = myRead,
	.write = myWrite,
	.release = myRelease
};

static struct miscdevice myMiscDevice = {
	.minor = MISC_DYNAMIC_MINOR, // 动态分配杂项设备次设备号
	.name = NAME,
	.fops = &myDrvOps
};

static int __init myInit(void)
{
	int retNum;
	retNum = misc_register(&myMiscDevice); // 驱动注册
	if (retNum) 
	{ // 注册失败
		printk("failed register device : %s\n", NAME);
		return retNum;
	}
	printk("successfully register device : %s\n", NAME);
	return 0;
}

static void __exit myExit(void)
{
	printk("removing device : %s\n", NAME);
	misc_deregister(&myMiscDevice); // 驱动注销
}

module_init(myInit);
module_exit(myExit);
