# This script should be use to profile kernel modules.
modules=( kernel_thread )
for i in "${modules[@]}"
	do
	echo "============$i=================="
	cd $i
	make all
	sudo insmod $i.ko
	sleep 1
	timeout 1 tail -f /var/log/syslog
	make clean
	sudo rmmod $i
	cd ..
	echo "==============$i END============"
done
