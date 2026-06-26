# Linux Character Driver - Project 1

## About

This is my first Linux kernel module project. I built it to learn the fundamentals of Linux character device drivers and how userspace communicates with the kernel.

The project implements a simple character driver that supports opening a device, reading from it, writing to it, and unloading the module safely.

## Features

* Dynamic character device registration
* Automatic device creation in `/dev`
* `open()` and `release()` file operations
* `read()` using `copy_to_user()`
* `write()` using `copy_from_user()`
* Basic initialization error handling
* Proper cleanup when the module is unloaded

## Technologies

* C
* Linux Kernel Modules (LKM)
* Linux Character Devices
* Make

## Project Structure

```
device.c      - Kernel module
user.c        - Simple userspace test program
Makefile      - Build script
```

## Build

```bash
make
```

## Load the Module

```bash
sudo insmod device.ko
```

Check that it loaded successfully:

```bash
dmesg | tail
```

## Compile the Test Program

```bash
gcc user.c -o user
```

## Run

```bash
./user
```

## Remove the Module

```bash
sudo rmmod device
```

## What I Learned

Through this project I learned:

* How Linux kernel modules are structured
* Character device registration (`alloc_chrdev_region`, `cdev_init`, `cdev_add`)
* Device classes and device creation
* Implementing file operations (`open`, `read`, `write`, `release`)
* Safe communication between kernel space and userspace using `copy_to_user()` and `copy_from_user()`
* Basic kernel error handling and resource cleanup

## Limitations

This is a learning project and is intentionally simple.

Current limitations include:

* Fixed-size kernel buffer
* No synchronization for concurrent access
* No `ioctl()` support
* No dynamic memory allocation
* Read operation does not yet fully implement file offsets

These are improvements I plan to make as I continue learning Linux kernel development.

## License

GPL v2
