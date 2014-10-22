##Reading commentary

### Innovative Features
The two biggest innovative features of the THE operating system are : 

 - software based memory segmentation (which is what all modern OS use today)
 - layered architecture :
    - layer 0 : scheduler
    - layer 1 : pager
    - layer 2 : communication between OS and console
    - layer 3 : manage I/O with peripherals (printers, keyboards...)
    - layer 4 : user programs
    - layer 5 : the operator (human)

### Question 1
*Dijkstra explicitly states their goals for the THE operating system. How do these goals compare to, say, Microsoft's goals for the Windows operating system? Why do we no longer build operating systems with the same goals as THE?*

THE has been designed as a single user system while modern operating systems are multi-user.
THE is designed as a batch system with a static set of processes (*The system does not cater for user programs.*)
 

