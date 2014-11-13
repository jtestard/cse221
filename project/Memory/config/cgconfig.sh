#Install cgroups libaries
sudo apt-get install cgroup-bin libcgroup1

# Delete old group
sudo cgdelete -g memory:limited

#Create cgroup for limited memory
sudo cgcreate -g memory:limited

#Configure Cgroup
sudo cgset -r memory.limit_in_bytes=75M limited
sudo cgset -r cpuset.cpus="0" limited

echo ""
echo "
The memory of processes in cgroup \"limited\" is now constrained to 75MB.
"
echo "
Now you can exec your files with the restriction using a command such as 
cgexec -g memory:limited <limited_process_file_name>
"

