
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

MsgManager::MsgManager() {
	m_pRMSGThread = new std::thread(&MsgManager::RcvMSGThread, this, this);
	m_pSMSGThread = new std::thread(&MsgManager::SndMSGThread, this, this);
    b_RMSGThread = true;
    b_SMSGThread = true;	
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