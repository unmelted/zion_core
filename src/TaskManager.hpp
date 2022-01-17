
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
#include "Stabilization.hpp"

namespace TaskPool {
class TaskManager {

public:
    TaskManager(size_t num_worker_);
    ~TaskManager();

    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(
    F&& f, Args&&... args);
    int MakeTask(int mode, void* arg);

private:
    size_t num_worker;
    size_t cur_worker;
    std::vector<std::thread> worker;
    std::queue<std::function<void()>> jobs;
    std::condition_variable cv_job;
    std::mutex m_job;

    bool stop_all;
    void WorkerThread();

};

template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> TaskManager::EnqueueJob(F&& f, Args&&... args) {
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
    return job_result_future;
}

} //namespace
