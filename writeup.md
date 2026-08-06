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
