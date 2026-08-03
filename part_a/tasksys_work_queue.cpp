#include "tasksys.h"
#include "itasksys.h"
#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#define DEBUG_1 0
#define DEBUG_2 0

IRunnable::~IRunnable() {}

ITaskSystem::ITaskSystem(int num_threads) {}
ITaskSystem::~ITaskSystem() {}

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
    workers = std::vector<std::thread>(num_threads);
    work_queues = std::vector<std::pair<int, int>>(num_threads);
    work_queue_locks = std::vector<std::mutex>(num_threads);
}

TaskSystemParallelThreadPoolSpinning::~TaskSystemParallelThreadPoolSpinning() {}

void ::TaskSystemParallelThreadPoolSpinning::workerStart(
        IRunnable* runnable, 
        int thread_id, 
        int num_total_tasks) {
    
    while (active != finished) {
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
                finished.fetch_add(1);
            // unlock before executing tasks
            work_queue_locks[thread_id].unlock();

            runnable->runTask(task_id, num_total_tasks);
        #if DEBUG_2
            printf("Thread %d calls runTask(%d, %d)\n", thread_id, task_id, num_total_tasks);
        #endif
        }

        // Steal
        int victim = rand() % num_threads;
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
            work_queues[victim].second = work_queues[victim].second - steal_size;
            active.fetch_add(1); // since size > 0, steal_size > 0, active must add 1
                // Important: add active before adding finished and unlock!
                // Otherwise, other threads may exit in advance!
            if (steal_size == size) finished.fetch_add(1);
        #if DEBUG_2
            printf("Thread %d steals from thread %d: Victim from [%d, %d) to [%d, %d). active: %d finished %d\n",
                thread_id, victim,
                work_queues[victim].first, work_queues[victim].second + steal_size,
                work_queues[victim].first, work_queues[victim].second, active.load(), finished.load());
        #endif
            // Unlock before getting new lock to avoid dead lock
            work_queue_locks[victim].unlock();

            work_queue_locks[thread_id].lock();
            work_queues[thread_id].second = second;
            work_queues[thread_id].first = work_queues[thread_id].second - steal_size;
        #if DEBUG_2
            printf("Thread %d steals from thread %d: Stealer becomes [%d, %d)\n",
                thread_id, victim,
                work_queues[thread_id].first, work_queues[thread_id].second);
        #endif
            work_queue_locks[thread_id].unlock();
            break;
        }
    }
}

void TaskSystemParallelThreadPoolSpinning::run(IRunnable* runnable, int num_total_tasks) {


    //
    // TODO: CS149 students will modify the implementation of this
    // method in Part A.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

    // Instead of using a real queue as a work queue
    // We just need to record the starting and ending indices
    active.store(std::min(num_threads, num_total_tasks)); 
        // Important: cannot use active.store(num_threads), because some 
        // threads may not have work initially. For example, num_threads 
        // == 16, but num_total_tasks == 3. active should be 3
    finished.store(0);
    int tasks_per_thread = (num_total_tasks + num_threads-1) / num_threads;
    
    // Work queue is [task.first, task.second)
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        work_queues[thread_id].first = std::min(thread_id * tasks_per_thread, num_total_tasks);
        work_queues[thread_id].second = std::min((thread_id + 1) * tasks_per_thread, num_total_tasks);
    }

    // Spawn threads after complete work queues
    for (int thread_id = 1; thread_id < num_threads; ++thread_id) {
        workers[thread_id] = std::thread(
            &TaskSystemParallelThreadPoolSpinning::workerStart,
            this, runnable, thread_id, num_total_tasks
        );
    }

    workerStart(runnable, 0, num_total_tasks);
    
    for (int k = 1; k < num_threads; ++k) {
        if (workers[k].joinable())
            workers[k].join();
    }
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