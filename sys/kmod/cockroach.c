#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("cockroach");
MODULE_DESCRIPTION ("Learning kernel module writing");
MODULE_VERSION ("v0.0.1");

#define DEVICE_NAME "cockroach_dev"
#define EXAMPLE_MSG "Hello, from cockroach\n"
#define MSG_BUFFER_LEN 30

/* Prototypes for device functions.  */
static int device_open (struct inode *, struct file *);
static int device_close (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *, size_t, loff_t *);
static ssize_t device_write (struct file *, const char *, size_t, loff_t *);
static int major_num;
static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;

/* This structure points to all of device functions.  */
struct file_operations file_ops = {
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_close
};

/* When a process reads from our device, this gets called  */
static ssize_t device_read (struct file *filp, char *buffer,
							size_t len, loff_t *offset)
{
	int bytes_read = 0;

	/* To allow device read again and again.  */
	if (*msg_ptr == 0) {
		msg_ptr = msg_buffer;
	}

	while (len && *msg_ptr) {
		put_user (*(msg_ptr++), buffer++);
		len--;
		bytes_read++;
	}
	return bytes_read;
}

/* When a process writes to our device.  */
static ssize_t device_write (struct file *filp, const char *buffer,
							 size_t len, loff_t *offset)
{
	printk (KERN_ALERT "This operation is not supported");
	return -EINVAL;
}

/* Called when a process opens our device.  */
static int device_open (struct inode *inode, struct file *filp)
{
	if (device_open_count) {
		return -EBUSY;
	}
	device_open_count++;
	module_put (THIS_MODULE);
	return 0;
}

/* Called when a process closes our decice.  */
static int device_close (struct inode *inode, struct file *filp)
{
	device_open_count--;
	module_put (THIS_MODULE);
	return 0;
}

static int __init cockroach_init (void)
{
	strncpy (msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
	msg_ptr = msg_buffer;
	major_num = register_chrdev (0, "cockroach_dev", &file_ops);
	if (major_num < 0) {
		printk (KERN_ALERT "Could not register device: %d\n", major_num);
		return major_num;
	}
	else {
		printk (KERN_INFO "cockroach module loaded with device major number %d\n",
			    major_num);
		return 0;
	}
}

static void __exit cockroach_exit (void)
{
	unregister_chrdev (major_num, DEVICE_NAME);
	printk (KERN_INFO "cockroach module was unloaded");
}

module_init (cockroach_init);
module_exit (cockroach_exit);
