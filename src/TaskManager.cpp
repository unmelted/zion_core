
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
    :num_worker(num_worker_), stop_all(false), watching(true) {
    
    cur_worker = 0;
    worker.reserve(num_worker);
    for (size_t i = 0; i < num_worker; ++i) {
        worker.emplace_back([this]() { this->WorkerThread(); });
    }

    watcher = new std::thread(&TaskManager::WatchFuture, this);
    stblz = new Dove();
}

TaskManager::~TaskManager() {
    printf(" stop all ? \n");
    stop_all = true;
    watching = false;

    cv_job.notify_all();

    for (auto& t : worker) {
        t.join();
    }

	if (watcher != nullptr)
	{
		watcher->join();
		delete watcher;
		watcher = nullptr;
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

        //std::this_thread::sleep_for(std::chrono::milliseconds(3));                

    }
}


int TaskManager::RunStabilize(shared_ptr<VIDEO_INFO> arg)
{
    int result = 0;
    printf("RunStabilize start..\n");
    printf(" swipe period size 2 %lu \n", arg->swipe_period.size());
    stblz->SetInfo(arg.get());    
    result = stblz->Process();
    printf("RunStabilize end..\n");    
    return result;
} 

int TaskManager::MakeTask(int mode, shared_ptr<VIDEO_INFO> arg) {

    if(cur_worker == TASKPOOL_SIZE)
        return CMD::TASKMANAGER_NO_MORE_WOKER;

    if(mode == CMD::POST_STABILIZATION) {
        printf(" swipe period size 1 %lu \n", arg->swipe_period.size());                
        EnqueueJob(&m_future, &TaskManager::RunStabilize, this, arg);
    }

    return CMD::ERR_NONE;
}

void TaskManager::WatchFuture() {
    int result = -1;

    while(watching) {
        if (m_future.IsQueue()) {
            auto f = m_future.Dequeue();
            printf(" result : %d \n", f);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));        
    }

}