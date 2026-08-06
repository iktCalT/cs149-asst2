#ifndef _TASKSYS_H
#define _TASKSYS_H

#include "itasksys.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

/*
 * TaskSystemSerial: This class is the student's implementation of a
 * serial task execution engine.  See definition of ITaskSystem in
 * itasksys.h for documentation of the ITaskSystem interface.
 */
class TaskSystemSerial: public ITaskSystem {
    public:
        TaskSystemSerial(int num_threads);
        ~TaskSystemSerial();
        const char* name();
        void run(IRunnable* runnable, int num_total_tasks);
        TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                const std::vector<TaskID>& deps);
        void sync();
};

/*
 * TaskSystemParallelSpawn: This class is the student's implementation of a
 * parallel task execution engine that spawns threads in every run()
 * call.  See definition of ITaskSystem in itasksys.h for documentation
 * of the ITaskSystem interface.
 */
class TaskSystemParallelSpawn: public ITaskSystem {
    private:
        std::atomic<int> unfinished{0};
        int num_threads;
        std::vector<std::thread> workers;
        std::vector<std::pair<int, int>> work_queues;
        std::vector<std::mutex> work_queue_locks;
    public:
        TaskSystemParallelSpawn(int num_threads);
        ~TaskSystemParallelSpawn();
        const char* name();
        void workerStart(IRunnable* runnable, int thread_id, int num_total_tasks);
        void run(IRunnable* runnable, int num_total_tasks);
        TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                const std::vector<TaskID>& deps);
        void sync();
};

/*
 * TaskSystemParallelThreadPoolSpinning: This class is the student's
 * implementation of a parallel task execution engine that uses a
 * thread pool. See definition of ITaskSystem in itasksys.h for
 * documentation of the ITaskSystem interface.
 */
class TaskSystemParallelThreadPoolSpinning: public ITaskSystem { 
    private:
        struct WorkQueue {
            std::mutex lock; // Mutex cannot be moved
            int start = 0;
            int end = 0;
        };
        
        int num_threads;
        std::vector<std::thread> workers;
        std::atomic<bool> exit{false}; // Only main thread can modify it
        
        IRunnable* runnable = nullptr;
        int num_total_tasks;
        std::atomic<int> unfinished{0}; // Number of unfinished work queues
        std::vector<WorkQueue> work_queues;

        std::atomic<int> spin_cnt{0}; // To simulate join. 
                                     // This variable cannot use vector<bool>!!!
    public:
        TaskSystemParallelThreadPoolSpinning(int num_threads);
        ~TaskSystemParallelThreadPoolSpinning();
        const char* name();

        void workerStart(int thread_id);
        void finishWork(int thread_id);
        void stealDoWork(int thread_id);
        void run(IRunnable* runnable, int num_total_tasks);
        
        TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                const std::vector<TaskID>& deps);
        void sync();
};

/*
 * TaskSystemParallelThreadPoolSleeping: This class is the student's
 * optimized implementation of a parallel task execution engine that uses
 * a thread pool. See definition of ITaskSystem in
 * itasksys.h for documentation of the ITaskSystem interface.
 */
class TaskSystemParallelThreadPoolSleeping: public ITaskSystem {
    private:
        struct WorkQueue {
            std::mutex lock; // Mutex cannot be moved
            int start = 0;
            int end = 0;
        };
        
        int num_threads;
        std::vector<std::thread> workers;
        std::atomic<bool> exit{false}; // Only main thread can modify it
        
        IRunnable* runnable = nullptr;
        int num_total_tasks;
        std::atomic<int> unfinished{0}; // Number of unfinished work queues
        std::vector<WorkQueue> work_queues;

        std::condition_variable worker_cv;
        std::condition_variable main_cv;
        std::mutex join_mtx;
        std::atomic<int> sleep_cnt{0};

    public:
        TaskSystemParallelThreadPoolSleeping(int num_threads);
        ~TaskSystemParallelThreadPoolSleeping();
        const char* name();

        void workerStart(int thread_id);
        void finishWork(int thread_id);
        void stealDoWork(int thread_id);
        void sleep();
        void run(IRunnable* runnable, int num_total_tasks);
        
        TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                const std::vector<TaskID>& deps);
        void sync();
};

#endif
