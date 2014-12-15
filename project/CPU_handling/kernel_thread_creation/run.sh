make all
sudo insmod kernel_thread.ko
sleep 1
timeout 1 tail -f /var/log/syslog #> ../measurements/kernel_thread.log
make clean
sudo rmmod kernel_thread

