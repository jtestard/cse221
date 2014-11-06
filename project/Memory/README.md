-----------------------------
ZEN LIBRARY PROFILING
-----------------------------

This project contains code for an extensive profiling of Derek Ruths's Zen library under memory constraints.
My own profiling was done under Ubuntu 12.0.4 on an Asus computer with 8G RAM, 500GB of disk space and 4 2.2Ghz
intel core i7 processors. However, these scripts should let you profile Zen under your own computer, provided
it is using the Linux Operating system and you have super user access (only for the installation/configuration
of the Cgroups). The profiling methods used are specific to the Linux operating system. It is not 
possible to use the C and python files included here on another ooperating system.


Profiling data is generated using the following method.

1) Setup the Cgroups environment:
Cgroups is a Linux technogy (orignally unix, so maybe MAC OS X can use them as well) that allows user to specify how
much maximum virtual or physical memory they want a process to use. This library may not be installed on your computer
by default. The next script assumes that the Cgroups tool has been installed. If not, please use the following command to 
install it first:

	sudo apt-get install cgroups


Cgroups configuration can be done using the following command from the project root directory (note that superuser access may be required):
	
	sh sh/cgconfig.sh


2) Generating the graphs :
This is the only step that is not operating-system specific. You can generate graphs for the 5 graphs categories profiled
by using the following command.

	sh sh/generate_graphs
	
This operation takes time, because the graphs generated can be quite large (up to 300 MB), and a total of 100 graphs are generated.
The operation should take about 40 minutes (most of the time is spent on the metric graphs category, this is the only graph category
not to be written in Cython :s). Information about generating times and IO saving times as well as graph size are available in 
csv format under the /csv folder.

3) Profiling C :
This step shows how the computer reacts to C memory allocations under the Cgroups constraints. Graphs describing the virtual memory and RAM usage when using C
are available under the /csv folder. To obtain the C profiling, please execute the following command from the project's root directory : 

	sh sh/c_start.sh
	
4) Profiling Zen :

This step profiles the load time and query access time of the Zen library on each type of graph. Profiling results are saved in .csv files 
available under /csv. To obtain the Zen library profiling under the CGroups constraints, please execute the following command fro mthe 
project's root directory : 

	sh sh/python_start.sh
	
