# Writeup

## Part A TaskSystemParallelSpawn

Initially, I wrote a inefficient version, which spawns a fixed-size thread pool and I allocate tasks to them **statically**. Meaning if workload is imbalanced, it can do nothing to help.

I then implemented a complex **dynamic-allocation** version. Each thread has its work queue. And when one thread finishes its job, it will steal half jobs from other threads. The victim thread (the thread whose work queue will be stolen) is chosen randomly.

However, it needs to create and destroy threads every time `run` is called. Which leads to huge cost in thread creation.

## Part A TaskSystemParallelThreadPoolSpinning



## Part A TaskSystemParallelThreadPoolSleeping
