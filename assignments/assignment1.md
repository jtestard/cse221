#CSE 221 Homework 1
 - **Full Name** : Jules Testard
 - **UCSD ID** : A53049930


##Question 1

Rights amplifications in operating systems

### Hydra

#### A. Protection Domain

This answer comes from the sample given by the professor : 

A protection domain in Hydra is the "local name space" (LNS). An LNS represents the current set of objects and rights to which a process has access, and those objects and rights change when a process moves from one LNS to another.

#### B. Mechanism for crossing protection domain borders

In Hydra, each procedure currently executing has its own LNS. Crossing LNS borders is done when calling another procedure and providing procedure parameters. When a procedure is called, the OS creates a new LNS for that procedure with a slot for each parameter in the procedure. 

#### C. Representation of rights

Rights in Hydra are expressed using the *capability* concept. Capabilities consists of a reference to an object and its access rights. Only the kernel can manipulate capabilities, making them impossible to forge.

#### D. Amplification of rights

The capabilities of an object are LNS-specific, meaning that when an object is passed as a parameter to a procedure, the caller and callee procedures may have different access rights on that object: 

*This implies that a calle may have greater freedom to operate on an object than the caller who passed it as a parameter, but the caller can in no way obtain that freedom for himself.* (Hydra, p.339)

#### E. Protection domain border crossing decision process

When the `CALL` mechanism is initiated, the OS decides whether the called procedure should be executed as follows :

 - It first compares the actual parameter type capabilities with that of the procedure template, and fails if there is a type mismatch.
 - It then looks at the template *check-rights* field and checks that the actual parameter rights include the rights specified in the *check-rights* field. If not, a protection failure occurs.

### Multics

#### A. Protection Domain

In Multics, protection is maintained differently for users and processes.

A user protection domain's identity is defined by the principal identifier(PI). A user can access an object only if he is in that object's *access control list*.

A process protection domain is defined by its *virtual address space*. A process can access a given physical segment only if the entry for that segment in the virtual address space's *descriptor segment* allows it. Some virtual address spaces are owned by *protected subsystems*.

#### B. Mechanism for crossing protection domain borders

The only barrier that can be crossed between protection domains in Multics is when a user process or a protected subsystem calls a procedure owned by another protected subsytem. These protected subsystems can only be entered through defined entry points called *gates*.

#### C. Representation of rights

In Multics, representation of rights is different for persistent storage and memory.

Access control lists are the Multics way to specify rights to stored objects. They consists of a list of `(PI, access mode)` tuples, where each `PI` is granted the rights specified by `Access Mode`.

Access to a (physical) memory segment is restricted by the virtual address space of the process trying to access it. The virtual adress space contains a special segment called the *descriptor segment*. Each physical segment mapped by the address space has an entry in the descriptor segment. This entry specifies what rights the process has over that segment and whether that segment belongs to a *protected subsystem*.

#### D. Amplification of rights

Amplification of rights can be done by accessing a protected subsystem. A protected subsystem has a privelege number which defines which segments it has access to. 

#### E. Protection domain border crossing decision process

The decision process for calling user procedure is not described. However, a protected subsystem A may access segments belong to another protected subsystem B if the privelege number of A is lower than that of B.

### Pilot

#### A. Protection Domain

Pilot is a single user machine. As such their ownly two protected domains; the domain of the implementation and the domain of the user:

 - The implementation domain allows access to the public and private interfaces of pilot.
 - The user domain only allows access to the public interfaces of pilot.
 
It is not mentioned what would happen if a user process attempts to open a core implementation file.

#### B. Mechanism for crossing protection domain borders

A user process can call a pilot procedure with parameters through one of its public interfaces. These parameters are then passed to the pilot implementation domain.

#### C. Representation of rights

The only way rights are represented in pilot by interfaces which define allowed interactions between modules. Their is no concept of file or process ownership, or read/write/execute access. 

Access rights seem to be significantly less important in Pilot when compared with other systems seen so far.

#### D. Amplification of rights

Apart from calling pilot procedures, there is no way or concept of "amplification of rights" for a pilot user process.

#### E. OS border crossing decision process

There is no explicit decision process done by the OS other than the suggestion that user processes should use Pilot's public interfaces.

## Question 2
Process debugging with protection and isolation.

### Part 1 
**Why must a traditional operating system like Unix explicitly provide support for process debugging?**

### Part 2
**List two distinct operations that a debugger must perform that require support from the operating system.**

### Part 3
**Because processes are protected and isolated from each other, operating systems must also provide support for communication and coordination among processes. Why can't debuggers just use the support that operating systems already provide for process communication and coordination?**

### Part 4

**Do language runtime environments like Java and Perl require operating system support for debugging programs in those languages? Why or why not?**

### Part 5

**When working on an operating system, developers also need to use a debugger on the operating system itself. Why is debugging the kernel of an operating system more challenging than debugging a user-level process? What is one option for where to run a kernel debugger?**

## Question 3

**Some of the systems we have read about and discussed use specialized hardware to facilitate their implementation. Choose one such instance, describe the hardware that was used, and what advantage it gave the system implementors and designers. What is one drawback of relying upon specialized hardware? Do we still use hardware of this form today?**

Multics is an example of such a system. Multic's protected subsystems are implemented on top of the HoneyWell 6180 computer at M.I.T. :

*A protected subsystem is a collection of procedures and data bases which are intended to be used only by calls to designated entry points, known in Multics as gates.*

The advantages of using such a system are described by the authors in the article?:


## Question 4

**Plan 9 used a three-tier storage model. The third tier, an optical write-once read-many jukebox, was used for daily dumps of the entire file system — and no data was ever deleted. Hard disk and memory were used solely as caches to data stored on the jukebox. At this point in time, their experience was that capacity was not an issue: "Technology has created storage faster than we can use it."**

**Do you think that this statement still holds today? If so, why? If not, why not? Try to incorporate how technology has changed since Plan 9 was developed (hardware and/or software), or how users have changed how and for what they use computers (workloads), in your answer. Does your answer depend on whether the users you consider are users at home, departments/enterprises, or Internet services like Google and Amazon?**

## Question 5

**Pilot made a strong and persuasive argument for tailoring the design and implementation of operating systems to personal computers. We have also seen commercial operating systems like MSDOS, Windows before NT, and "classic" MacOS tailored towards personal computers as well. Why do you think we still run multi-user timesharing systems like Unix — which the Plan 9 developers refer to as "old UNIX warhorses" — on our PCs today? (Even Android, one of the most popular operating systems for smartphones, uses Linux!) Consider, for example, the requirements we have of the systems that we use.**

