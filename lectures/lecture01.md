# CSE 221 Operating Systems Lecture 1

## Organization of the semester workload
 - Readings 2 per lecture.
 - Every lecture reading questions.
 - homework assignments.
 - project (implementation).
 - final exam.

Course website is available [here](http://cseweb.ucsd.edu/classes/fa14/cse221-a/).

####First Questions asked
 - Why was that paper written?
 
## Operating System Refresher

### Concepts
 - Process Control Block : data structure the OS uses to keep track of a process.
 - Kernel Mode : Execution with full privileges of the hardware. In particular :
   - Read/write to any memory block
   - access any I/O device, read/write disk sector
   - send/read any packet.
 - User Mode : Limited privileges.

### Hardware support
Dual mode operations :

 - priveleged instructions (accessible only to the kernel).
 - limits on memory access (prevent user code from overwriting kernel).
 - timer : used to regain control from a program in a loop.
 - safe way to switch from user to kernel mode and vice versa.
 
## OS Concepts

### Virtual Memory, translation look aside buffer (TLB)
References for this text can be found [here](http://pages.cs.wisc.edu/~remzi/OSTEP/vm-tlbs.pdf).

In short, a TLB is a cache for virtual addresses. Each virtual address request is first looked upon the TLB. If the TLB contains a mapping for that address, it returns the corresponding physical address directly, without consulting the page table, which contains all of the mappings and takes much longer to search. 

## The structure of "THE" multi-programming system (E.W. Djikstra)

In the first part of the paper, Djikstra describes the process of creating the system as well as the design decisions taken along the way.

#### Storage system 
Concepts of drum and core pages are assumed. It seems that core pages need to be "dumped" in the drum to be available to other users.

Terminology:

 - memory units :
   - drum pages
   - core pages
 - information units :
   - segments (one segment fits in a page)

#### Process allocation
Djikstra introduces the concept of *sequential processes*, in which the speed at which the process happens does not matter.

#### Notes
 - The machine is meant for a single user only