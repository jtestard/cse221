

Hello Professor,

My name is Jules Testard and I am one of your students for the CSE 221 course. I will be unable to attend class tomorrow and as such will deliver a written report of my readings, as mentioned in the syllabus description. Please note that text in italic is directly quoted from the papers.

Thank you very much,

Best,

Jules Testard

### A Fast File System for UNIX

##### 1. What problem does the paper address

The author are tyring to improve the UNIX file system. The UNIX file system already existing at that time, while simple and elegant, was *incapable of providing the data throughput rates that many applications require*. The authors are trying to increase the file system's data throughput while keeping disk space waste as low as possible.

##### 2. How is it different from previous work, if any

Previous improvements from the original UNIX operating system include :

 - An increase in performance by a factor of more than 2 after increasing block size from 512 bytes to 1kB.
 - Transfer rates deterioration has been addressed previously by using a background process for periodic data reorganization in a way which increases data locality.

This paper attempts to be a continuation to these related works. It introduces novel techniques to improve the performance and data locality of the system while keeping disk space waste as low as possible.

##### 3. What is the approach used to solve the problem?
 
Authors have noticed that increasing block size yields better performance. However, the current UNIX file system does not allow multiple files to live in the same block, resulting in huge disk space waste when the block size increased too much. The authors have found a solution to the problem by dividing blocks into fragments, and allowing a file to live on a single fragment (thus multiple files can live in the same block). This way, authors achieve the performance increase gained from the block size increase while keeping disk space waste at a minimum.

The author allows their file system to be configured according to the charactheristics of the underlying mass storage device (the speed of the processor, the hardware support for mass storage transfers...). The configuration information is then used to adopt the best scheme to increase data transfer rates. For example, if the underlying hardware is known to be a hard disk (in which sequential access is a lot faster than random access), the new file system tries to allocate new blocks on the same cylinder as the previous block in the same file.

##### 4. How does the paper support or otherwise justify its arguments and conclusions?

The authors implement their system and execute a performance evaluation, which demonstrates that their system is 10 times faster than a traditional UNIX file system.

##### 5. Was the paper, in your judgement, successful in addressing the problem?

The authors were successful in increasing the performance of the file system. According to the Wikipedia entry for the Unix file system, it seems that the FFS system is still being used on modern versions of Unix.

### B The Design and Implementation of a Log-Structured File 

##### 1. What problem does the paper address

The authors have noticed that while CPU speed, RAM size, disk capacity have been improving rapidly, disk access times have not. Increasingly, disk access times are becoming the bottleneck of software applications and systems. The authors want to build a file system in which disk access times are shorter.

##### 2. How is it different from previous work, if any

This paper is the first to present a log-based (rather than random-access based) file system.

##### 3. What is the approach used to solve the problem?

The authors have noticed that an increase in RAM size will mean more RAM will be available for caching, thus most file read requests will be absorbed by caching and the disk access workload will most likely be write-heavy.

In this situation, they come up with the approach of a log-based file system, in which all data written to the disk is written sequentially. This dramatically increases performance because seek operations are required much less often.

However, they get into a problem when files need to be removed "in the middle of the log". Eventually, this makes the log fragmented, and while space is available on disk, it is no longer sequentially clustered, and the whole idea of the log falls apart. To solve this problem, the authors came up with a *segment cleaning* algorithm which moves the data regularily back into block structure.

##### 4. How does the paper support or otherwise justify its arguments and conclusions?

The authors have implemented their solution and compared it with the FFS system. They considerably outperform FFS when considering writes.

##### 5. Was the paper, in your judgement, successful in addressing the problem?

The LFS filesystem wasn't as successful as the FFS over the years. One reason could be that workloads are not as write heavy to the disk as anticipated (resulting in more time spent on reads) and in particular that the LFS system's doesn't perform as well when disk utilization is high (because more segment cleanup operations are required).