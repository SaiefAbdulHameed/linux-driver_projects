#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAIEF");
MODULE_DESCRIPTION("Simple Character Driver");

#define MY_DEVICE_NAME "saief_driver"

static dev_t dev_nr;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static char buffer[255];
static size_t buffer_pointer;

static int open_driver(struct inode *inode, struct file *file)
{
    pr_info("Device opened\n");
    return 0;
}

static int close_driver(struct inode *inode, struct file *file)
{
    pr_info("Device closed\n");
    return 0;
}

static ssize_t read_drive(struct file *file,
                          char __user *user_buffer,
                          size_t count,
                          loff_t *off)
{
    size_t to_copy, not_copy, delta;

    to_copy = min(count, buffer_pointer);

    if (to_copy == 0)
        return 0;

    not_copy = copy_to_user(user_buffer, buffer, to_copy);
    delta = to_copy - not_copy;

    if (not_copy)
        pr_warn("Failed to copy %zu byte(s) to userspace\n", not_copy);

    return delta;
}

static ssize_t write_drive(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *off)
{
    size_t to_copy, not_copy, delta;

    to_copy = min(count, sizeof(buffer));

    not_copy = copy_from_user(buffer, user_buffer, to_copy);
    delta = to_copy - not_copy;

    buffer_pointer = delta;

    if (not_copy)
        pr_warn("Failed to copy %zu byte(s) from userspace\n", not_copy);

    return delta;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = open_driver,
    .release = close_driver,
    .read = read_drive,
    .write = write_drive,
};

static int __init ModuleInit(void)
{
    int ret;

    /* Allocate device number */
    ret = alloc_chrdev_region(&dev_nr, 0, 1, MY_DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate device number\n");
        return ret;
    }

    /* Initialize cdev */
    cdev_init(&my_cdev, &fops);

    /* Add cdev */
    ret = cdev_add(&my_cdev, dev_nr, 1);
    if (ret) {
        pr_err("Failed to add cdev\n");
        unregister_chrdev_region(dev_nr, 1);
        return ret;
    }

    /* Create device class */
    my_class = class_create("saief_class");
    if (IS_ERR(my_class)) {
        pr_err("Failed to create class\n");
        ret = PTR_ERR(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_nr, 1);
        return ret;
    }

    /* Create device */
    my_device = device_create(my_class, NULL, dev_nr, NULL, MY_DEVICE_NAME);
    if (IS_ERR(my_device)) {
        pr_err("Failed to create device\n");
        ret = PTR_ERR(my_device);
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_nr, 1);
        return ret;
    }

    pr_info("Driver loaded successfully\n");
    pr_info("Major = %d, Minor = %d\n", MAJOR(dev_nr), MINOR(dev_nr));

    return 0;
}

static void __exit ModuleExit(void)
{
    device_destroy(my_class, dev_nr);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_nr, 1);

    pr_info("Driver unloaded\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);