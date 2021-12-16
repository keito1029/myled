#!/bin/sh
cd ~/myled; make; rmmod myled; insmod myled.ko; chmod 666 /dev/myled0
