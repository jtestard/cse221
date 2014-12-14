from subprocess import Popen,PIPE
avg_times = 0
for i in xrange(1000):
	avg_times += int(Popen(['./sys_call'],stdout=PIPE).communicate()[0][:-1])
print str(avg_times/1000)
