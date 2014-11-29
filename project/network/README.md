##Part 1: Round Trip Time (RTT)

The following command can be run on a remote host to implement a simple echo server:
ncat -l 2000 --keep-open --exec "/bin/cat"
