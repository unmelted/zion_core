
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
    
    cur_worker = 0;
    worker.reserve(num_worker);
    for (size_t i = 0; i < num_worker; ++i) {
        worker.emplace_back([this]() { this->WorkerThread(); });
    }

    stblz = new Dove();
}

TaskManager::~TaskManager() {
    printf(" stop all ? \n");
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

        //std::this_thread::sleep_for(std::chrono::milliseconds(3));                

    }
}


void TaskManager::RunStabilize(shared_ptr<VIDEO_INFO> arg)
{
    printf("RunStabilize start..\n");
    stblz->SetInfo(arg.get());    
    stblz->Process();
    //stblz->NewTest();
    printf("RunStabilize end..\n");    
} 
/*
void TaskManager::RunStabilize(int a, VIDEO_INFO* info)
{
    printf("test  start..\n");
    stblz->SetInfo(info);
    stblz->NewTest();
//    std::this_thread::sleep_for(std::chrono::seconds(a));    
    printf("test  %d  end..\n", a);    
} */


int TaskManager::MakeTask(int mode, shared_ptr<VIDEO_INFO> arg) {

    if(cur_worker == TASKPOOL_SIZE)
        return CMD::TASKMANAGER_NO_MORE_WOKER;

    if(mode == CMD::POST_STABILIZATION) {
        EnqueueJob(&TaskManager::RunStabilize, this, arg);
        //RunStabilize(arg);
        //cur_worker++;

    }

    return CMD::ERR_NONE;
}
