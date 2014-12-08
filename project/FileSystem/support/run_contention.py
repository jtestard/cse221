import os
import sys

count = int(sys.argv[1])
path = "measurements/contention/raw"
for i in xrange(1,count + 1):
	for j in xrange(i):
		# Append file with id info
		# os.system("echo \"%d %d \" >> %s/process[%d,%d].csv" % (i, j, path, i, j))
		# Perform experiment
		os.system("sudo ./bin/random_access %s%d 1 %s/process[%d,%d].csv 1&" %
		("files/file", j, path, i, j))
