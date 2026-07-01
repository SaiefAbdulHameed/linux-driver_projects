# Character Device Driver - Project 1

## About
This is my first Linux kernel driver. I built it while learning how 
kernel modules work and how userspace programs talk to the kernel.
It went through several iterations as I learned — starting with basic
read/write, then adding proper offset handling, mutex locking, and ioctl.

## What it does
A simple character device at `/dev/saief_driver` that:
- Stores data you write to it
- Lets you read it back
- Supports 3 ioctl commands to manage the buffer

## ioctl commands
- `CLEAR_BUF` — clears the buffer
- `READ_BUF` — returns how many bytes are stored
- `WRITE_BUF` — sets a custom size limit

## What I learned building this
- How kernel modules are structured and loaded
- Character device registration
- File operations — open, read, write, release, llseek
- Why loff_t matters and how offset tracking works
- Mutex locking to prevent concurrent access bugs
- ioctl — what it is, when to use it vs read/write
- How copy_to_user and copy_from_user work
- Proper error handling at each step

## Build and run
```bash
# build driver
make

# load driver
sudo insmod device.ko

# build test program
gcc user.c -o user

# run test
sudo ./user

# remove driver
sudo rmmod device

# check kernel logs
dmesg | tail
```

## Known limitations
- Buffer is fixed at 255 bytes
- No dynamic memory allocation
- Basic error handling only

## License
GPL v2
