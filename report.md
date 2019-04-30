# 2019 OS Project 1 - Process Scheduling
> http://rswiki.csie.org/dokuwiki/courses:107_2:project_1

## 設計 Program design
### User Space : main.c --> user.o

The main process first reads in the parameters, and then fork() out several child process at their `ready time R` with specified `time units T` and `CPU affinity`.

After each child was forked, the main process sets its `sched policy` and `sched_priority`.

### Kernel Module : OS_PJ1.c --> OS_PJ1.ko

In order to `record execution time` and `show message in kernel ring buffer`, a kernel module is needed to assist us.

When a child process starts to run, it sends a message including its pid to the module.
When the child process is about to finish, it sends another message.

The kernel module should record its start and finish time respectively, and print them together.

### How to run

    make
    sudo insmod ./OS_PJ1.ko
    sudo ./user.o

    sudo rmmod OS_PJ1

## 範例測資執行結果 Results with given test set

## 結果分析 Result analysis

## 組員貢獻 Contributions

B04201007：

B04201020：

B04201021：

B04201043：

B05902028：

B06901031：
