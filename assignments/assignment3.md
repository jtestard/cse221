# Assignment 3 CSE 221
Jules Testard

## Section 1 : Eraser

### Part 1 
**What will the lockset algorithm conclude about how x, y and z are protected? Will it report one or more races? If not, explain why not. If so, explain what they will be and why they will be reported.**

In this question, we assume that a new thread t1 starts executing the code fragment (not the main thread). We report in the following table the state of each variable, the locks held set, and the set of candidates lock of each variable after each instruction in the fragment. Empty cells in the table contain the same value as the cell above them.

| Program    | State E=exclusive SM=SharedModified | locks\_held(t) | C(x)  | C(y)  | C(z)  |
|------------|-------------------------------------|----------------|-------|-------|-------|
|            | {x = E, y = E, z = E}               | {}             | {a,b} | {a,b} | {a,b} |
| lock(a)    |                                     | {a}            |       |       |       |
| y := x + 1 | {x = SM, y = SM, z = E}             |                | {a}   | {a}   |       |
| lock(b)    |                                     | {a,b}          |       |       |       |
| z = y+1    | {x = SM, y = SM, z = SM}            |                |       |       |       |
| y = x+1    |                                     |                |       |       |       |
| unlock(a)  |                                     | {b}            |       |       |       |
| unlock(b)  |                                     | {}             |       |       |       |

The lockset algorithm concludes that all variables are protected at all times. It will not report any races, because the set of candidate locks is non-empty for all variables at all times.

### Part 2
**We add a new code sequence, that is executed by some other thread on the same program (not necessarily by the thread from the question above). What will the lockset algorithm conclude about how x, y and z are protected? Will it report one or more races? If not, explain why not. If so, explain what they will be and why they will be reported.**

In this question, we assume a new thread t2 starts executing the code fragment (t2 different from  t1). We consider that the previous code fragment has already been executed by t1, therefore the state and set of candidate locks of each variable is inherited from the above.

| Program   | State E=exclusive SM=SharedModified | locks\_held(t2) | C(x) | C(y) | C(z)  |
|-----------|-------------------------------------|-----------------|------|------|-------|
|           | {x = SM, y = SM, z = SM}            | {}              | {a}  | {a}  | {a,b} |
| lock(b)   |                                     | {b}             |      |      |       |
| lock(a)   |                                     | {a,b}           |      |      |       |
| y = x -1  |                                     |                 |      |      |       |
| unlock(b) |                                     | {a}             |      |      |       |
| z = z +1  |                                     |                 |      |      | {a}   |
| unlock(a) |                                     | {}              |      |      |       |

The lockset algorithm concludes that all variables are protected at all times. It will not report any races, because the set of candidate locks is non-empty for all variables at all times.

### Part 3
**Aside from data races the Eraser paper briefly mentions how a similar technique might be used to detect deadlock. Explain how such a scheme could detect a deadlock in the previous code examples (be specific about what the deadlock is).**

The program run by t1 locks a then b (before releasing any locks). The program run by t2 locks b then a in the same fashion. If t1 locks a and t2 locks b immediately afterwards, the deadlock would be caused.

Lets assume locks are sorted in lexicographic order. Then, according to the discipline proposed in the paper, then the program executed by t2 would generate a warning when execute, because lock b is acquired before lock a. 

## Section 2 : Schedular Activations

**The Scheduler Activations paper states that deadlock is potentially an issue when activations perform an upcall :**

*One issue we have not yet addressed is that a user-level thread could be executing in a critical section at the instant when it is blocked or preempted...[a] possible ill effect ... [is] deadlock (e.g., the preempted thread could be holding a lock on the user-level thread ready list; if so, deadlock would occur if the upcall attempted to place the preempted thread onto the ready list)." (p. 102)*

**Why is this not a concern with standard kernel threads, i.e., why do scheduler activations have to worry about this deadlock issue, but standard kernel threads implementations do not have to?**

For traditional threads, the thread table (ready list in my understanding), is managed by the kernel. As such, the kernel would know what to do to avoid a deadlock situation because it knows if there is a lock on the table and who holds it. On the other hand, in the scheduler activations model, the thread table is implemented in user space (see figure 1 of the paper). The kernel is still responsible for sending scheduler activations and preempting threads, yet it doesn’t know which thread has a lock on the table, because it doesn't see it; it's in user space. Thus, it is possible the kernel decides to pre-empt a thread which has a lock on the table and cause deadlock.

## Section 3 : LFS

### Part 1
**Explain what are the assumptions about hardware and access patterns that LFS depends on**


 1. In LFS, the hardware is assumed to be a hard disk. As such, reading/writing blocks sequentially is assumed to be much faster than reading blocks at random locations. 
 - Moreover, the LFS authors notice that RAM sizes (thus file cache sizes) have increased at a much faster rate than disk bandwidth. As a result, the authors anticipate that most read requests will affect data stored on cache, leaving the disk workload to be primarily write-based. Thus, in order to achieve the best possible performance, write performance should be privileged at the possible detriment of read performance.
 - In addition, the LFS authors notice that bigger cache sizes also allow write buffering (accumulating write requests in a buffer). Given that writes do not have to be sent to disk immediately, a strategy to position them efficiently on disk can be put in motion.
 - Finally, the authors notice that not all files are accessed with the same frequency. A minority of files (hot files) are read/written much more often than the majority (cold files). 

### Part 2
**Explain which mechanisms in LFS depend on each assumption**

We consider here three major mechanisms of the LFS : 

1. **The writing mechanism for LFS** is behind the fundamental idea of a log structured file system. Given the hard disk assumption, it is inconvenient to write to random locations on the file system. Instead, LFS chooses to write all of the incoming changes sequentially, even if they correspond to unrelated files. Moreover, all of the changes are done in one write, which is possible given the assumed large size of the write buffer.
2. **The reading mechanism for LFS** is more similar to that of traditional file systems (such as FFS). An inode data structure contains pointers to the file's disk blocks. However, inodes need to be retrieved somehow, because that how no longer stored on a fix location (as in FFS), but written like everything else at the end of the log. Therefore, an additional layer of indirection (given by an inode map) is required. This makes reads slightly slower or just as good as FFS, which is not such a big problem given the workload isn't expected to be read heavy.
3. **The cleaning mechanism for LFS** is required to guarantee free space to be constantly available in the disk and to avoid fragmentation of the log. This is done by loading into memory (fragmented) segments of data and write them (sequentially) back to disk. The read segments are now marked as free. This is made possible by the large file cache assumption, allowing to load the segments in memory. The authors thread very carefully with the loaded segments because some of the data could be "live" (about to be edited). Given the hot/cold assumption, by focus on moving around cold files, the problem of having updates on segments being cleaned happens less often.

### Part 3
**At the time the LFS paper was written, non-volatile RAM (NVRAM) was not commonly available. If disks were replaced with NVRAM entirely, would the LFS design still make sense? Explain why or why not and be specific in justifying your answer.**

The seek time for data in NVRAM is no longer limited by the movement of a mechanical arm. This makes random accesses much faster and reading sequentially less important. The LFS gets its write performance from the fact that all of its writes are sequential. LFS's write performance dominance will be crippled because its performance will not increase as much as that of other file systems such as FFS. The drawback of having to clean segments, however, will remain. As a result, the design is likely to become less attractive in such an environment.

### Part 4
**In real-life the cost per byte of disk is likely to be far cheaper than NVRAM for some time. So instead, consider a situation where some NVRAM is available (e.g., 1/10th of the disk size). This NVRAM might be used for caching reads, caching writes, or storing particular meta-data. Argue which use might be most appropriate for improving the performance of LFS.** 

Managing metadata in LFS is currently cumbersome. An extra layer of indirection (the inode map) is required. Whenever a file is updated, this map must be update as well. The inode is not appended to the log like other writes, because it must be easy to find. Having the inode map located in NVRAM would benefit both read and write performance.

Another use of the NVRAM could be read caching. Reading data is where LFS does not perform well, an extra layer of caching could only be beneficial to the overall performance. 











 