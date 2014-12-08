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

touch random_access.csv
touch sequential_access.csv
mkdir files
python support/make_files.py files/file 20

exit

modules=( 32 64 128 256 512 1024 2048 4096 8192 16384 )
for i in "${modules[@]}":
	do
	echo "Sequential access (file size $i MB)"
	sudo ./bin/sequential_access /dev/sda1 $i
	echo "Random access (file size $i MB)"
	sudo ./bin/random_access /dev/sda1 $i
	echo "==================================="
done

make clean
rm -rf files
