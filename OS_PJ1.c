#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>

void t_unit (void) { volatile unsigned long i; for(i=0;i<1000000UL;i++); }

void child (int Y, pid_t pid) {
    struct timespec then, now;
    getnstimeofday(&then);
    while (Y--) t_unit();
    getnstimeofday(&now);
    // https://stackoverflow.com/questions/8304259
    printk(KERN_INFO "[Project1] %d %lld.%.9ld %lld.%.9ld\n", pid, (long long)then.tv_sec, then.tv_nsec, (long long)now.tv_sec, now.tv_nsec);
}

int init_module (void) {
    return 0;
}

void cleanup_module (void) {

}

MODULE_LICENSE("LGPL");