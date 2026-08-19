#include "tasksys.h"
#include "itasksys.h"
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#define DEBUG_1 0
#define DEBUG_2 0
#define DEBUG_3 0
#define DEBUG_4 0

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
    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }

    return 0;
}

void TaskSystemSerial::sync() {
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
    // NOTE: CS149 students are not expected to implement TaskSystemParallelSpawn in Part B.
}

TaskSystemParallelSpawn::~TaskSystemParallelSpawn() {}

void TaskSystemParallelSpawn::run(IRunnable* runnable, int num_total_tasks) {
    // NOTE: CS149 students are not expected to implement TaskSystemParallelSpawn in Part B.
    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }
}

TaskID TaskSystemParallelSpawn::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                 const std::vector<TaskID>& deps) {
    // NOTE: CS149 students are not expected to implement TaskSystemParallelSpawn in Part B.
    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }

    return 0;
}

void TaskSystemParallelSpawn::sync() {
    // NOTE: CS149 students are not expected to implement TaskSystemParallelSpawn in Part B.
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
    // NOTE: CS149 students are not expected to implement TaskSystemParallelThreadPoolSpinning in Part B.
}

TaskSystemParallelThreadPoolSpinning::~TaskSystemParallelThreadPoolSpinning() {}

void TaskSystemParallelThreadPoolSpinning::run(IRunnable* runnable, int num_total_tasks) {
    // NOTE: CS149 students are not expected to implement TaskSystemParallelThreadPoolSpinning in Part B.
    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }
}

TaskID TaskSystemParallelThreadPoolSpinning::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                              const std::vector<TaskID>& deps) {
    // NOTE: CS149 students are not expected to implement TaskSystemParallelThreadPoolSpinning in Part B.
    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }

    return 0;
}

void TaskSystemParallelThreadPoolSpinning::sync() {
    // NOTE: CS149 students are not expected to implement TaskSystemParallelThreadPoolSpinning in Part B.
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

    for (int thread_id = 0; thread_id < num_threads; ++thread_id)
        workers.emplace_back(
            &TaskSystemParallelThreadPoolSleeping::workerStart,
            this, thread_id);
}

TaskSystemParallelThreadPoolSleeping::~TaskSystemParallelThreadPoolSleeping() {
    //
    // TODO: CS149 student implementations may decide to perform cleanup
    // operations (such as thread pool shutdown construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //

    {
        std::lock_guard<std::mutex> lock(sync_mtx);

        exit.store(true, std::memory_order_release);
        wakeupWorker();
    }

    for (int thread_id = 0; thread_id < num_threads; ++thread_id)
        if (workers[thread_id].joinable()) {
            #if DEBUG_4
                printf("Joining worker %d...\n", thread_id);
            #endif
            workers[thread_id].join();
            #if DEBUG_4
                printf("Joined worker %d...\n", thread_id);
            #endif
        }
}

void TaskSystemParallelThreadPoolSleeping::finishWork(Task* task, int thread_id) {
    while (true) {
        task->work_queues[thread_id].lock.lock();
        if (task->unfinished->load(std::memory_order_acquire) == 0 
            || task->work_queues[thread_id].start == task->work_queues[thread_id].end) {
            // Incase work is stolen before locking
            task->work_queues[thread_id].lock.unlock();
            break;
        }

        int sub_task_id = task->work_queues[thread_id].start++;
        if (task->work_queues[thread_id].start == task->work_queues[thread_id].end)
            if (task->unfinished->fetch_sub(1, std::memory_order_release) == 1)
                unfinished_tasks.fetch_sub(1, std::memory_order_release);
    #if DEBUG_1
        printf("Task %p: thread %d is running %p, sub_task_id: %d, num_total_tasks: %d\n", 
            task->runnable, thread_id, task->runnable, sub_task_id, task->num_total_tasks);
    #endif
        // unlock before executing tasks
        task->work_queues[thread_id].lock.unlock();

        task->runnable->runTask(sub_task_id, task->num_total_tasks);
    }
}

// Steal work from other threads. And do one task immediately.
// In case all threads are trying to steal one task
void TaskSystemParallelThreadPoolSleeping::stealDoWork(Task* task, int thread_id) {
    if (task->unfinished->load(std::memory_order_acquire) == 0) return;

    int victim = (thread_id + 1) % num_threads;
    for (int _ = 0; _ < num_threads; ++_) { // iterate at most num_threads times
        // A fast lookup: if victim's work queue is empty, choose next victim
        // if (task->work_queues[victim].end == task->work_queues[victim].start) {
        //     victim = (victim + 1) % num_threads;
        //     continue;
        // }

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
    #if DEBUG_1
        printf("\tTask %p: stealer %d is running %d\n", task->runnable, thread_id, start);
    #endif

        // Do one work immediately after stealing
        task->runnable->runTask(start, task->num_total_tasks);
        break;
    }
}

void TaskSystemParallelThreadPoolSleeping::workerStart(int thread_id) {
    while (true) {
        // Sleep if all tasks are done
        #if DEBUG_1
            printf("unfinished_tasks: %d\n", unfinished_tasks.load(std::memory_order_acquire));
        #endif
        if (unfinished_tasks.load(std::memory_order_acquire) == 0)
            workerSleep();

        if (exit.load(std::memory_order_acquire)) {
        #if DEBUG_4
            printf("Worker %d exiting\n", thread_id);
        #endif
            return;
        }

        // Finish all my work (from different tasks)
        for (auto[task_id, task] : tasks) {
        #if DEBUG_1
            printf("Calling finishWork(%p, %d)\n", task, thread_id);
        #endif
            finishWork(task, thread_id);
        }

        // Steal work from the first unfinished runnable_task, and finish it
        for (auto[task_id, task] : tasks) {
            stealDoWork(task, thread_id);
            finishWork(task, thread_id);
        }
    }
}

// Worker starts sleeping -> can only be called by workers
void TaskSystemParallelThreadPoolSleeping::workerSleep() {
#if DEBUG_4
    printf("Worker sleep\n");
#endif
    std::unique_lock<std::mutex> lock(sync_mtx);

    if (exit.load(std::memory_order_acquire)) return;

    ++sleep_cnt;
    if (sleep_cnt == num_threads)
        wakeupMain();

    // Sleep
    worker_cv.wait(lock, [this]{
        return exit.load(std::memory_order_acquire) == true 
            || unfinished_tasks.load(std::memory_order_acquire) != 0; }); 

    --sleep_cnt; // Awaken, subtract sleep_cnt
    // Sub sleep count before checking exit.
    // Otherwise, if next run starts, and this thread find exit is false.
    // But before it subtract sleep_cnt, it's hanged. And other threads 
    // enter next run and finish all tasks, quickly. exit becomes true.
    // This thread may still think that exit is false, and start
    // finishWork and stealDoWork, then sleep, never exit
#if DEBUG_4
    printf("Worker waken up\n");
#endif
}

// Wakeup workers -> can only be called by main thread
void TaskSystemParallelThreadPoolSleeping::wakeupWorker() {
#if DEBUG_4
    printf("Try to wakeup workers\n");
#endif
    // When tasks are prepared, notify all sleeping threads to start working
    worker_cv.notify_all(); // Workers start working
}

void TaskSystemParallelThreadPoolSleeping::run(IRunnable* runnable, int num_total_tasks) {


    //
    // TODO: CS149 students will modify the implementation of this
    // method in Parts A and B.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

#if DEBUG_2
    printf("run() called. Runnable: %p, num_total_tasks: %d\n", runnable, num_total_tasks);
#endif
    runAsyncWithDeps(runnable, num_total_tasks, {});
    sync();
}

TaskID TaskSystemParallelThreadPoolSleeping::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                    const std::vector<TaskID>& deps) {


    //
    // TODO: CS149 students will implement this method in Part B.
    //

    TaskID task_id = new_task_id++;
    // C++ 11 doesn't support try_emplace
    pending_tasks.emplace(task_id, PendingTask{runnable, num_total_tasks, deps});
#if DEBUG_2
    printf("runWithAsync() called. Runnable: %p, num_total_tasks: %d, deps: ", runnable, num_total_tasks);
    for (TaskID task_id : deps)
        printf("%d ", task_id);
    printf("\n");
#endif
    return task_id;
}

// When all workers are sleeping and waiting for work, this function is called
void TaskSystemParallelThreadPoolSleeping::addTask(
    TaskID task_id, IRunnable* runnable, int num_total_tasks) {
    
#if DEBUG_4
    printf("addTask() called. task_id: %d, runnable: %p, num_total_tasks: %d\n", task_id, runnable, num_total_tasks);
#endif
    int tasks_per_thread = (num_total_tasks + num_threads-1) / num_threads;
    // Work queue is [task.first, task.second)
    int active_queue = 0;
    std::vector<WorkQueue> work_queues(num_threads);
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        // std::lock_guard<std::mutex> lock(tasks[task_id]->work_queues[thread_id].lock); 
                // Not necessary, because this function must be run when all workers are sleeping
        work_queues[thread_id].start = 
            std::min(thread_id * tasks_per_thread, num_total_tasks);
        work_queues[thread_id].end =
            std::min((thread_id + 1) * tasks_per_thread, num_total_tasks);
        if (work_queues[thread_id].start != work_queues[thread_id].end) 
            ++active_queue;
    }

    this->tasks.emplace(task_id,
        new Task(runnable, num_total_tasks, active_queue, work_queues)); // remember to delete it
}

void TaskSystemParallelThreadPoolSleeping::mainSleep() {
    // If not finish, block the main
    std::unique_lock<std::mutex> lock(sync_mtx);
    mainSleep(lock);
}

inline void TaskSystemParallelThreadPoolSleeping::mainSleep(std::unique_lock<std::mutex>& lock) {
    // If not finish, block the main
#if DEBUG_4
    printf("Main sleep\n");
#endif
    main_cv.wait(lock, 
        [this]{
        #if DEBUG_4
            printf("Unfinished tasks: %d, non-sleeping workers: %d\n", 
                unfinished_tasks.load(std::memory_order_relaxed), 
                num_threads - sleep_cnt);
        #endif
            return unfinished_tasks.load(std::memory_order_acquire) == 0 &&
                                            sleep_cnt == num_threads;});
                // When all tasks are done and all threads are sleeping, wake up
#if DEBUG_4
    printf("Main waken up\n");
#endif
}

void TaskSystemParallelThreadPoolSleeping::wakeupMain() {
#if DEBUG_4
    printf("Try to wakeup main\n");
#endif
    main_cv.notify_one(); // Notify main thread that all threads are sleeping
}

// Sync, must be called when all threads are sleeping
void TaskSystemParallelThreadPoolSleeping::sync() {

    //
    // TODO: CS149 students will modify the implementation of this method in Part B.
    //

#if DEBUG_4
    printf("sync() called\n");
#endif
    mainSleep(); // Make sure to run sync() when all worker threads are sleeping

    while (!pending_tasks.empty()) {
        // Hold the lock, incase some workers are waken up 
        // due to stealDoWork's unfinished_tasks.fetch_add(1)
        std::unique_lock<std::mutex> lock(sync_mtx);
        mainSleep(lock);

        // All work in task is done, add new works to tasks
        for (const auto& task : tasks) {
            finished_tasks.emplace(task.first);
        }
        tasks.clear(); // Clear tasks list

        for (auto it = pending_tasks.begin(); it != pending_tasks.end();) {
            bool can_add = true;
            for (TaskID dep : it->second.deps) {
                if (finished_tasks.find(dep) == finished_tasks.end()) { // C++11 doesn't support contains()
                    can_add = false;
                    break;
                }
            }

            if (can_add) {
                addTask(it->first, it->second.runnable, it->second.num_total_tasks);
                it = pending_tasks.erase(it);
            } else {
                ++it;
            }
        }

        unfinished_tasks.store(tasks.size(), std::memory_order_release);
        lock.unlock();

        wakeupWorker(); // Tell workers that you have new work to do
    }
    mainSleep();        // Sleep until all tasks are done
}
