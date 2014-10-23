modules=( kernel_thread timing )
for i in "${modules[@]}"
	do
	echo "============$i=================="
	cd $i
	make clean
	sudo rmmod kernel_thread
	make all
	sudo insmod kernel_thread.ko
	sleep 1
	timeout 1 tail -f /var/log/syslog
	cd ..
	echo "==============$i END============"
done
