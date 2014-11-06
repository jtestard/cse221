#Compile the C file located in C for allocation.
#Its a small file so we don't care if we recompile it each time 
#we execute the script.
g++ C/cgroups.cpp -o C/cgroups

#Execute the file with the argument given to the bash script
#This will apply the cgconfig.
#cgexec -g memory:limited 
cgexec -g memory:limited C/cgroups $1
