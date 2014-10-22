**Jules Testard A53044930**

# Assignment 0, CSE 221

### Question 1
**Assumption** : by privileged, we mean that an instruction should not be executed by a user (only by the OS).

 a. YES : OS control is based on the value from the timer. A malicious user could manipulate the timer in a way that never gives back control to the OS.
 b. NO : reading the clock cannot result in a malicious OS exploit.
 c. YES : this would allow a malicious user to clear memory from other programs.
 d. YES : a malicious user could turn off interrupts and enter an infinite loop in a way that the OS cannot stop that program.
 e. YES : if a user could switch to kernel mode at will, all the benefits of having privileged instructions would be lost.

### Question 2
1) Here is an implementation of the `CountDownEvent` class in Java :

	public class CountDownEvent {
		private int count;
		private bool signaled;
		private Lock lock;
		private Condition condition;
		
		public CountDownEvent (int count) throws ValueException {
			lock = new Lock();
			condition = new Condition();
			if (count > 0) {
				this.count = count;
				this.signaled = false;
			} else if (count == 0) {
				this.count = count;
				this.signaled = true;
			} else {
				throw new ValueException();
			}
		}
		
Continued on next page...
		
		public void increment() {
			lock.lock();
			if (!signaled) {
				count++;
			}
			lock.unlock();
		}

		public void decrement(){
			lock.lock();
			if (!signaled) {
				count--;
				if (count == 0) {
					condition.signal();
					signaled = true;
				}
			}
			lock.unlock();
		}
		public void wait() {
			lock.lock();
			if (!signaled) {
				condition.await();
			}
			lock.unlock();
		}
	}


2) Semaphores differ from a CountDownEvent in a least two ways : 

   - *implicit* vs *explicit* blocking : in the sense that in `CountDownEvent` threads have to explicitely call the wait method, whereas in semaphores threads may be block by calling `increment` or `decrement`.
   - `CountDownEvent` blocks when the counter is positive, whereas semaphores block when the counter is zero or less.
   
3) A `Barrier` class in Java using the `CountDownEvent`:

	public class Barrier {
		CountDownEvent event;
		
		public Barrier (int n) throws ValueException {
			event = new CountDownEvent(n);
		}
		
		public void done() {
			event.decrement();
			event.wait();
		}
	}


### Question 3

 - When the process issues the read request for page P, it will first look for that virtual address in the **TLB** (translation look aside buffer).
 - If it doesn't find the page in the TLB, it will look at the page table to find the physical page address mapping for that virtual address, if it exists.
 - If that virtual address does not have a physical address mapping, it will result in a **page fault**, which means that the page for that virtual address has to be fetched from the disk.