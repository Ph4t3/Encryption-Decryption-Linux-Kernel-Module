obj-m = encdec.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

remove:
	test -n "$(shell grep -e "^encdec" /proc/modules)" && rmmod encdec.ko || echo "removed"
	rm /dev/encdec 2> /dev/null || echo "removed"

test:
	gcc test.c && ./a.out
	
clean:
	rm a.out 2> /dev/null || echo "removed a.out"
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
