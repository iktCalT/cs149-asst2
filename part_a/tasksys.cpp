#include "tasksys.h"
#include "itasksys.h"
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

#define DEBUG_1 0
#define DEBUG_2 0
#define DEBUG_3 0

#if DEBUG_1 || DEBUG_2 || DEBUG_3
std::mutex print_mtx;
#endif

IRunnable::~IRunnable() {}

ITaskSystem::ITaskSystem(int num_threads) {}
ITaskSystem::~ITaskSystem() {}

/*
 * ================================================================
 * Serial task system implementation
 * ================================================================
 */

const char* TaskSystemSerial::name() {
    return "Serial";
}

TaskSystemSerial::TaskSystemSerial(int num_threads): ITaskSystem(num_threads) {
}

TaskSystemSerial::~TaskSystemSerial() {}

void TaskSystemSerial::run(IRunnable* runnable, int num_total_tasks) {
    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }
}

TaskID TaskSystemSerial::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                          const std::vector<TaskID>& deps) {
    // You do not need to implement this method.
    return 0;
}

void TaskSystemSerial::sync() {
    // You do not need to implement this method.
    return;
}

/*
 * ================================================================
 * Parallel Task System Implementation
 * ================================================================
 */

const char* TaskSystemParallelSpawn::name() {
    return "Parallel + Always Spawn";
}

TaskSystemParallelSpawn::TaskSystemParallelSpawn(int num_threads): ITaskSystem(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
    this->num_threads = num_threads;
    workers = std::vector<std::thread>(num_threads);
    work_queues = std::vector<std::pair<int, int>>(num_threads);
    work_queue_locks = std::vector<std::mutex>(num_threads);
}

TaskSystemParallelSpawn::~TaskSystemParallelSpawn() {}

void TaskSystemParallelSpawn::workerStart(IRunnable* runnable, int thread_id, int num_total_tasks) {
    while (unfinished.load(std::memory_order_acquire) != 0) {
        // Finish my work
        while (work_queues[thread_id].first != work_queues[thread_id].second) {
            work_queue_locks[thread_id].lock();
            if (work_queues[thread_id].first == work_queues[thread_id].second) {
                // Incase work is stolen before locking
                work_queue_locks[thread_id].unlock();
                break;
            }

            int task_id = work_queues[thread_id].first;
            ++work_queues[thread_id].first;
            if (work_queues[thread_id].first == work_queues[thread_id].second) 
                unfinished.fetch_sub(1);
            // unlock before executing tasks
            work_queue_locks[thread_id].unlock();

            runnable->runTask(task_id, num_total_tasks);
        #if DEBUG_1
            print_mtx.lock();
            printf("Thread %d calls runTask(%d, %d)\n", thread_id, task_id, num_total_tasks);
            print_mtx.unlock();
        #endif
        }

        // Steal
        int victim = (thread_id + 1) % num_threads;
        for (int _ = 0; _ < num_threads; ++_) { // iterate at most num_threads times
            // A fast lookup: if victim's work queue is empty, choose next victim
            if (work_queues[victim].second == work_queues[victim].first) {
                victim = (victim + 1) % num_threads;
                continue;
            }

            work_queue_locks[victim].lock();
            int size = work_queues[victim].second - work_queues[victim].first;
            if (size == 0) { // In case victim's work queue is stolen before locking
                work_queue_locks[victim].unlock();
                victim = (victim + 1) % num_threads;
                continue;
            }

            // steal (size + 1) / 2 tasks. size >= 1, so steal_size >= 1
            int steal_size = (size + 1) / 2;
            int second = work_queues[victim].second;
            work_queues[victim].second -= steal_size;
            if (steal_size != size) unfinished.fetch_add(1);
        #if DEBUG_1
            print_mtx.lock();
            printf("Thread %d steals from thread %d: Victim from [%d, %d) to [%d, %d).\n",
                thread_id, victim,
                work_queues[victim].first, work_queues[victim].second + steal_size,
                work_queues[victim].first, work_queues[victim].second);
            print_mtx.unlock();
        #endif
            // Unlock before getting new lock to avoid dead lock
            work_queue_locks[victim].unlock();

            work_queue_locks[thread_id].lock();
            work_queues[thread_id].second = second;
            work_queues[thread_id].first = second - steal_size;
        #if DEBUG_1
            print_mtx.lock();
            printf("Thread %d steals from thread %d: Stealer becomes [%d, %d)\n",
                thread_id, victim,
                work_queues[thread_id].first, work_queues[thread_id].second);
            print_mtx.unlock();
        #endif
            work_queue_locks[thread_id].unlock();
            break;
        }
    }
}

void TaskSystemParallelSpawn::run(IRunnable* runnable, int num_total_tasks) {


    //
    // TODO: CS149 students will modify the implementation of this
    // method in Part A.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

    // Static allocation
    /* 
     * int tasks_per_thread = (num_total_tasks + num_threads-1) / num_threads;
     * 
     * for (int thread_id = 1; thread_id < num_threads; ++thread_id) {
     *     int start = thread_id * tasks_per_thread;
     *     if (start < num_total_tasks) {
     *         workers[thread_id] = std::thread(
     *             &TaskSystemParallelSpawn::workerStart,
     *              this, runnable, start, tasks_per_thread, num_total_tasks
     *         );
     *     }
     * }
     * 
     * workerStart(runnable, 0, tasks_per_thread, num_total_tasks);
     * 
     * for (int k = 1; k < num_threads; ++k) {
     *     if (workers[k].joinable())
     *         workers[k].join();
     * }
     */

    // Dynamic allocation, work queue
    // Instead of using a real queue as a work queue
    // We just need to record the starting and ending indices

    int tasks_per_thread = (num_total_tasks + num_threads-1) / num_threads;
    
    // Work queue is [task.first, task.second)
    int non_empty_queues = 0;
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        work_queues[thread_id].first = std::min(thread_id * tasks_per_thread, num_total_tasks);
        work_queues[thread_id].second = std::min((thread_id + 1) * tasks_per_thread, num_total_tasks);
        
        if (work_queues[thread_id].first != work_queues[thread_id].second) 
            ++non_empty_queues;
    }

    unfinished.store(non_empty_queues); 
        // Important: cannot use unfinished.store(num_threads), because some 
        // threads may not have work initially. For example, num_threads 
        // == 4, but num_total_tasks == 5. -> [2,2,1,0] -> unfinished = 3

    // Spawn threads after complete work queues
    for (int thread_id = 1; thread_id < num_threads; ++thread_id) {
        workers[thread_id] = std::thread(
            &TaskSystemParallelSpawn::workerStart,
            this, runnable, thread_id, num_total_tasks
        );
    }

    workerStart(runnable, 0, num_total_tasks);
    
    for (int k = 1; k < num_threads; ++k) {
        if (workers[k].joinable())
            workers[k].join();
    }
}

TaskID TaskSystemParallelSpawn::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                 const std::vector<TaskID>& deps) {
    // You do not need to implement this method.
    return 0;
}

void TaskSystemParallelSpawn::sync() {
    // You do not need to implement this method.
    return;
}

/*
 * ================================================================
 * Parallel Thread Pool Spinning Task System Implementation
 * ================================================================
 */

const char* TaskSystemParallelThreadPoolSpinning::name() {
    return "Parallel + Thread Pool + Spin";
}

TaskSystemParallelThreadPoolSpinning::TaskSystemParallelThreadPoolSpinning(int num_threads): ITaskSystem(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
    this->num_threads = num_threads;
    workers.reserve(num_threads);
    work_queues = std::vector<WorkQueue>(num_threads);
}

TaskSystemParallelThreadPoolSpinning::~TaskSystemParallelThreadPoolSpinning() {
    exit.store(true);
    for (int thread_id = 0; thread_id < num_threads; ++thread_id)
        if (workers[thread_id].joinable())
            workers[thread_id].join();
}

void TaskSystemParallelThreadPoolSpinning::finishWork(int thread_id) {
    // Finish my work
    if (unfinished.load(std::memory_order_acquire) == 0
        || work_queues[thread_id].start == work_queues[thread_id].end) return;

    while (true) {
        work_queues[thread_id].lock.lock();
        if (work_queues[thread_id].start == work_queues[thread_id].end) {
            // Incase work is stolen before locking
            work_queues[thread_id].lock.unlock();
            break;
        }

        int task_id = work_queues[thread_id].start++;
        if (work_queues[thread_id].start == work_queues[thread_id].end)
            unfinished.fetch_sub(1, std::memory_order_release);
        #if DEBUG_2
            print_mtx.lock();
            printf("Thread %d is running %p, task id: %d, total tasks: %d\n", 
                thread_id, runnable, task_id, num_total_tasks);
            print_mtx.unlock();
        #endif
        // unlock before executing tasks
        work_queues[thread_id].lock.unlock();

        runnable->runTask(task_id, num_total_tasks);
    }
}

// Steal work from other threads. And do one task immediately.
// In case all threads are trying to steal one task
void TaskSystemParallelThreadPoolSpinning::stealDoWork(int thread_id) {
    if (unfinished.load(std::memory_order_acquire) == 0) return;

    int victim = (thread_id + 1) % num_threads;
    for (int _ = 0; _ < num_threads; ++_) { // iterate at most num_threads times
        // A fast lookup: if victim's work queue is empty, choose next victim
        if (work_queues[victim].end == work_queues[victim].start) {
            victim = (victim + 1) % num_threads;
            continue;
        }

        // If victim's work queue size >= 1, steal (size + 1) / 2 tasks
        work_queues[victim].lock.lock();
        int size = work_queues[victim].end - work_queues[victim].start;
        if (size == 0) { // In case victim's work queue is stolen before locking
            work_queues[victim].lock.unlock();
            victim = (victim + 1) % num_threads;
            continue;
        }

        // Steal (size + 1) / 2 tasks. size >= 1, so steal_size >= 1
        int end = work_queues[victim].end;
        int start = end - (size + 1) / 2;
        work_queues[victim].end = start;
        // Unlock before getting new lock to avoid dead lock
        work_queues[victim].lock.unlock();

        // Put (steal_size - 1) tasks in work queue
        work_queues[thread_id].lock.lock();
        work_queues[thread_id].end = end;
        work_queues[thread_id].start = start + 1;
        if (size == 1) { unfinished.fetch_sub(1, std::memory_order_release); }
        else if (size == 2) {} 
        else { unfinished.fetch_add(1, std::memory_order_release); }
        work_queues[thread_id].lock.unlock();

    #if DEBUG_2
        print_mtx.lock();
        printf("Thread %d is stealing %d/%d tasks from thread %d\n\tStealer becomes [%d, %d). Victim from [%d, %d) to [%d, %d)\n", 
        thread_id, (size + 1) / 2, size, victim, 
        start, end, 
        end - size, end, 
        end - size, end - (size + 1) / 2);
        printf("\tStealer %d is running %d\n", thread_id, start);
        print_mtx.unlock();
    #endif

        // Do one work immediately after stealing
        runnable->runTask(start, num_total_tasks);
        break;
    }
}

void TaskSystemParallelThreadPoolSpinning::workerStart(int thread_id) {
    while (true) {
        if (unfinished.load(std::memory_order_acquire) == 0) {
            spin_cnt.fetch_add(1, std::memory_order_release);
            while (unfinished.load(std::memory_order_acquire) == 0) {
                if (exit.load(std::memory_order_acquire)) return;
            } // Spinning
            spin_cnt.fetch_sub(1, std::memory_order_release); // Next run (DON'T set it to 0 in run()!!!)
        }   // This is very important!!! Otherwise, when run is called again,
            // it may start stealing and lose its unfinished work!!!! I spent 
            // days on this bug!

        // Finish all my works
        TaskSystemParallelThreadPoolSpinning::finishWork(thread_id);
        // Steal work from the first unfinished runnable_task
        TaskSystemParallelThreadPoolSpinning::stealDoWork(thread_id);
    }
}

void TaskSystemParallelThreadPoolSpinning::run(IRunnable* runnable, int num_total_tasks) {

    //
    // TODO: CS149 students will modify the implementation of this
    // method in Part A.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //
#if DEBUG_2
    print_mtx.lock();
    printf("\nNew tasks: %p, total tasks: %d\n", runnable, num_total_tasks);
    print_mtx.unlock();
#endif

    int tasks_per_thread = (num_total_tasks + num_threads-1) / num_threads;
    // Work queue is [task.first, task.second)
    int active_queue = 0;
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        // std::lock_guard<std::mutex> lock(work_queues[thread_id].lock); // Important!!!
        work_queues[thread_id].start = 
            std::min(thread_id * tasks_per_thread, num_total_tasks);
        work_queues[thread_id].end =
            std::min((thread_id + 1) * tasks_per_thread, num_total_tasks);
        if (work_queues[thread_id].start != work_queues[thread_id].end) 
            ++active_queue;
    }
    this->num_total_tasks = num_total_tasks;
    this->runnable = runnable;
    unfinished.store(active_queue);
    
    // If no workers, create workers and launch tasks
    if (workers.empty())
        for (int thread_id = 0; thread_id < num_threads; ++thread_id)
            workers.emplace_back(
                &TaskSystemParallelThreadPoolSpinning::workerStart,
                this, thread_id);

    // If not finish, block the main thread
    while (unfinished.load(std::memory_order_acquire) != 0) {} // Spinning

    // Simulate join
    while (spin_cnt.load(std::memory_order_acquire) != num_threads) {}
}

TaskID TaskSystemParallelThreadPoolSpinning::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                              const std::vector<TaskID>& deps) {
    // You do not need to implement this method.
    return 0;
}

void TaskSystemParallelThreadPoolSpinning::sync() {
    // You do not need to implement this method.
    return;
}

/*
 * ================================================================
 * Parallel Thread Pool Sleeping Task System Implementation
 * ================================================================
 */

const char* TaskSystemParallelThreadPoolSleeping::name() {
    return "Parallel + Thread Pool + Sleep";
}

TaskSystemParallelThreadPoolSleeping::TaskSystemParallelThreadPoolSleeping(int num_threads): ITaskSystem(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
    this->num_threads = num_threads;
    workers.reserve(num_threads);
    work_queues = std::vector<WorkQueue>(num_threads);
}

TaskSystemParallelThreadPoolSleeping::~TaskSystemParallelThreadPoolSleeping() {
    //
    // TODO: CS149 student implementations may decide to perform cleanup
    // operations (such as thread pool shutdown construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //

#if DEBUG_3
    printf("Destructor called.\n");
#endif
    exit.store(true);
    worker_cv.notify_all();
    for (int thread_id = 0; thread_id < num_threads; ++thread_id)
        if (workers[thread_id].joinable())
            workers[thread_id].join();
}

void TaskSystemParallelThreadPoolSleeping::finishWork(int thread_id) {
    // Finish my work
    if (unfinished.load(std::memory_order_acquire) == 0
        || work_queues[thread_id].start == work_queues[thread_id].end) return;

    while (true) {
        work_queues[thread_id].lock.lock();
        if (work_queues[thread_id].start == work_queues[thread_id].end) {
            // Incase work is stolen before locking
            work_queues[thread_id].lock.unlock();
            break;
        }

        int task_id = work_queues[thread_id].start++;
        if (work_queues[thread_id].start == work_queues[thread_id].end)
            unfinished.fetch_sub(1, std::memory_order_release);
    #if DEBUG_3
        print_mtx.lock();
        printf("Thread %d is running %p, task id: %d, total tasks: %d\n", 
            thread_id, runnable, task_id, num_total_tasks);
        print_mtx.unlock();
    #endif
        // unlock before executing tasks
        work_queues[thread_id].lock.unlock();

        runnable->runTask(task_id, num_total_tasks);
    }
}

// Steal work from other threads. And do one task immediately.
// In case all threads are trying to steal one task
void TaskSystemParallelThreadPoolSleeping::stealDoWork(int thread_id) {
    if (unfinished.load(std::memory_order_acquire) == 0) return;

    int victim = (thread_id + 1) % num_threads;
    for (int _ = 0; _ < num_threads; ++_) { // iterate at most num_threads times
        // A fast lookup: if victim's work queue is empty, choose next victim
        if (work_queues[victim].end == work_queues[victim].start) {
            victim = (victim + 1) % num_threads;
            continue;
        }

        // If victim's work queue size >= 1, steal (size + 1) / 2 tasks
        work_queues[victim].lock.lock();
        int size = work_queues[victim].end - work_queues[victim].start;
        if (size == 0) { // In case victim's work queue is stolen before locking
            work_queues[victim].lock.unlock();
            victim = (victim + 1) % num_threads;
            continue;
        }

        // Steal (size + 1) / 2 tasks. size >= 1, so steal_size >= 1
        int end = work_queues[victim].end;
        int start = end - (size + 1) / 2;
        work_queues[victim].end = start;
        // Unlock before getting new lock to avoid dead lock
        work_queues[victim].lock.unlock();

        // Put (steal_size - 1) tasks in work queue
        work_queues[thread_id].lock.lock();
        work_queues[thread_id].end = end;
        work_queues[thread_id].start = start + 1;
        if (size == 1) { unfinished.fetch_sub(1, std::memory_order_release); }
        else if (size == 2) {} 
        else { unfinished.fetch_add(1, std::memory_order_release); }
        work_queues[thread_id].lock.unlock();

    #if DEBUG_3
        print_mtx.lock();
        printf("Thread %d is stealing %d/%d tasks from thread %d\n\tStealer becomes [%d, %d). Victim from [%d, %d) to [%d, %d)\n", 
        thread_id, (size + 1) / 2, size, victim, 
        start, end, 
        end - size, end, 
        end - size, end - (size + 1) / 2);
        printf("\tStealer %d is running %d\n", thread_id, start);
        print_mtx.unlock();
    #endif

        // Do one work immediately after stealing
        runnable->runTask(start, num_total_tasks);
        break;
    }
}

void TaskSystemParallelThreadPoolSleeping::sleep() {
    join_mtx.lock();
    sleep_cnt.fetch_add(1, std::memory_order_release);
    main_cv.notify_all(); // Notify main thread
    join_mtx.unlock();
#if DEBUG_3
    printf("Notifying thread starts sleeping\n");
#endif

    // Sleep
    std::unique_lock<std::mutex> lock(join_mtx);
    worker_cv.wait(lock, [this]{
        return (exit.load(std::memory_order_acquire) 
            || unfinished.load(std::memory_order_acquire)); }); 
                                            // Wait for next run

    sleep_cnt.fetch_sub(1, std::memory_order_release);
    // Sub sleep count before checking exit.
    // Otherwise, if next run starts, and this thread find exit is false.
    // But before it subtract sleep_cnt, it's hanged. And other threads 
    // enter next run and finish all tasks, quickly. exit becomes true.
    // This thread may still think that exit is false, and start
    // finishWork and stealDoWork, then sleep, never exit
}

void TaskSystemParallelThreadPoolSleeping::workerStart(int thread_id) {
    while (true) {
        // Sleep if all tasks are done
        if (unfinished.load(std::memory_order_acquire) == 0)
            TaskSystemParallelThreadPoolSleeping::sleep();
        if (exit.load(std::memory_order_acquire) == true) return;
        // Finish all my works
        TaskSystemParallelThreadPoolSleeping::finishWork(thread_id);
        // Steal work from the first unfinished runnable_task
        TaskSystemParallelThreadPoolSleeping::stealDoWork(thread_id);
    }
}

void TaskSystemParallelThreadPoolSleeping::run(IRunnable* runnable, int num_total_tasks) {


    //
    // TODO: CS149 students will modify the implementation of this
    // method in Parts A and B.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

#if DEBUG_3
    print_mtx.lock();
    printf("\nNew tasks: %p, total tasks: %d\n", runnable, num_total_tasks);
    print_mtx.unlock();
#endif

    // If no workers, create workers and launch tasks
    if (workers.empty())
        for (int thread_id = 0; thread_id < num_threads; ++thread_id)
            workers.emplace_back(
                &TaskSystemParallelThreadPoolSleeping::workerStart,
                this, thread_id);
    
    int tasks_per_thread = (num_total_tasks + num_threads-1) / num_threads;
    // Work queue is [task.first, task.second)
    int active_queue = 0;
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        // std::lock_guard<std::mutex> lock(work_queues[thread_id].lock); // Important!!!
        work_queues[thread_id].start = 
            std::min(thread_id * tasks_per_thread, num_total_tasks);
        work_queues[thread_id].end =
            std::min((thread_id + 1) * tasks_per_thread, num_total_tasks);
        if (work_queues[thread_id].start != work_queues[thread_id].end) 
            ++active_queue;
    }
    this->num_total_tasks = num_total_tasks;
    this->runnable = runnable;
    unfinished.store(active_queue);
    
    // Tasks prepared, notify all threads to start working
    worker_cv.notify_all(); // Workers start working
    
    // If not finish, block the main 
    std::unique_lock<std::mutex> lock(join_mtx);
    main_cv.wait(lock, 
        [this]{return unfinished.load(std::memory_order_acquire) == 0 &&
            sleep_cnt.load(std::memory_order_acquire) == num_threads;});
                // unfinished == 0 is necessary! Otherwise, when it start a new run, 
                // main thread may exit sleeping while other threads haven't started
#if DEBUG_3
    printf("Main thread starts reaping\n");
#endif
}

TaskID TaskSystemParallelThreadPoolSleeping::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                    const std::vector<TaskID>& deps) {


    //
    // TODO: CS149 students will implement this method in Part B.
    //

    return 0;
}

void TaskSystemParallelThreadPoolSleeping::sync() {

    //
    // TODO: CS149 students will modify the implementation of this method in Part B.
    //

    return;
}
