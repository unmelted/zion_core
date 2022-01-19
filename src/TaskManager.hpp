
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
#include "DMServer.h"
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
    void OnRcvTask(std::shared_ptr<CMD::MSG_T>pData);
    int CommandTask(int mode, shared_ptr<VIDEO_INFO> arg);

private:
    size_t num_worker;
    size_t cur_worker;
    std::vector<std::thread> worker;
    std::thread* watcher{ nullptr };
    std::queue<std::function<void()>> jobs;
    std::condition_variable cv_job;
    std::mutex m_job;
    MessageQueue<int> m_future;
    MessageQueue<std::shared_ptr<CMD::MSG_T>> m_qTMSG;    

    Dove* stblz;
    bool stop_all;
    bool watching;
    void WorkerThread();
    void WatchFuture();
    int RunStabilize(shared_ptr<VIDEO_INFO> arg);
    void MakeSendMsg(std::shared_ptr<CMD::MSG_T> ptrMsg, int result);
};

} //namespace
