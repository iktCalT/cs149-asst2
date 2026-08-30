#ifndef _TASKSYS_H
#define _TASKSYS_H

#include "itasksys.h"
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdlib>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
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
    public:
        TaskSystemParallelSpawn(int num_threads);
        ~TaskSystemParallelSpawn();
        const char* name();
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
    public:
        TaskSystemParallelThreadPoolSpinning(int num_threads);
        ~TaskSystemParallelThreadPoolSpinning();
        const char* name();
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
        
        struct Task {
            IRunnable* runnable;
            int num_total_tasks;
            const std::unordered_set<TaskID> downstream;
            std::atomic<int>* unfinished; // Number of unfinished work queues
            std::vector<WorkQueue> work_queues;
            
            Task(IRunnable* runnable, int num_total_tasks, std::unordered_set<TaskID>& downstream, int active_queue, std::vector<WorkQueue>& work_queues)
                : runnable(runnable), 
                  num_total_tasks(num_total_tasks),
                  downstream(std::move(downstream)), 
                  unfinished(new std::atomic<int>(active_queue)), 
                  work_queues(std::move(work_queues)) {}

            ~Task() {delete(unfinished);}
        };

        struct PendingTask {
            IRunnable* runnable;
            int num_total_tasks;
            std::unordered_set<TaskID> upstream;
            std::unordered_set<TaskID> downstream;

            PendingTask(IRunnable* runnable, int num_total_tasks, const std::vector<int>& upstream) 
                : runnable(runnable), num_total_tasks(num_total_tasks), upstream(upstream.begin(), upstream.end()), downstream({}) {}
        };
        
        int num_threads;
        std::vector<std::thread> workers;
        std::atomic<bool> exit{false};           // Only main thread can modify it
        
        TaskID new_task_id;                         // If new task is created, use this id. 
                                                    // Only main thread can access it
        std::unordered_map<TaskID, Task*> tasks;    // tasks only store task pointers. 
                                                    // So that when we add new task to
                                                    // tasks, other tasks won't be affected
        std::unordered_map<TaskID, PendingTask> pending_tasks;
        // std::unordered_set<TaskID> finished_tasks;
        std::atomic<int> unfinished_tasks{0};    // Number of unfinished bunch of tasks

        std::condition_variable worker_cv;          // Worker threads sleep / wakeup
        std::condition_variable main_cv;            // Main thread sleep / wakeup
        std::mutex sync_mtx;                        // Mutex that worker_cv and main_cv want to grab
        int sleep_cnt = 0;

        void workerStart(int thread_id);
        void finishWork(Task* task, int thread_id);
        void stealDoWork(Task* task, int thread_id);
        void workerSleep();
        void wakeupWorker();
        void mainSleep();
        inline void mainSleep(std::unique_lock<std::mutex>& lock);
        void wakeupMain();
        void addTask(const TaskID task_id, PendingTask& pending_task);
        void buildDependents();
        void activateInitTasks();
        void activateReadyTasks();

    public:
        TaskSystemParallelThreadPoolSleeping(int num_threads);
        ~TaskSystemParallelThreadPoolSleeping();
        const char* name();
        void run(IRunnable* runnable, int num_total_tasks);
        TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                const std::vector<TaskID>& deps);
        void sync();
};

#endif
