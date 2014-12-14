# Assignment 2 CSE 221
Jules Testard

## Part 1 : GMS Design

This restriction means that any page in global memory is expendable. If a node crashes, then the contents of the memory of this node can be recreated from disk. If we allowed dirty pages in global memory, then in the events of a crash the changes on that page would be lost forever.

The other advantage is in the case where the same page exists on two nodes at the same time (shared pages). If we allow writes to that page, then if a write occured on node A, the copy of the page on node B would need to be updated. No such work is required as long as pages are clean.

Finally, if a page is clean and is sent to be written to disk by the GMS, it can be discarded instead.

 - Propagate any write made to disk to all copies of that page, to ensure all copies of the same page are identical at all times.

Needless to say, this would be very expensive for a marginal gain.






**2)** To achieve this goal of transparency, the authors are hiding :
 - The distributed nature of the computation : the operating system may execute a procedure on another workstation in the system if it deems it worthwhile, and the user doesn't have to be aware of this. A simple remote procedure call facility allows locally initialized processes to execute on remote machines.
 - The absence of disks on individual workstations : variable-sized file caches are installed on each node which allows diskless workstations to remain efficient.
 
**3)** To hide the distributed nature of the file system, for example, each node stores locally a prefix table. This table is used to determine if a file is available locally or if it has to be fetch from another node in cluster. In the latter case, the system will use the remote procedure call facility to retrieve the file and serve it to the user.

**4)** One issue arose when a locally cached version of a file was being viewed on one node and edited on another. The solution adopted by the authors was to propagate updates as soon as they happen to other copies of the file in the network.

**5)** The authors achieve their goal of transparency in their (relatively small) setting, but it seems doubtful their system would scale very far, especially if nodes are on different geographical locations. In particular, allowing simultaneous access over a file will become a much harder task.

### Section 2 : Xen

**1)** Xen is a virtualization operating system which allows multiple heterogeneous guest operating systems (GuestOS) to execute simultaneously on the same machine. Their goal is to give the user the illusion he is using the GuestOS as if Xen did not exist. As such, the author try to hide their implementation as much as possible.

**2)** Three main aspects of the operating system are hidden from the user :

 - Memory virtualization : the system is given the illusion it makes its own memory management, but memory page faults are handled by Xen, not the guest OS.
 - CPU virtualization : the GuestOS kernel is given the illusion of having direct control over process scheduling, but in reality it is itself scheduled by Xen. 
 - Device I/O virtualization : the GuestOS will be given the illusion that it can directly access I/O devices, while in reality all GuestOS accesses will go through Xen first.
 
All of these illusions are provided by the hypervisor system, which sits at the highest level of privilege (ring 0). All GuestOS kernels are relegated to ring 1 (rings are an x86 hardware concept which seggregates instruction registers and other things according to privilege level).

**3)** 
Lets take the example of how the CPU is virtualized. The GuestOS kernel itself is given a time slice by the Xen scheduler (located in the hypervisor). When that time slice is up, the hypervisor hangs the current GuestOS and transfers control to the next GuestOS, which is itself given a time slice to execute its operations. One problem which comes with this scheduling method is that GuestOSes rely on a timer to schedule the processor they themselves are running. When execution resumes in a GuestOS, relying on the machine timer would give currently running threads on that OS an inaccurate time spent value. In order to solve that problem, Xen manages an array of timers (one for each GuestOS) which records only the time that OS has spent executing.

**4)** Xen also allows exception handling virtualization by letting GuestOSes register their own exception handlers on the hardware. This allows the GuestOSes to run their exception directly rather than rely on a trap into the Xen hypervisor. One problem is that it is possible for a GuestOS to invoke an exception handler it did not yet register. This situation is never supposed to happen if the GuestOS runs directly on the machine and would lead to a crash of the entire operating system. The author solved this problem by allowing Xen to trap these "double faults", since they can only occur on GuestOS exceptions, not Xen exceptions.

**5)** The authors achieve their goal pretty well, and their virtualization techniques are still use nowadays at giant software companies such as Amazon.

### Section 3 : Grapevine




Do the following : 

 - Decribe a workload scenario in which this implementation will not be an optimization. 










Xen tries to make the exception registration process as fast as possible. Therefore, exception handling validation does not do safety checks such as verfiying that the handler code is present in memory (which could be expensive). As such, it could be the case that a guestOS invokes an exception handler which is not in memory, this is called a "double fault" (the program supposed to handle the exception faults as well). In such cases, Xen is capable of fecthing the appropriate handler for the guestOS. 

This kind of optimization can hardly go wrong with OSes the authors expect to handle, but one could imagine a malicious OS which would register an unusually high number of exception handlers which would cause double faults to occur much too often.
