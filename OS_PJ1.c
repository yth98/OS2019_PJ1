#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>

// TODO: triggered by child process in user space
void child (pid_t pid, bool flag) {
    // flag=0: start ; flag=1: end
    struct timespec ST, FT;
    if (!flag) {
        getnstimeofday(&ST);
        // TODO: record pid and its corresponding start time ST.

    } else {
        getnstimeofday(&FT);
        // TODO: record finish time FT and find out the corresponding ST with pid.

        // https://stackoverflow.com/questions/8304259
        printk(KERN_INFO "[Project1] %d %lld.%.9ld %lld.%.9ld\n", pid, (long long)ST.tv_sec, ST.tv_nsec, (long long)FT.tv_sec, FT.tv_nsec);
    }
}

int init_module (void) {
    return 0;
}

void cleanup_module (void) {

}

MODULE_LICENSE("GPL");