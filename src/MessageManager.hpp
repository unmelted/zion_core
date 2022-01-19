
/*****************************************************************************
*                                                                            *
*                            MessageManager   								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : MessageManager.hpp
    Author(S)       : Me Eunkyung
    Created         : 16 Jan 2022

    Description     : MessageManager for socket transfer
    Notes           : 
*/
#pragma once 
#include "CMDefine.hpp"
#include "TaskManager.hpp"

class MsgManager {

public:

    MsgManager();
    ~MsgManager();
	void OnRcvMessage(char* pData);
	void OnRcvSndMessage(std::string);
	void SetDMServer(DMServer* dmServer);
	DMServer* GetDMServer();

private :
	DMServer* m_dmServer;
    bool b_RMSGThread;
	std::thread* m_pRMSGThread{ nullptr };
    bool b_SMSGThread;
	std::thread* m_pSMSGThread{ nullptr };
	MessageQueue<std::shared_ptr<CMD::MSG_T>> m_qRMSG;
	MessageQueue<std::shared_ptr<std::string>> m_qSMSG;    

	TaskPool::TaskManager m_taskmanager;
    void* RcvMSGThread(void* arg);
    void* SndMSGThread(void* arg);
};