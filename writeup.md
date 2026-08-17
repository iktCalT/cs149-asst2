# Writeup

## Part A TaskSystemParallelSpawn

Initially, I wrote a inefficient version, which spawns a fixed-size thread pool and I allocate tasks to them **statically**. Meaning if workload is imbalanced, it can do nothing to help.

I then implemented a complex **dynamic-allocation** version. Each thread has its work queue. And when one thread finishes its job, it will steal half jobs from other threads. The victim thread (the thread whose work queue will be stolen) is chosen randomly.

However, it needs to create and destroy threads every time `run` is called. Which leads to huge cost in thread creation.

## Part A TaskSystemParallelThreadPoolSpinning

I use two variables: `unfinished` (representing unfinished work queues. Although unfinished tasks is easier to manage, but it will slow down our program) and `spin_cnt` to simulate join process in `TaskSystemParallelSpawn`. `unfinished` is used to decide if we should start joining process. And `spin_cnt` is used to determine if joining ends.

I spent days debugging but I also learnt a lot from this task. It's my first multithread program, I met many bugs that I have never though before.

For example, when joining ends, I let main thread set `spin_cnt` to 0. However, it triggers bug! When other workers steal and finish all tasks quickly, but there is one worker just wake up. It will notice that `unfinished` is still 0. Then it falls into the spin again and will never stop.

This "Spinning" version doesn't pass all tests. But since it's a medium state, it's okay as long as the "Sleeping" version passes all tests (maybe I will improve this version later).

\[Update\] I noticed that `std::this_thread::yield();` can handover it control immediately. It can improve the performance significantly (making it even faster than sleeping version for some tasks).

```bash
# After using "yield()"
python3 ../tests/run_test_harness.py -n 16
runtasks_ref
Linux x86_64
================================================================================
Running task system grading harness... (11 total tests)
  - Detected CPU with 16 execution contexts
  - Task system configured to use at most 16 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                9.017     8.948       1.01  (OK)
[Parallel + Always Spawn]               448.536   482.213     0.93  (OK)
[Parallel + Thread Pool + Spin]         5.555     20.092      0.28  (OK)
[Parallel + Thread Pool + Sleep]        82.247    81.189      1.01  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                41.79     55.869      0.75  (OK)
[Parallel + Always Spawn]               450.692   484.355     0.93  (OK)
[Parallel + Thread Pool + Spin]         10.245    22.46       0.46  (OK)
[Parallel + Thread Pool + Sleep]        82.594    82.248      1.00  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                667.7     905.651     0.74  (OK)
[Parallel + Always Spawn]               481.12    522.167     0.92  (OK)
[Parallel + Thread Pool + Spin]         125.514   186.275     0.67  (OK)
[Parallel + Thread Pool + Sleep]        195.892   207.833     0.94  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1262.371  1284.403    0.98  (OK)
[Parallel + Always Spawn]               502.422   534.095     0.94  (OK)
[Parallel + Thread Pool + Spin]         183.001   193.101     0.95  (OK)
[Parallel + Thread Pool + Sleep]        253.645   221.963     1.14  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_linux
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                720.537   1280.183    0.56  (OK)
[Parallel + Always Spawn]               113.724   162.529     0.70  (OK)
[Parallel + Thread Pool + Spin]         101.031   154.914     0.65  (OK)
[Parallel + Thread Pool + Sleep]        103.612   148.561     0.70  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                443.706   452.44      0.98  (OK)
[Parallel + Always Spawn]               2298.129  2480.889    0.93  (OK)
[Parallel + Thread Pool + Spin]         95.47     164.463     0.58  (OK)
[Parallel + Thread Pool + Sleep]        427.461   430.13      0.99  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                443.898   452.412     0.98  (OK)
[Parallel + Always Spawn]               2324.239  2483.155    0.94  (OK)
[Parallel + Thread Pool + Spin]         128.544   168.101     0.76  (OK)
[Parallel + Thread Pool + Sleep]        421.372   420.152     1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                228.369   233.607     0.98  (OK)
[Parallel + Always Spawn]               296.95    316.735     0.94  (OK)
[Parallel + Thread Pool + Spin]         42.154    50.804      0.83  (OK)
[Parallel + Thread Pool + Sleep]        64.948    62.646      1.04  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                243.217   232.765     1.04  (OK)
[Parallel + Always Spawn]               85.489    90.497      0.94  (OK)
[Parallel + Thread Pool + Spin]         41.94     43.006      0.98  (OK)
[Parallel + Thread Pool + Sleep]        48.047    45.38       1.06  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_linux
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                256.95    452.517     0.57  (OK)
[Parallel + Always Spawn]               133.803   232.509     0.58  (OK)
[Parallel + Thread Pool + Spin]         251.124   332.252     0.76  (OK)
[Parallel + Thread Pool + Sleep]        133.437   231.215     0.58  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_linux
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                293.773   293.954     1.00  (OK)
[Parallel + Always Spawn]               21.846    21.955      1.00  (OK)
[Parallel + Thread Pool + Spin]         23.529    22.669      1.04  (OK)
[Parallel + Thread Pool + Sleep]        23.199    21.625      1.07  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : All passed Perf
[Parallel + Thread Pool + Spin]         : All passed Perf
[Parallel + Thread Pool + Sleep]        : All passed Perf
```

## Part A TaskSystemParallelThreadPoolSleeping

I use two condition variables and a mutex to synchronize. It is the first time I use condition variable. That's an interesting object in C++.

I met a unnoticeable bug and could not find out. But thanks to **Gemini**, it told me that the `rand()` function has a hidden mutex lock. So, when a lot of threads are calling `rand()`, the who program will stuck! The outputs below shows the difference between using `rand()` and `(thread_id + 1)`. This is a problem I have never thought about.

```bash
# Use "rand()"
python3 ../tests/run_test_harness.py -n 16
runtasks_ref
Linux x86_64
================================================================================
Running task system grading harness... (11 total tests)
  - Detected CPU with 16 execution contexts
  - Task system configured to use at most 16 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                9.077     9.013       1.01  (OK)
[Parallel + Always Spawn]               442.081   470.202     0.94  (OK)
[Parallel + Thread Pool + Spin]         30.291    19.381      1.56  (NOT OK)
[Parallel + Thread Pool + Sleep]        92.608    80.493      1.15  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                41.458    56.535      0.73  (OK)
[Parallel + Always Spawn]               443.117   479.637     0.92  (OK)
[Parallel + Thread Pool + Spin]         78.695    22.983      3.42  (NOT OK)
[Parallel + Thread Pool + Sleep]        86.197    82.137      1.05  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                668.437   905.927     0.74  (OK)
[Parallel + Always Spawn]               480.761   520.631     0.92  (OK)
[Parallel + Thread Pool + Spin]         264.09    186.239     1.42  (NOT OK)
[Parallel + Thread Pool + Sleep]        196.019   207.37      0.95  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1263.858  1283.767    0.98  (OK)
[Parallel + Always Spawn]               506.065   537.08      0.94  (OK)
[Parallel + Thread Pool + Spin]         358.72    191.678     1.87  (NOT OK)
[Parallel + Thread Pool + Sleep]        255.748   222.053     1.15  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_linux
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                722.674   1280.46     0.56  (OK)
[Parallel + Always Spawn]               114.673   162.928     0.70  (OK)
[Parallel + Thread Pool + Spin]         119.279   154.904     0.77  (OK)
[Parallel + Thread Pool + Sleep]        103.659   148.941     0.70  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                446.059   454.947     0.98  (OK)
[Parallel + Always Spawn]               2321.752  2491.659    0.93  (OK)
[Parallel + Thread Pool + Spin]         204.478   162.682     1.26  (NOT OK)
[Parallel + Thread Pool + Sleep]        430.142   429.149     1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                447.219   453.735     0.99  (OK)
[Parallel + Always Spawn]               2322.833  2496.027    0.93  (OK)
[Parallel + Thread Pool + Spin]         246.682   164.479     1.50  (NOT OK)
[Parallel + Thread Pool + Sleep]        423.813   422.073     1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                229.856   234.461     0.98  (OK)
[Parallel + Always Spawn]               298.546   318.779     0.94  (OK)
[Parallel + Thread Pool + Spin]         75.28     50.395      1.49  (NOT OK)
[Parallel + Thread Pool + Sleep]        67.6      64.283      1.05  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                242.283   232.644     1.04  (OK)
[Parallel + Always Spawn]               86.041    91.16       0.94  (OK)
[Parallel + Thread Pool + Spin]         69.402    43.497      1.60  (NOT OK)
[Parallel + Thread Pool + Sleep]        46.343    46.909      0.99  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_linux
```

```bash
# Using (thread_id + 1)
python3 ../tests/run_test_harness.py -n 16
runtasks_ref
Linux x86_64
================================================================================
Running task system grading harness... (11 total tests)
  - Detected CPU with 16 execution contexts
  - Task system configured to use at most 16 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                9.038     9.325       0.97  (OK)
[Parallel + Always Spawn]               447.57    482.143     0.93  (OK)
[Parallel + Thread Pool + Spin]         21.282    20.17       1.06  (OK)
[Parallel + Thread Pool + Sleep]        85.935    80.192      1.07  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                41.509    55.962      0.74  (OK)
[Parallel + Always Spawn]               440.214   482.588     0.91  (OK)
[Parallel + Thread Pool + Spin]         26.111    22.823      1.14  (OK)
[Parallel + Thread Pool + Sleep]        88.217    83.296      1.06  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                672.145   911.825     0.74  (OK)
[Parallel + Always Spawn]               481.449   523.676     0.92  (OK)
[Parallel + Thread Pool + Spin]         225.256   186.514     1.21  (NOT OK)
[Parallel + Thread Pool + Sleep]        199.274   209.396     0.95  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1268.664  1290.29     0.98  (OK)
[Parallel + Always Spawn]               503.923   547.747     0.92  (OK)
[Parallel + Thread Pool + Spin]         364.258   198.443     1.84  (NOT OK)
[Parallel + Thread Pool + Sleep]        255.156   223.059     1.14  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_linux
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                726.176   1284.518    0.57  (OK)
[Parallel + Always Spawn]               114.794   164.49      0.70  (OK)
[Parallel + Thread Pool + Spin]         119.609   156.237     0.77  (OK)
[Parallel + Thread Pool + Sleep]        103.892   149.228     0.70  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                449.413   455.205     0.99  (OK)
[Parallel + Always Spawn]               2326.915  2506.693    0.93  (OK)
[Parallel + Thread Pool + Spin]         184.552   167.466     1.10  (OK)
[Parallel + Thread Pool + Sleep]        433.245   429.935     1.01  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                448.151   455.664     0.98  (OK)
[Parallel + Always Spawn]               2347.115  2500.313    0.94  (OK)
[Parallel + Thread Pool + Spin]         462.367   171.668     2.69  (NOT OK)
[Parallel + Thread Pool + Sleep]        425.559   424.315     1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                231.962   233.886     0.99  (OK)
[Parallel + Always Spawn]               300.118   319.588     0.94  (OK)
[Parallel + Thread Pool + Spin]         69.694    51.351      1.36  (NOT OK)
[Parallel + Thread Pool + Sleep]        66.527    62.31       1.07  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                238.699   233.925     1.02  (OK)
[Parallel + Always Spawn]               85.711    91.418      0.94  (OK)
[Parallel + Thread Pool + Spin]         70.009    43.419      1.61  (NOT OK)
[Parallel + Thread Pool + Sleep]        46.634    46.742      1.00  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_linux
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                257.909   454.284     0.57  (OK)
[Parallel + Always Spawn]               134.413   232.562     0.58  (OK)
[Parallel + Thread Pool + Spin]         293.457   305.085     0.96  (OK)
[Parallel + Thread Pool + Sleep]        141.819   232.962     0.61  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_linux
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                296.163   296.454     1.00  (OK)
[Parallel + Always Spawn]               21.956    22.102      0.99  (OK)
[Parallel + Thread Pool + Spin]         23.899    22.842      1.05  (OK)
[Parallel + Thread Pool + Sleep]        24.074    21.122      1.14  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : All passed Perf
[Parallel + Thread Pool + Spin]         : Perf did not pass all tests
[Parallel + Thread Pool + Sleep]        : All passed Perf
```

## Part B

There is a bug that I spent hours to find: 

```c++
void TaskSystemParallelThreadPoolSleeping::stealDoWork(Task* task, int thread_id) {
    if (task->unfinished->load(std::memory_order_acquire) == 0) return;

    int victim = (thread_id + 1) % num_threads;
    for (int _ = 0; _ < num_threads; ++_) { // iterate at most num_threads times
        // A fast lookup: if victim's work queue is empty, choose next victim
        if (task->work_queues[victim].end == task->work_queues[victim].start) {
            victim = (victim + 1) % num_threads;
            continue;
        }

        // If victim's work queue size >= 1, steal (size + 1) / 2 tasks
        task->work_queues[victim].lock.lock();
        int size = task->work_queues[victim].end - task->work_queues[victim].start;
        if (size == 0) { // In case victim's work queue is stolen before locking
            task->work_queues[victim].lock.unlock();
            victim = (victim + 1) % num_threads;
            continue;
        }

        // Steal (size + 1) / 2 tasks. size >= 1, so steal_size >= 1
        int end = task->work_queues[victim].end;
        int start = end - (size + 1) / 2;
        task->work_queues[victim].end = start;
        
        // Unlock before getting new lock to avoid dead lock
        task->work_queues[victim].lock.unlock();

        // Put (steal_size - 1) tasks in work queue
        task->work_queues[thread_id].lock.lock();
        task->work_queues[thread_id].end = end;
        task->work_queues[thread_id].start = start + 1;
        task->work_queues[thread_id].lock.unlock();

        if (size == 1) { 
            if (task->unfinished->fetch_sub(1, std::memory_order_release) == 1)
                unfinished_tasks.fetch_sub(1, std::memory_order_release);
        }
        else if (size == 2) {} 
        else { task->unfinished->fetch_add(1, std::memory_order_release); }
        // Do one work immediately after stealing
        task->runnable->runTask(start, task->num_total_tasks);
        break;
    }
}
```

This is incorrect! Because we renew `task->unfinished` after unlock, `task->unfinished` may be less than real value can be subtracted to 0 multiple times. And every time it becomes 0, `unfinished_tasks` will subtract 1. So, even when all tasks are done, some workers may find that `unfinished_tasks < 0`, and won't go sleep. Here is how I solve it:  

1. Move it into lock
2. Make sure if we subtract `unfinished_tasks` unexpectedly, add it back.

```c++
void TaskSystemParallelThreadPoolSleeping::stealDoWork(Task* task, int thread_id) {
    if (task->unfinished->load(std::memory_order_acquire) == 0) return;

    int victim = (thread_id + 1) % num_threads;
    for (int _ = 0; _ < num_threads; ++_) { // iterate at most num_threads times
        // A fast lookup: if victim's work queue is empty, choose next victim
        if (task->work_queues[victim].end == task->work_queues[victim].start) {
            victim = (victim + 1) % num_threads;
            continue;
        }

        // If victim's work queue size >= 1, steal (size + 1) / 2 tasks
        task->work_queues[victim].lock.lock();
        int size = task->work_queues[victim].end - task->work_queues[victim].start;
        if (size == 0) { // In case victim's work queue is stolen before locking
            task->work_queues[victim].lock.unlock();
            victim = (victim + 1) % num_threads;
            continue;
        }

        // Steal (size + 1) / 2 tasks. size >= 1, so steal_size >= 1
        int end = task->work_queues[victim].end;
        int start = end - (size + 1) / 2;
        task->work_queues[victim].end = start;
        if (size == 1) { 
            if (task->unfinished->fetch_sub(1, std::memory_order_release) == 1)
                unfinished_tasks.fetch_sub(1, std::memory_order_release);
        }
        else if (size > 2) { 
            if (task->unfinished->fetch_add(1, std::memory_order_release) == 0)
                unfinished_tasks.fetch_add(1, std::memory_order_release);
        }
        // Unlock before getting new lock to avoid dead lock
        task->work_queues[victim].lock.unlock();

        // Put (steal_size - 1) tasks in work queue
        task->work_queues[thread_id].lock.lock();
        task->work_queues[thread_id].end = end;
        task->work_queues[thread_id].start = start + 1;
        task->work_queues[thread_id].lock.unlock();

    #if DEBUG_3
        printf("Thread %d is stealing %d/%d tasks from thread %d\n\tStealer becomes [%d, %d). Victim from [%d, %d) to [%d, %d)\n", 
        thread_id, (size + 1) / 2, size, victim, 
        start, end, 
        end - size, end, 
        end - size, end - (size + 1) / 2);
    #endif
    #if DEBUG_4
        printf("\tTask %p: stealer %d is running %d\n", task->runnable, thread_id, start);
    #endif

        // Do one work immediately after stealing
        task->runnable->runTask(start, task->num_total_tasks);
        break;
    }
}
```

### Result

```zsh
# MacOS + M5
python3 ../tests/run_test_harness.py -n 10 -a
runtasks_ref
Darwin arm64
================================================================================
Running task system grading harness... (22 total tests)
  - Detected CPU with 10 execution contexts
  - Task system configured to use at most 10 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.388     3.337       1.02  (OK)
[Parallel + Always Spawn]               3.357     25.208      0.13  (OK)
[Parallel + Thread Pool + Spin]         3.331     33.246      0.10  (OK)
[Parallel + Thread Pool + Sleep]        11.451    13.351      0.86  (OK)
================================================================================
Executing test: super_super_light_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.473     3.427       1.01  (OK)
[Parallel + Always Spawn]               3.455     25.758      0.13  (OK)
[Parallel + Thread Pool + Spin]         3.479     25.812      0.13  (OK)
[Parallel + Thread Pool + Sleep]        11.103    14.139      0.79  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                13.631    21.63       0.63  (OK)
[Parallel + Always Spawn]               13.592    33.521      0.41  (OK)
[Parallel + Thread Pool + Spin]         13.532    42.282      0.32  (OK)
[Parallel + Thread Pool + Sleep]        16.403    18.307      0.90  (OK)
================================================================================
Executing test: super_light_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                13.651    21.45       0.64  (OK)
[Parallel + Always Spawn]               13.646    34.674      0.39  (OK)
[Parallel + Thread Pool + Spin]         13.635    33.013      0.41  (OK)
[Parallel + Thread Pool + Sleep]        17.223    17.163      1.00  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                218.963   392.056     0.56  (OK)
[Parallel + Always Spawn]               219.106   87.143      2.51  (NOT OK)
[Parallel + Thread Pool + Spin]         219.296   83.022      2.64  (NOT OK)
[Parallel + Thread Pool + Sleep]        56.371    73.026      0.77  (OK)
================================================================================
Executing test: ping_pong_equal_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_equal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                219.019   395.934     0.55  (OK)
[Parallel + Always Spawn]               219.012   88.322      2.48  (NOT OK)
[Parallel + Thread Pool + Spin]         219.078   74.875      2.93  (NOT OK)
[Parallel + Thread Pool + Sleep]        57.799    72.761      0.79  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                399.284   470.448     0.85  (OK)
[Parallel + Always Spawn]               395.385   113.854     3.47  (NOT OK)
[Parallel + Thread Pool + Spin]         395.109   113.909     3.47  (NOT OK)
[Parallel + Thread Pool + Sleep]        88.765    101.102     0.88  (OK)
================================================================================
Executing test: ping_pong_unequal_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_unequal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                393.734   467.043     0.84  (OK)
[Parallel + Always Spawn]               394.383   114.342     3.45  (NOT OK)
[Parallel + Thread Pool + Spin]         394.824   103.234     3.82  (NOT OK)
[Parallel + Thread Pool + Sleep]        89.012    99.943      0.89  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_osx_arm
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                825.15    824.681     1.00  (OK)
[Parallel + Always Spawn]               825.225   126.613     6.52  (NOT OK)
[Parallel + Thread Pool + Spin]         824.762   131.304     6.28  (NOT OK)
[Parallel + Thread Pool + Sleep]        124.819   129.02      0.97  (OK)
================================================================================
Executing test: recursive_fibonacci_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: recursive_fibonacci_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                825.317   825.259     1.00  (OK)
[Parallel + Always Spawn]               825.036   126.055     6.55  (NOT OK)
[Parallel + Thread Pool + Spin]         824.92    127.567     6.47  (NOT OK)
[Parallel + Thread Pool + Sleep]        123.253   126.986     0.97  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                184.731   184.492     1.00  (OK)
[Parallel + Always Spawn]               183.003   149.138     1.23  (OK)
[Parallel + Thread Pool + Spin]         182.919   161.656     1.13  (OK)
[Parallel + Thread Pool + Sleep]        92.797    97.186      0.95  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                185.674   184.532     1.01  (OK)
[Parallel + Always Spawn]               183.181   151.512     1.21  (OK)
[Parallel + Thread Pool + Spin]         183.227   115.112     1.59  (NOT OK)
[Parallel + Thread Pool + Sleep]        98.463    85.782      1.15  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                185.252   185.424     1.00  (OK)
[Parallel + Always Spawn]               182.946   149.444     1.22  (OK)
[Parallel + Thread Pool + Spin]         182.935   158.723     1.15  (OK)
[Parallel + Thread Pool + Sleep]        97.015    96.236      1.01  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                193.686   185.309     1.05  (OK)
[Parallel + Always Spawn]               182.897   150.143     1.22  (OK)
[Parallel + Thread Pool + Spin]         182.839   34.446      5.31  (NOT OK)
[Parallel + Thread Pool + Sleep]        32.643    34.36       0.95  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                103.513   103.452     1.00  (OK)
[Parallel + Always Spawn]               94.41     35.604      2.65  (NOT OK)
[Parallel + Thread Pool + Spin]         94.288    36.407      2.59  (NOT OK)
[Parallel + Thread Pool + Sleep]        26.615    30.649      0.87  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fan_in_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                102.404   102.848     1.00  (OK)
[Parallel + Always Spawn]               94.285    35.989      2.62  (NOT OK)
[Parallel + Thread Pool + Spin]         94.215    19.972      4.72  (NOT OK)
[Parallel + Thread Pool + Sleep]        17.391    18.658      0.93  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                101.602   102.564     0.99  (OK)
[Parallel + Always Spawn]               93.911    21.898      4.29  (NOT OK)
[Parallel + Thread Pool + Spin]         93.772    21.866      4.29  (NOT OK)
[Parallel + Thread Pool + Sleep]        19.043    18.86       1.01  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_reduction_tree_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                102.043   102.65      0.99  (OK)
[Parallel + Always Spawn]               93.876    21.343      4.40  (NOT OK)
[Parallel + Thread Pool + Spin]         93.808    16.687      5.62  (NOT OK)
[Parallel + Thread Pool + Sleep]        16.674    16.556      1.01  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_osx_arm
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                292.8     292.716     1.00  (OK)
[Parallel + Always Spawn]               293.016   148.707     1.97  (NOT OK)
[Parallel + Thread Pool + Spin]         293.208   163.695     1.79  (NOT OK)
[Parallel + Thread Pool + Sleep]        148.562   148.542     1.00  (OK)
================================================================================
Executing test: spin_between_run_calls_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: spin_between_run_calls_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                292.837   292.936     1.00  (OK)
[Parallel + Always Spawn]               293.036   148.624     1.97  (NOT OK)
[Parallel + Thread Pool + Spin]         292.964   161.833     1.81  (NOT OK)
[Parallel + Thread Pool + Sleep]        148.466   148.461     1.00  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_osx_arm
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                227.999   230.863     0.99  (OK)
[Parallel + Always Spawn]               227.266   32.592      6.97  (NOT OK)
[Parallel + Thread Pool + Spin]         226.199   33.677      6.72  (NOT OK)
[Parallel + Thread Pool + Sleep]        32.085    32.481      0.99  (OK)
================================================================================
Executing test: mandelbrot_chunked_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: mandelbrot_chunked_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                227.494   229.153     0.99  (OK)
[Parallel + Always Spawn]               226.68    32.496      6.98  (NOT OK)
[Parallel + Thread Pool + Spin]         226.24    33.158      6.82  (NOT OK)
[Parallel + Thread Pool + Sleep]        32.088    32.614      0.98  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : Perf did not pass all tests
[Parallel + Thread Pool + Spin]         : Perf did not pass all tests
[Parallel + Thread Pool + Sleep]        : All passed Perf
```