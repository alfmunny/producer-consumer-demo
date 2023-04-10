Auther: Yuanchen Zhang

GitHub: https://github.com/alfmunny

# Simple Producer-Consumer Demo

It is a simple producer and consumer demostration.

## Architect

- Use a queue as middleware for synchronize between producer and consumer
- Different kinds of queue should implement the interface `queue_intf.h` for exetension
- Two types of queue are implemented:
    - Synchronization with Lock
    - Synchronization without Lock (CAS support)
- Producer gathers the data and enqueue it to queue
- Consumer should get the data from queue, filter it with a customizable function and send it out
- They can run in different speeds
- Advanced topics (Not implemented):
  - If the producer and consumer and the queue should be distributed, or consumer has heavy IO operations while performing `send_data`, consider use a IO coroutine framework (Please check my library: https://github.com/alfmunny/EVA01)
    - Coroutine with a thread pool to handle the IO operations.
    - Lightweight Coroutine implemented with Event-based (epoll) user-space context switch.
    - All the socket family blocking IO needs to be hooked, bend them into coroutine.
    - Then we schedule a ton of IO events in each thread.
    - Coding the asychrounous behavior in a synchronous non-blocking manner.

## Structure

```
- include (headers)
    - data_frame.h (data structure)
    - queue_intf.h (interface for different queue implementation)
    - blocking_queue.h (implementation of a blocking queue)
    - spsc_queue.h (implementation of a lockfree queue (CAS queue from Boost))
    - producer.h (producer header)
    - consumer.h (consumer header)
- src (source code)
    - producer.cpp
    - consumer.cpp
- demo_lock.cpp (demo with a queue with lock)
- demo_lockfree.cpp (demo with a lockfree queue)
```

## Build

- Linux and ARM 64bit
- gcc-9
- cmake 3.22
- Boost

Start building:

```bash
cmake . 
make
```

## Run

See help

```bash
bin/demo_lock

Usage: provide interval(ms) for producer and time span for running(s)

```

```bash
bin/demo_lock 100 10
```


## Performace

Some performace reports:

```
Performance counter stats for 'bin/demo_lock 1 10':

         15,548.74 msec task-clock                #    1.548 CPUs utilized
             4,154      context-switches          #  267.160 /sec
                11      cpu-migrations            #    0.707 /sec
            32,918      page-faults               #    2.117 K/sec
    40,102,937,095      cycles                    #    2.579 GHz
     9,156,295,604      instructions              #    0.23  insn per cycle
       256,688,115      branches                  #   16.509 M/sec
         1,603,503      branch-misses             #    0.62% of all branches

      10.042439299 seconds time elapsed

      15.401480000 seconds user
       0.152450000 seconds sys

 Performance counter stats for 'bin/demo_lockfree 1 10':

         14,002.65 msec task-clock                #    1.304 CPUs utilized
             3,215      context-switches          #  229.599 /sec
                10      cpu-migrations            #    0.714 /sec
         1,606,067      page-faults               #  114.697 K/sec
    43,463,802,600      cycles                    #    3.104 GHz
    20,795,442,392      instructions              #    0.48  insn per cycle
     2,905,094,559      branches                  #  207.467 M/sec
         3,545,780      branch-misses             #    0.12% of all branches

      10.738615598 seconds time elapsed

      10.948569000 seconds user
       3.076555000 seconds sys

Performance counter stats for 'bin/demo_lock 5 10':

         15,541.83 msec task-clock                #    1.089 CPUs utilized
             3,622      context-switches          #  233.049 /sec
                 9      cpu-migrations            #    0.579 /sec
         2,441,561      page-faults               #  157.096 K/sec
    55,462,293,312      cycles                    #    3.569 GHz
    40,608,646,179      instructions              #    0.73  insn per cycle
     7,774,472,524      branches                  #  500.229 M/sec
         5,352,204      branch-misses             #    0.07% of all branches

      14.265381303 seconds time elapsed

      11.648568000 seconds user
       3.923009000 seconds sys

Performance counter stats for 'bin/demo_lockfree 5 10':

         12,591.41 msec task-clock                #    1.252 CPUs utilized
             3,026      context-switches          #  240.322 /sec
                12      cpu-migrations            #    0.953 /sec
         1,380,796      page-faults               #  109.662 K/sec
    35,484,150,051      cycles                    #    2.818 GHz
    12,950,253,704      instructions              #    0.36  insn per cycle
     1,337,733,897      branches                  #  106.242 M/sec
         3,362,496      branch-misses             #    0.25% of all branches

      10.057319784 seconds time elapsed

       9.819094000 seconds user
       2.809463000 seconds sys
```
