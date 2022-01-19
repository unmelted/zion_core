
/*****************************************************************************
*                                                                            *
*                             TaskManager     								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : TaskManager.hpp
    Author(S)       : Me Eunkyung
    Created         : 16 Jan 2022

    Description     : TaskManager
    Notes           : 
*/
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "CMDefine.hpp"
#include "json.hpp"
#include "MessageQueue.h"
#include "Stabilization.hpp"

namespace TaskPool {
class TaskManager {

public:
    TaskManager(size_t num_worker_);
    ~TaskManager();

    template <class F, class... Args>
    void EnqueueJob( MessageQueue<int>* fu, F&& f, Args&&... args);
    int MakeTask(int mode, shared_ptr<VIDEO_INFO> arg);

private:
    size_t num_worker;
    size_t cur_worker;
    std::vector<std::thread> worker;
    std::thread* watcher{ nullptr };
    std::queue<std::function<void()>> jobs;
    std::condition_variable cv_job;
    std::mutex m_job;
    MessageQueue<int> m_future;

    Dove* stblz;
    bool stop_all;
    bool watching;
    void WorkerThread();
    void WatchFuture();
    int RunStabilize(shared_ptr<VIDEO_INFO> arg);

};

template <class F, class... Args>
void TaskManager::EnqueueJob(MessageQueue<int>* fu, F&& f, Args&&... args) {
    if (stop_all) {
        throw std::runtime_error("Can't add job in ThreadPool");
    }
    
    using return_type = typename std::result_of<F(Args...)>::type;
    auto job = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> job_result_future = job->get_future();
    {
        std::lock_guard<std::mutex> lock(m_job);
        jobs.push([job]() { (*job)(); });
    }
    cv_job.notify_one();
    fu->Enqueue(job_result_future.get());
//    return job_result_future;
}

} //namespace
