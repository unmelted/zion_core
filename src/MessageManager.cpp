
/*****************************************************************************
*                                                                            *
*                            MessageManager   								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : MessageManager.cpp
    Author(S)       : Me Eunkyung
    Created         : 16 Jan 2022

    Description     : MessageManager for socket transfer
    Notes           : 
*/

#include "MessageManager.hpp"
#include "DefData.hpp"
using json = nlohmann::json;

MsgManager::MsgManager()
	: m_taskmanager(TASKPOOL_SIZE, this) {
    b_RMSGThread = true;
    b_SMSGThread = true;		
	m_pRMSGThread = new std::thread(&MsgManager::RcvMSGThread, this, this);
	m_pSMSGThread = new std::thread(&MsgManager::SndMSGThread, this, this);	
	std::function<void(MsgManager&, const std::string msg)> f1 = &MsgManager::OnRcvSndMessage;
	m_taskmanager.SetSndQue(f1);
} 


DMServer* MsgManager::GetDMServer() {
	return m_dmServer;
}


void MsgManager::SetDMServer(DMServer* dmServer) {
	m_dmServer = dmServer;
}

MsgManager::~MsgManager() {

	b_RMSGThread = false;
	if (m_pRMSGThread != nullptr)
	{
		m_pRMSGThread->join();
		delete m_pRMSGThread;
		m_pRMSGThread = nullptr;
	}
	
	b_SMSGThread = false;
	if (m_pSMSGThread != nullptr)
	{
		m_pSMSGThread->join();
		delete m_pSMSGThread;
		m_pSMSGThread = nullptr;
	}
}

void* MsgManager::RcvMSGThread(void* arg) {

	std::shared_ptr<CMD::MSG_T> msg = nullptr;
	while(b_RMSGThread)
	{
		if (m_qRMSG.IsQueue())
		{
			msg = m_qRMSG.Dequeue();	
			m_taskmanager.OnRcvTask(msg);
			if (msg != nullptr)
			{
				CMd_INFO("RcvMSGThread : {} ", msg->txt);
				json j = json::parse(msg->txt);
				if(j.contains("Action") == false || j.contains("Section3") == false)
				{
					CMd_WARN("Json component missing. can't execute.");
					continue;
				}
				string section3 = j["Section3"];
				string action = j["Action"];
				if (action == "Stabilization" || section3 == "Stabilize") {
					m_taskmanager.CommandTask(CMD::POST_STABILIZATION, msg->txt);
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

	return nullptr;	
}

void MsgManager::OnRcvMessage(std::string pData) {

	std::shared_ptr<CMD::MSG_T> ptrMsg = std::shared_ptr<CMD::MSG_T>(new CMD::MSG_T);
	ptrMsg->type = CMD::PACKET_TYPE::TEXT;
	ptrMsg->txt = pData;		
	m_qRMSG.Enqueue(ptrMsg);

}

void MsgManager::OnRcvSndMessage(std::string msg) {
	//CMd_INFO("OnRcvSndMessage : {}", msg );
	std::shared_ptr<std::string> pmsg = make_shared<std::string>(msg);
	m_qSMSG.Enqueue(pmsg);
}

void* MsgManager::SndMSGThread(void* arg) {

	std::shared_ptr<std::string> msg = nullptr;
	while(b_SMSGThread) {

		if (m_qSMSG.IsQueue()) {
			msg = m_qSMSG.Dequeue();		
			CMd_INFO(" SndMsg thread msg : {} " , msg->c_str());
			GetDMServer()->SendData(msg->c_str());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

	return nullptr;		
}
