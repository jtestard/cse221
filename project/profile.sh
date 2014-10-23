modules=( kernel_thread process )
for i in "${modules[@]}"
	do
	echo "============$i=================="
	cd $i
	make clean
	sudo rmmod $i
	make all
	sudo insmod $i.ko
	sleep 1
	timeout 1 tail -f /var/log/syslog
	cd ..
	echo "==============$i END============"
done
