# OS Lab Assignment 1
## Vignere Cipher Encrypt/Decrypt Kernel Module

* Run the following commands to load kernel module
```
    sudo mknod /dev/encdec c 150 1
    sudo chmod a+r+w /dev/encdec
    sudo make
    sudo insmod encdec.ko
```
* To test the device, compile and run the test.c program
```
    make test
```
* To see the system log, use the command
```
    tail -f /var/log/syslog
```
* To unload the kernel module run the following commands
```
    sudo make clean
    sudo make remove
```
