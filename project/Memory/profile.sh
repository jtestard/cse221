#Compile the C file located in C for allocation.
#Its a small file so we don't care if we recompile it each time 
#we execute the script.
make all

#sudo cgexec -g memory:limited ./bin/experiment1
sudo ./bin/experiment1
sudo ./bin/experiment2

#Execute the file with the argument given to the bash script
#This will apply the cgconfig.
sudo cgexec -g memory:limited bin/experiment3

#Cleanup.
make clean
