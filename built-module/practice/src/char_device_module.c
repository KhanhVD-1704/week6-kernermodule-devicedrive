#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#include <char_buffer_ioctl.h>

#define DEVICE_NAME "char_buffer"
#define FIRST_MINOR 0
#define DEVICE_COUNT 1
#define BUFFER_SIZE 256

static dev_t device_number;
static struct cdev char_buffer_cdev;
static struct class *char_buffer_class;
static struct device *char_buffer_device;
static char device_buffer[BUFFER_SIZE];
static size_t data_size;
static DEFINE_MUTEX(buffer_lock);

// Hàm Kernel gọi
static int char_buffer_open(struct inode *inode, struct file *file)
{
	pr_info("char_device_module: device opened, major=%u minor=%u\n",
		imajor(inode), iminor(inode));

	return 0;
}

static int char_buffer_release(struct inode *inode, struct file *file)
{
	pr_info("char_device_module: device closed, major=%u minor=%u\n",
		imajor(inode), iminor(inode));

	return 0;
}

static ssize_t char_buffer_read(struct file *file, char __user *user_buffer,
				size_t count, loff_t *offset)
{
	ssize_t bytes_read;

	if (mutex_lock_interruptible(&buffer_lock))
		return -ERESTARTSYS;

	bytes_read = simple_read_from_buffer(user_buffer, count, offset,
					     device_buffer, data_size);
	mutex_unlock(&buffer_lock);

	if (bytes_read >= 0)
		pr_info("char_device_module: read %zd bytes\n", bytes_read);

	return bytes_read;
}

// Hàm Kernel gọi
static ssize_t char_buffer_write(struct file *file,
				 const char __user *user_buffer, size_t count,
				 loff_t *offset)
{
	size_t bytes_to_write;
	ssize_t result;

	bytes_to_write = min(count, (size_t)(BUFFER_SIZE - 1));

	if (mutex_lock_interruptible(&buffer_lock))
		return -ERESTARTSYS;

	if (copy_from_user(device_buffer, user_buffer, bytes_to_write)) {
		result = -EFAULT;
		goto out;
	}

	device_buffer[bytes_to_write] = '\0';
	data_size = bytes_to_write;
	result = bytes_to_write;

	pr_info("char_device_module: wrote %zu bytes\n", bytes_to_write);

out:
	mutex_unlock(&buffer_lock);
	return result;
}

// ioctl
static long char_buffer_ioctl(struct file *file, unsigned int command,
			      unsigned long argument)
{
	unsigned int current_size;
	long result = 0;

	if (mutex_lock_interruptible(&buffer_lock))
		return -ERESTARTSYS;

	switch (command) {
	case CHAR_BUFFER_CLEAR:
		memset(device_buffer, 0, sizeof(device_buffer));
		data_size = 0;

		pr_info("char_device_module: buffer cleared\n");
		break;

	case CHAR_BUFFER_GET_SIZE:
		current_size = data_size;
		if (copy_to_user((unsigned int __user *)argument, &current_size,
				 sizeof(current_size)))
			result = -EFAULT;
		break;

	default:
		result = -ENOTTY;
	}

	mutex_unlock(&buffer_lock);
	return result;
}

// Đăng ký callback
static const struct file_operations char_buffer_fops = {
	.owner = THIS_MODULE,
	.open = char_buffer_open,
	.write = char_buffer_write,
	.read = char_buffer_read,
	.release = char_buffer_release,
	.unlocked_ioctl = char_buffer_ioctl,
};

// Hàm khởi tạo module nhân
static int __init char_device_module_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&device_number, FIRST_MINOR, DEVICE_COUNT,
				  DEVICE_NAME);
	if (ret) {
		pr_err("char_device_module: failed to allocate device number: "
		       "%d\n",
		       ret);
		return ret;
	}

	pr_info("char_device_module: allocated major=%u minor=%u\n",
		MAJOR(device_number), MINOR(device_number));

	cdev_init(&char_buffer_cdev, &char_buffer_fops);

	ret = cdev_add(&char_buffer_cdev, device_number, DEVICE_COUNT);
	if (ret) {
		pr_err("char_device_module: failed to add cdev: %d\n", ret);
		unregister_chrdev_region(device_number, DEVICE_COUNT);
		return ret;
	}

	char_buffer_class = class_create(DEVICE_NAME);
	if (IS_ERR(char_buffer_class)) {
		ret = PTR_ERR(char_buffer_class);
		pr_err("char_device_module: failed to create device class: %d\n",
		       ret);
		goto err_del_cdev;
	}

	char_buffer_device = device_create(char_buffer_class, NULL, device_number,
					   NULL, DEVICE_NAME);
	if (IS_ERR(char_buffer_device)) {
		ret = PTR_ERR(char_buffer_device);
		pr_err("char_device_module: failed to create device: %d\n", ret);
		goto err_destroy_class;
	}

	pr_info("char_device_module: Loaded\n");

	return 0;

err_destroy_class:
	class_destroy(char_buffer_class);
err_del_cdev:
	cdev_del(&char_buffer_cdev);
	unregister_chrdev_region(device_number, DEVICE_COUNT);
	return ret;
}

static void __exit char_device_module_exit(void)
{
	device_destroy(char_buffer_class, device_number);
	class_destroy(char_buffer_class);
	cdev_del(&char_buffer_cdev);
	unregister_chrdev_region(device_number, DEVICE_COUNT);

	pr_info("char_device_module: Unloaded\n");
}

module_init(char_device_module_init);
module_exit(char_device_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khanh");
MODULE_DESCRIPTION("Loadable character device module");
MODULE_VERSION("0.1");
