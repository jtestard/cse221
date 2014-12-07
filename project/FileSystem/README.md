# File system estimation and methodology

## Part 1 : File cache.

The file cache varies in size. Assuming the OS is mostly idle (no other user program being run explicitely).
We verify idleness by doing the `top` command and checking the total CPU and RAM usage is below 10%.

In this exercise, the methodology is fixed to opening and reading a file (with varying sizes) and measuring 
read I/O time. 
We expect the file cache size to grow when the size of the file increases.
We measure file cache size 

## Part 2 : File read time

### Sequential Access

#### Expectations

Sequential access of a file is when we get the most out of the disk bandwidth.
According to our vendor's specification, the maximum read throughput is 520MB/s.
We are using a trick : we are not reading off a file that we have created. Rather,
we are reading directly of from a disk partition, using a special file : `/dev/sda1`[seek\_ref].
In linux systems, `/dev/sda[x]` is the name of the special file representing disk
partition `x`. There is no reason for a disk partition to be scattered across the
disk (the way a user-generated file could be), therefore we can expect our disk access
to be truly sequential. 

#### Methodology

We read sequentially off the hard drive by issuing a `ioctl()`  

### Random Access

#### Expectations 

As seen in class, we expect random access to be a lot slow than sequential access
when accessing a file through disk. However, our machine uses a SSD, which are
known to have much better seek times than traditional hard drives.

#### Methodology

To read randomly, we also read directly from the disk partition. We use the Linux
`lseek()` command which reads at an offset of a file descriptor, as follows:
	
	numblocks = SIZE_OF_FILE;
	offset = (off64_t) numblocks * random() / RAND_MAX;
	retval = lseek64(fd, BLOCKSIZE * offset, SEEK_SET);

Where `fd` is the file seeked (in our case the disk partition), `offset` is the randomly
chosen offset. `SEEK_SET` is an option which tells the `lseek()` command to start offsetting
from the beginning of the file. 
Finally, we simulate files of varying size by changing the `numblocks` variable.

To avoid the file system cache, we use `O_DIRECT`.

To obtain the block size, we used the command : `blockdev --getbsz /dev/sda1`. We found it was
4096.


**Note :** For actual writeup, create a new report which follows the description shown
in the T.A's comments.
