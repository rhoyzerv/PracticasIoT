#!/bin/bash

while true
do
echo 0 > /sys/class/gpio/gpio16/value
echo 1 > /sys/class/gpio/gpio21/value
cat /sys/class/gpio/gpio16/value
cat /sys/class/gpio/gpio21/value
sleep 1
echo 0 > /sys/class/gpio/gpio21/value
echo 1 > /sys/class/gpio/gpio26/value
cat /sys/class/gpio/gpio21/value
cat /sys/class/gpio/gpio26/value
sleep 1
echo 0 > /sys/class/gpio/gpio26/value
echo 1 > /sys/class/gpio/gpio20/value
cat /sys/class/gpio/gpio26/value
cat /sys/class/gpio/gpio20/value
sleep 1
echo 0 > /sys/class/gpio/gpio20/value
echo 1 > /sys/class/gpio/gpio16/value
cat /sys/class/gpio/gpio20/value
cat /sys/class/gpio/gpio16/value
sleep 1
done
