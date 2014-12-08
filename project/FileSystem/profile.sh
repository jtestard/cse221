make all

if [ -e "files" ]
then
	rm -rf files
fi
if [ -e "random_access.csv" ]
then
	rm random_access.csv
fi
if [ -e "sequential_access.csv" ]
then
	rm sequential_access.csv
fi

# Section 4.2 (File reading)
file_read() {
	mkdir measurements/file_read
	touch random_access.csv
	touch sequential_access.csv
	modules=( 32 64 128 256 512 1024 2048 4096 8192 16384 )
	for i in "${modules[@]}"; do
		echo "Sequential access (file size $i MB)"
		sudo ./bin/sequential_access /dev/sda1 $i measurements/file_read/random_access.csv 5
		echo "Random access (file size $i MB)"
		sudo ./bin/random_access /dev/sda1 $i measurements/file_read/sequential_access.csv 5
		echo "==================================="
	done
}



# Section 4.4 (Contention)
contention() {
	mkdir files
	sudo rm measurements/contention/raw/*
	python support/make_files.py files/file 20
	python support/run_contention.py 20
	rm -rf files
}

file_read
contention

make clean
