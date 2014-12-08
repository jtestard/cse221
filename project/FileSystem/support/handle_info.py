import sys

count = int(sys.argv[1])
path = "measurements/contention"
with open("%s/analysis.csv" % path,"w+") as dest:
	for i in xrange(1,count):
		avg_time = 0.0
		for j in xrange(i):
			with open("%s/raw/process[%d,%d].csv" % (
				path,
				i,
				j
			), "r") as src:
				data = src.readlines()[0].split(" ")
				avg_time += float(data[2])
				pass
			pass
		avg_time = avg_time / i
		dest.write("%d %.7f\n" % (i, avg_time))
