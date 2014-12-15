from subprocess import Popen,PIPE
avg_times = 0
for i in xrange(100):
	avg_times += int(Popen(['./process'],stdout=PIPE).communicate()[0][:-1])
print str(avg_times/100)
