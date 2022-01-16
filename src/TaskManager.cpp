
/*****************************************************************************
*                                                                            *
*                             TaskManager     								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : TaskManager.cpp
    Author(S)       : Me Eunkyung
    Created         : 16 Jan 2022

    Description     : TaskManager
    Notes           : 
*/

#include "TaskManager.hpp"

using namespace TaskPool;

TaskManager::TaskManager(size_t num_worker_) 
    :num_worker(num_worker_), stop_all(false) {
    
    worker.reserve(num_worker);
    for (size_t i = 0; i < num_worker; ++i) {
        worker.emplace_back([this]() { this->WorkerThread(); });
    }

}

TaskManager::~TaskManager() {
    stop_all = true;
    cv_job.notify_all();

    for (auto& t : worker) {
        t.join();
    }
}

void TaskManager::WorkerThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(m_job);
        cv_job.wait(lock, [this]() { return !this->jobs.empty() || stop_all; });
        if (stop_all && this->jobs.empty()) {
            return;
        }

        std::function<void()> job = std::move(jobs.front());
        jobs.pop();
        lock.unlock();

        job();
    }
}
