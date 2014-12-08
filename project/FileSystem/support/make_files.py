import sys

if len(sys.argv) != 3:
	print "make_file requires exactly two arguments."
	sys.exit(0)
name = sys.argv[1]
count = int(sys.argv[2])
for i in xrange(count):
	filename = "%s%d" % (name, i)
	with open(filename, 'w') as f:
		for j in xrange(4096):
			f.write('12345678')
			# File size is thus 16384 bytes (16kB).
