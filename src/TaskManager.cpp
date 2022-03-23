
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
#include "MessageManager.hpp"
#include "ExpUtil.hpp"

using namespace TaskPool;
using namespace rapidjson;

TaskManager::TaskManager(size_t num_worker_, MsgManager* a) 
    :num_worker(num_worker_), stop_all(false), watching(true) {
    
    m_msgmanager = a;
    cur_worker = 0;
    worker.reserve(num_worker);
    for (size_t i = 0; i < num_worker; ++i) {
        worker.emplace_back([this]() { this->WorkerThread(); });
    }

    watcher = new std::thread(&TaskManager::WatchFuture, this);
}

TaskManager::~TaskManager() {
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
    CMd_DEBUG("TaskManager Destroyed Done.");
}

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

int TaskManager::RunStabilize(shared_ptr<VIDEO_INFO> arg)
{
    int result = -1;
    CMd_DEBUG(" Stabil Swipe period size {}", arg->swipe_period.size());
    unique_ptr<Dove> stblz(new Dove());
    stblz->SetInfo(arg.get());    
    result = stblz->Process();
    CMd_INFO("Run stabilize Done. Result : {}" , result);
    return result;
} 

int TaskManager::CommandTask(int mode, std::string arg) {

    if(cur_worker == num_worker)
        CMd_DEBUG("CMd Job Queue is fool. working worker + job = : {}", cur_worker);
    cur_worker++;

    if(mode == CMD::POST_STABILIZATION) {
        ExpUtil in;
        shared_ptr<VIDEO_INFO>info = make_shared<VIDEO_INFO>();
        int result = in.ImportVideoInfo(arg, info.get());
        CMd_INFO(" swipe period size {} ", info->swipe_period.size());
        if (result == CMD::ERR_NONE)        
            EnqueueJob(&m_future, &TaskManager::RunStabilize, this, info);
        else {
            CMd_WARN(" Stabilization Message is not compatible ERR: {} ", result);
            m_qTMSG.Dequeue();
        }


    } else if (mode == CMD::UPDATE_CONFIGURE) {

    } else if (mode == CMD::SEND_VERSION) {
        SendVersionMessage(arg);
    }

    return CMD::ERR_NONE;
}

void TaskManager::WatchFuture() {

    while(watching) {
        if (m_future.IsQueue()) {
            MakeSendMsg(m_qTMSG.Dequeue(),  m_future.Dequeue());
            cur_worker--;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));        
    }
}

void TaskManager::OnRcvTask(std::shared_ptr<CMD::MSG_T> ptrMsg) {
	m_qTMSG.Enqueue(ptrMsg);
}

std::string TaskManager::GetDocumentToString(Document& document)
{
	StringBuffer strbuf;
	strbuf.Clear();
	PrettyWriter<StringBuffer> writer(strbuf);
	document.Accept(writer);
	std::string ownShipRadarString = strbuf.GetString();

	return ownShipRadarString;
}

void TaskManager::MakeSendMsg(std::shared_ptr<CMD::MSG_T> ptrMsg, int result) {

    if(result <  CMD::ERR_NONE) {
        CMd_WARN(" Captured future return is ERR {} ", result);        
        return;
    }

    Document sndDoc(kObjectType);
    Document::AllocatorType& allocator = sndDoc.GetAllocator();

    if( result == CMD::STABIL_COMPLETE){
        nlohmann::json j = nlohmann::json::parse(ptrMsg->txt);
        std::string outfile = j["output"];
        std::string str_token = Configurator::Get().GenerateToken();
        CMd_INFO(" Generated token {} ", str_token.c_str());

        sndDoc.AddMember(MTDPROTOCOL_SECTION1, "4DReplay", allocator);
        sndDoc.AddMember(MTDPROTOCOL_SECTION2, "CM", allocator);
        sndDoc.AddMember(MTDPROTOCOL_SECTION3, "StabilizeDone", allocator);
        sndDoc.AddMember(MTDPROTOCOL_SENDSTATE, "request", allocator);
        sndDoc.AddMember(MTDPROTOCOL_TOKEN, str_token, allocator); //token..
        sndDoc.AddMember(MTDPROTOCOL_FROM, "CMd", allocator);
        sndDoc.AddMember(MTDPROTOCOL_TO, "4DPD", allocator);
        sndDoc.AddMember(MTDPROTOCOL_ACTION, "set", allocator);
        sndDoc.AddMember("output", outfile, allocator);
    }
    
    std::string strSendString = GetDocumentToString(sndDoc);
    m_msgmanager->OnRcvSndMessage(strSendString);
}

void TaskManager::SendVersionMessage(std::string ptrMsg) {
	Document document;
	document.Parse(ptrMsg.c_str());
    Document sndDoc(kObjectType);
    Document::AllocatorType& allocator = sndDoc.GetAllocator();

	Value ver(kObjectType);
	Value cmd(kObjectType);
	cmd.AddMember("verion", CURRENTVERSION, allocator);
	cmd.AddMember("date", Configurator::Get().getCurrentDateTime("now"), allocator);
	ver.AddMember("CMd", cmd, allocator);

	sndDoc.AddMember(MTDPROTOCOL_SECTION1, document[MTDPROTOCOL_SECTION1], allocator);
	sndDoc.AddMember(MTDPROTOCOL_SECTION2, document[MTDPROTOCOL_SECTION2], allocator);
	sndDoc.AddMember(MTDPROTOCOL_SECTION3, document[MTDPROTOCOL_SECTION3], allocator);
	sndDoc.AddMember(MTDPROTOCOL_SENDSTATE, "response", allocator);
	sndDoc.AddMember(MTDPROTOCOL_TOKEN, document[MTDPROTOCOL_TOKEN], allocator);
	sndDoc.AddMember(MTDPROTOCOL_FROM, document[MTDPROTOCOL_TO], allocator);
	sndDoc.AddMember(MTDPROTOCOL_TO, document[MTDPROTOCOL_FROM], allocator);
	sndDoc.AddMember(MTDPROTOCOL_ACTION, document[MTDPROTOCOL_ACTION], allocator);
	sndDoc.AddMember("Version", ver, allocator);
	sndDoc.AddMember(MTDPROTOCOL_RESULTCODE, COMMON_ERR_NONE, allocator);
	sndDoc.AddMember(MTDPROTOCOL_ERRORMSG, "SUCCESS", allocator);
    std::string strSendString = GetDocumentToString(sndDoc);
    m_msgmanager->OnRcvSndMessage(strSendString);
}