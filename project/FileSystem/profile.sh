make all

if [ -e "files" ]
then
	rm -rf files
fi
if [ -e "file_cache_size.csv" ]
then
	rm file_cache_size.csv
fi
if [ -e "random_access.csv" ]
then
	rm random_access.csv
fi
if [ -e "sequential_access.csv" ]
then
	rm sequential_access.csv
fi

# Section 4.1 (File Cache)
file_cache() {
	mkdir measurements/file_read
	touch file_cache_size.csv
	fcmodules=( 32 64 128 256 512 1024 2048 4096 8192 16384)
	for i in "${fcmodules[@]}"; do
		echo 3 > /proc/sys/vm/drop_caches
		./bin/file_cache_size /dev/sda5 $i measurements/file_read/file_cache_size.csv 5
		echo "==================================="
	done
}

# Section 4.2 (File reading)
file_read() {
	touch random_access.csv
	touch sequential_access.csv
	modules=( 32 64 128 256 512 1024 2048 4096 8192 16384 )
	for i in "${modules[@]}"; do
		echo "Sequential access (file size $i MB)"
		sudo ./bin/sequential_access /dev/sda5 $i measurements/file_read/random_access.csv 5
		echo "Random access (file size $i MB)"
		sudo ./bin/random_access /dev/sda5 $i measurements/file_read/sequential_access.csv 5
		echo "==================================="
	done
}

# Section 4.3 (File reading on NFS)
file_read_nfs() {
	touch nfs_access.csv
	modules=( 32 64 128 256 512 1024 2048 4096 8192 16384 )
	for i in "${modules[@]}"; do
		#echo "NFS sequential access (file size $i MB)"
		#sudo ./bin/sequential_access /mnt/workstation $i measurements/file_read/nfs_access.csv 5
		echo "NFS Random access (file size $i MB)"
		sudo ./bin/nfs_random_access /home/rmp/share_cse221/bigfile1 $i measurements/file_read/nfs_access.csv 5
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

#file_cache
#file_read
file_read_nfs
#contention

make clean
