// OS_PJ1.c : the kernel module recording execution time of child processes
// Operating System 2019, NTU CSIE
#include "OS_PJ1.h"

// concepts are borrowed from CSL 2018,
// use proc instead of insert a new system call to the kernel.
// Reference: https://victoryuembeddedlinux.blogspot.com/2011/04/driverproc.html
struct proc_dir_entry *myproc;

// triggered when users write some chars to /proc/ospj1_proc
static ssize_t proc_write (struct file *file, const char __user *buffer, size_t count, loff_t *pos)
{
    long int _pid;
    char* buf = kmalloc(count+1, GFP_KERNEL);
    copy_from_user(buf, (char*)buffer , count); // copy data from user space to kernel space
    buf[count] = 0;                             // give it an EOF
    kstrtol(buf+1, 10, &_pid);                  // like atoi() in user space, base=10

    //printk(KERN_INFO "[Project1] %c %d\n", (buf[0]=='F' ? 'F' : 'S'), (pid_t)_pid);
    child((pid_t)_pid, (buf[0]=='F' ? true : false));

    kfree(buf);
    return count;                               // inform system how many chars I have read.
                                                // if return 0, infinite loop will occur.
}

static struct file_operations proc_fops = {
    .write      = proc_write,
};

struct rec_node *_nroot;
struct rec_node* node_new (void) {
    struct rec_node *_node = kmalloc(sizeof(struct rec_node), GFP_KERNEL);
    _node->pid = 0; _node->prev = _node->next = 0;
    return _node;
}

// record pid and its corresponding ST/FT.
void child (pid_t pid, bool flag) {
    // flag=0: start ; flag=1: end
    struct timespec ST, FT;
    struct rec_node *_node, *_curr;
    if (!flag) {
        getnstimeofday(&ST);
        _node = node_new();
        if (!_node) {
            printk(KERN_INFO "[Project1] ST record allocation failed.\n");
            return;
        }
        _node->pid = pid; _node->ST = ST;

        // append a node
        _curr = _nroot;
        while (_curr->next) _curr = _curr->next;
        _curr->next = _node; _node->prev = _curr;

    } else {
        getnstimeofday(&FT);

        // delete the node
        _curr = _nroot;
        while (_curr->next && _curr->pid != pid) _curr = _curr->next;
        if (_curr->pid != pid) {
            printk(KERN_INFO "[Project1] pid %d has no ST record.\n", pid);
            return;
        }
        if (_curr->next) _curr->next->prev = _curr->prev;
        _curr->prev->next = _curr->next;

        ST = _curr->ST;
        kfree(_curr);
        // https://stackoverflow.com/questions/8304259
        printk(KERN_INFO "[Project1] %d %lld.%.9ld %lld.%.9ld\n", pid, (long long)ST.tv_sec, ST.tv_nsec, (long long)FT.tv_sec, FT.tv_nsec);
    }
}

int init_module (void) {
    _nroot = node_new();
    // create proc_dir_entry ospj1_proc under /proc (a.k.a. /proc/ospj1_proc),
    // with permission 0222 (write only).
    // file_operations behaviors are defined in proc_fops.
    myproc = proc_create("ospj1_proc", 0222, NULL, &proc_fops);
    return 0;
}

void cleanup_module (void) {
    remove_proc_entry("ospj1_proc", NULL);
    kfree(_nroot);
}

MODULE_LICENSE("GPL");