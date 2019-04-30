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
### FIFO
#### FIFO_1
<pre>
FIFO&#x09;&#x09;|
5&#x09;&#x09;|
P1 0 500&#x09;|&#x09;P2 2466
P2 0 500&#x09;|&#x09;P1 2465
P3 0 500&#x09;|&#x09;P3 2467
P4 0 500&#x09;|&#x09;P4 2468
P5 0 500&#x09;|&#x09;P5 2469
.........................

[ 3770.087394] [Project1] 2465 1556648293.772418693 1556648295.074972999
[ 3776.369806] [Project1] 2467 1556648293.812594915 1556648301.357462488
[ 3776.403488] [Project1] 2466 1556648293.795989321 1556648301.391145299
[ 3776.406318] [Project1] 2468 1556648293.787808269 1556648301.393976649
[ 3776.412794] [Project1] 2469 1556648293.780183312 1556648301.400451478
</pre>
## 結果分析 Result analysis

## 組員貢獻 Contributions

B04201007：

B04201020：

B04201021：

B04201043：

B05902028：

B06901031：
