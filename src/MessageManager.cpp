
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
#include "ExpUtil.hpp"
#include "DefData.hpp"
using json = nlohmann::json;

MsgManager::MsgManager()
	: m_taskmanager(TASKPOOL_SIZE) {
    b_RMSGThread = true;
    b_SMSGThread = true;		
	m_pRMSGThread = new std::thread(&MsgManager::RcvMSGThread, this, this);
	m_pSMSGThread = new std::thread(&MsgManager::SndMSGThread, this, this);	
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
	printf("RcvMsgThread start \n");

	std::shared_ptr<MSG_T> msg = nullptr;
	while(b_RMSGThread)
	{
		if (m_qRMSG.IsQueue())
		{
			msg = m_qRMSG.Dequeue();			
			if (msg != nullptr)
			{
				cout << "RcvMSGThread : " << msg->txt<< endl;
				json j = json::parse(msg->txt);
				string action = j["Action"];
				if (action == "Stabilization") {
					ExpUtil in;
					shared_ptr<VIDEO_INFO>info = make_shared<VIDEO_INFO>();
					int result = in.ImportVideoInfo(msg->txt, info.get());
			        printf(" swipe period size 0 %lu \n", info->swipe_period.size());        					
					if (result == CMD::ERR_NONE) {
						m_taskmanager.MakeTask(CMD::POST_STABILIZATION, info);
					}
				}

			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

	return nullptr;	
}

void MsgManager::OnRcvMessage(char* pData) {

	std::shared_ptr<MsgManager::MSG_T> ptrMsg = std::shared_ptr<MsgManager::MSG_T>(new MsgManager::MSG_T);
	ptrMsg->type = MsgManager::PACKET_TYPE::TEXT;
	ptrMsg->txt = pData;		
	m_qRMSG.Enqueue(ptrMsg);

}

void MsgManager::MakeSendMsg() {

}

void* MsgManager::SndMSGThread(void* arg) {

	while(b_SMSGThread)
	{
		if (m_qSMSG.IsQueue())
		{
				//m_ctrl_client.SendPacket(pkt->payload);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

	return nullptr;		
}