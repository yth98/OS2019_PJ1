// OS_PJ1.h : the kernel module recording execution time of child processes
// Operating System 2019, NTU CSIE
#include <linux/module.h>   // MODULE_LICENSE
//#include <linux/kernel.h>   // printk
//#include <linux/unistd.h>   // (struct)timespec
//#include <linux/fs.h>       // (struct)file_operations
#include <linux/slab.h>     // kmalloc kfree
#include <linux/uaccess.h>  // copy_from_user
#include <linux/proc_fs.h>  // proc_create remove_proc_entry

struct proc_dir_entry *myproc;
static ssize_t proc_write (struct file *file, const char __user *buffer, size_t count, loff_t *pos);
static struct file_operations proc_fops;

// doubly linked-list
struct rec_node {
    pid_t pid;
    struct timespec ST;
    struct rec_node *prev, *next;
};
struct rec_node *_nroot;
struct rec_node* node_new (void);

void child (pid_t, bool);

int init_module (void);
void cleanup_module (void);