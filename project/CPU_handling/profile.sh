# This script should be use to profile kernel modules.

# Kernel module section
modules=( kernel_thread )
for i in "${modules[@]}"
	do
	echo "============$i=================="
	cd $i
	make all
	sudo insmod $i.ko
	sleep 1
	timeout 1 tail -f /var/log/syslog > ../measurements/$i.log
	make clean
	sudo rmmod $i
	cd ..
	echo "==============$i END============"
done

# User process section
modules=( process context_switch loop_timing timing )
for i in "${modules[@]}"
	do
	echo "============$i=================="
	cd $i
	make all
	./$i > ../measurements/$i.log
	make clean
	cd ..
	echo "==============$i END============"
done

