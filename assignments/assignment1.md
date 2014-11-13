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

In Hydra, each procedure currently executing has its own LNS. Crossing LNS borders is done when calling another procedure and providing procedure parameters. When a procedure is called, the OS creates a new LNS for that procedure with a slot for each parameter in the procedure. Same when a method returns.

#### C. Representation of rights

Rights in Hydra are expressed using the *capability* concept. Capabilities consists of a reference to an object and its access rights. Only the kernel can manipulate capabilities, making them impossible to forge.

#### D. Amplification of rights

The capabilities of an object are LNS-specific, meaning that when an object is passed as a parameter to a procedure, the caller and callee procedures may have different access rights on that object. This is done by merging the procedure template's rights with the caller's rights, as described below : 

*[...] A capability is constructed in the (new) LNS which references the object by merging the caller's right's with the rights specified in the template. This implies that a callee may have greater freedom to operate on an object than the caller who passed it as a parameter, but the caller can in no way obtain that freedom for himself.* (Hydra, p.339)

#### E. Protection domain border crossing decision process

When the `CALL` mechanism is initiated, the OS decides whether the called procedure should be executed as follows :

 - It first compares the actual parameter type capabilities with that of the procedure template, and fails if there is a type mismatch.   
 - It then looks at the template *check-rights* field and checks that the actual parameter rights include the rights specified in the *check-rights* field. If not, a protection failure occurs.

### Multics

#### A. Protection Domain

In Multics, protection is maintained differently for users and processes.

**protected subsystem**.

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

Pilot is a single user machine. As such there are ownly two domains; the domain of the implementation and the domain of the user:

 - The implementation domain allows access to the public and private interfaces of pilot.
 - The user domain only allows access to the public interfaces of pilot.
 
It is not mentioned what would happen if a user process attempts to open a core implementation file. Therefore we cannot really say that those two domains are protected. We could thus say that pilot has a single *protected* domain.

#### B. Mechanism for crossing protection domain borders

Since there is only one protection domain, the only *border crossing* occuring happens when accessing an external device (screen, mouse, printer...).

#### C. Representation of rights
I
The only way rights are represented in pilot by interfaces which define allowed interactions between modules. Their is no concept of file or process ownership, or read/write/execute access. 

Access rights seem to be significantly less important in Pilot when compared with other systems seen so far.

#### D. Amplification of rights

Apart from calling pilot procedures (which themselves can use private interface methods
), there is no way or concept of "amplification of rights" for a pilot user process.

#### E. OS border crossing decision process

There is no explicit decision process done by the OS other than the suggestion that user processes should use Pilot's public interfaces.

## Question 2
Process debugging with protection and isolation.

### Part 1 & 2
**Why must a traditional operating system like Unix explicitly provides support for process debugging? List two distinct operations that a debugger must perform that require support from the operating system.**

A debugger provides an environment in which the user can freeze and inspect the execution of his program while it is executing. However, in order for the user to have access to such an environment, three things must happen :

 - A complete isolation from other programs (otherwise, otherwise other programs may interfere with the user's inspection).
 - A way to freeze the execution of a process. 
 - Another program (the debugger) must be able to inspect the data of the debugged program, thus access it's memory.

All three of these actions required operating system support. As such, debugging would not be possible without help from the OS.

### Part 3
**Because processes are protected and isolated from each other, operating systems must also provide support for communication and coordination among processes. Why can't debuggers just use the support that operating systems already provide for process communication and coordination?**

In order to provide the aforementioned isolation, the debugger must stop contact with the regular means of communication and coordination. The only communication allowed is one that is controlled and monitored by the debugger, thus it has to happen through specialized communication channels.

### Part 4

**Do language runtime environments like Java and Perl require operating system support for debugging programs in those languages? Why or why not?**

Language runtimes such as Java do not require OS support, because they provide their own. Java executes using its own bytecode using its own virtual machine (the JVM). As such, Java debugging tools must be provided in Java bytecode and be interpreted by the JVM.

### Part 5

**When working on an operating system, developers also need to use a debugger on the operating system itself. Why is debugging the kernel of an operating system more challenging than debugging a user-level process? What is one option for where to run a kernel debugger?**

When debugging the kernel of an operating system, one cannot rely on the debugging tools provided by the kernel itself (as opposed to a user process). One option is to do what was done in Pilot : have a system call to freeze the main OS and start a new debug-only OS (such as CoPilot) which can then consider the main OS as just another user process).

## Question 3

**Some of the systems we have read about and discussed use specialized hardware to facilitate their implementation. Choose one such instance, describe the hardware that was used, and what advantage it gave the system implementors and designers. What is one drawback of relying upon specialized hardware? Do we still use hardware of this form today?**

Multics is an example of such a system. Multic's protected subsystems are implemented on top of the HoneyWell 6180 computer at M.I.T. :

*A protected subsystem is a collection of procedures and data bases which are intended to be used only by calls to designated entry points, known in Multics as gates. [p. 396]*

The advantages of using such a system are described by the authors in the article:

 - It allows expressing limited access to programs : *"construct proprietary programs which cannot be read, data base managers which return only statics rather than raw data to some callers, and debugging tools which cannot be accidentally disabled. (p. 396)"*.
 - There is no overhead in execution time, thus programmers don't need to take into account the use of protected subsystems when designing for performance.
 - It is possible for users to create their own.

The main drawback of relying on specialized hardware is portability. It is not possible to run an operating system like Multics on a machine that does not include Multics specialized hardware. It also means that changes to the operating system may have to be accompanied by changes to hardware, which means the operating system may not be updated without making changes to or replacing the machine.

While this was not seen as a problem when Multics was first built, we nowadays have such a variety of machines and devices that relying on specialized hardware is a lot more constraining. Moreover, the specialized hardware used by Multics is very expensive. As such, the use of specialized hardware nowadays is very limited.

That being said, we still use specialized hardware to implement some algorithms we believe are "good enough", and which all modern operating systems use. An example of such of virtual memory. In most operating systems, at least some part of the virtual memory implementation (page table, caches...) is done in hardware.

## Question 4

**Plan 9 used a three-tier storage model. The third tier, an optical write-once read-many jukebox, was used for daily dumps of the entire file system — and no data was ever deleted. Hard disk and memory were used solely as caches to data stored on the jukebox. At this point in time, their experience was that capacity was not an issue: "Technology has created storage faster than we can use it."**

**Do you think that this statement still holds today? If so, why? If not, why not? Try to incorporate how technology has changed since Plan 9 was developed (hardware and/or software), or how users have changed how and for what they use computers (workloads), in your answer. Does your answer depend on whether the users you consider are users at home, departments/enterprises, or Internet services like Google and Amazon?**

The statement (as quoted) is still true, but the problematic surrounding it has evolved.

In the days of plan 9, all computers (personal computers, mainframes, computer labs...) had one point in common : their data was stored locally. The data was either stored in a central location, or was distributed over a local area network. 

Nowadays in the age of the Internet, the statement is no longer true. Home users use cloud based web services such as Google Docs for their daily work or iCloud for their pictures from last week-end's night out. Entreprises use Jobvite for their recruting, MailChimp for their newsletter, Amazon Web Services for their databases and more. Internet services themselves use other internet services to support their service. Data is global, and jumps from one spot to another before reaching your smartphone's screen.

Today, anybody can buy a 3TB hard drive for 100$ (price found on Amazon). For most personal computer users, this is more than they will ever need. For a company, this is little more than engineer's hourly wage. More than ever, *storage* has been cheap and easy to find. However, *delivery* is another matter. The quoted sentence could be modernized as: 

*"Technology has created storage faster than we can use it, but not bandwith."*

An average internet user does not expect a web page to load in more than a second. In this second, all of the following must happen : 

 - The user's request must be processed by his browser, sent over the network to his local router, sent from there to his Internet Service Provider and eventually reach the web service on which the user is trying to access.
 - The web service will queue his request and then handle it as soon as it can.  Mostly likely, the data the user seeks is located on some web storage service (such as Amazon DynamoDB) on some cluster located halfway around the country. The web service will then make a request to the web storage service to obtain that data.
 - The web storage service will again queue this request and handle it as soon as possible, and finally will have to make some disk access to find the requested data. 
 - Once the data is fetched from the disk of the web storage service, it has to travel back the way it came from before being rendered by the user's browser.
 
That is a lot of travelling and processing for just one second of expected latency, and bottelnecks can arise for a multitude of reasons (limited bandwith, limited disk access, request saturation, missing/outdated data...). Fortunately, caches can be used at multiple levels to alleviate some of the incurred latency cost, but it also constrains how much data can be accessed at once. This is why internet requests usually only involve small dataloads, as to not constrain servers or create bottlenecks in the described process.

I will not delve much further (I could talk about this topic for hours), but this question is a major one of the bigger modern problem we like to call Big Data (and this is part of my research). By this point, the reader can guess that this question has peaked my interest more than the others have.

## Question 5

**Pilot made a strong and persuasive argument for tailoring the design and implementation of operating systems to personal computers. We have also seen commercial operating systems like MSDOS, Windows before NT, and "classic" MacOS tailored towards personal computers as well. Why do you think we still run multi-user timesharing systems like Unix — which the Plan 9 developers refer to as "old UNIX warhorses" — on our PCs today? (Even Android, one of the most popular operating systems for smartphones, uses Linux!) Consider, for example, the requirements we have of the systems that we use.**

The very short answer : for portability. Simply put, all of our machines (whatever their purpose may be) can run the same (or a variant) on the same operating system. The fact that UNIX has been designed for time sharing  machines doesn't mean it can't be used for personal computers or even smartphones. This means that :

 - Implementation of operating systems on new machines is now easier (just plugin a variant of UNIX and you are done).
 - Technical expertise is more concentrated (only need to understand how UNIX works and you know how almost any machine's operating system works).
 
On the other side, an operating system like pilot is completely unusable in a multi-user environment (or any environment which is not a single user personal computer environment).

The slightly longer answer is that while modern operating systems are **based** on old warhorses like UNIX, they have evolved a lot since then as well. Modern operating systems have kept the core of UNIX (multiple users, processes, fork...), but have added a lot of features tailored for the use of the machines on which they are run.
