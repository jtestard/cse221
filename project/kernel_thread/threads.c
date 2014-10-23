#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>  // for threads
#include <linux/sched.h>  // for task_struct
#include <linux/time.h>   // for using jiffies 
#include <linux/timer.h>

// Thread structure
static struct task_struct *thread1;

// Function run by the kernel thread
int thread_fn(void) {

unsigned long j0,j1;
int delay = 60*HZ;

printk(KERN_INFO "In thread1");
j0 = jiffies;
j1 = j0 + delay;

while (time_before(jiffies, j1))
        schedule();

return 0;
}

// Create kernel thread
int thread_init (void) {
   
    char  our_thread[8]="thread1";
	void * data = NULL;
    printk(KERN_INFO "in init");
    thread1 = kthread_create(thread_fn,data,our_thread);
    if((thread1))
        {
        printk(KERN_INFO "in if");
        wake_up_process(thread1);
        }

    return 0;
}


// Delete kernel thread
void thread_cleanup(void) {
 int ret;
 ret = kthread_stop(thread1);
 if(!ret)
  printk(KERN_INFO "Thread stopped");

}

// Create module code
MODULE_LICENSE("GPL");   
module_init(thread_init);
module_exit(thread_cleanup);

