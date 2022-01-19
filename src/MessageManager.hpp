
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

#include <thread>
#include <algorithm>
#include <iostream>
#include <queue>
#include <deque>
#include <string>
#include <iostream>
#include <set>
#include <cstdint>
#include <map>
#include <string>


#include "TaskManager.hpp"

class MsgManager {

public:

    MsgManager();
    ~MsgManager();
	void OnRcvMessage(char* pData);

private :
    bool b_RMSGThread;
	std::thread* m_pRMSGThread{ nullptr };
    bool b_SMSGThread;
	std::thread* m_pSMSGThread{ nullptr };
	MessageQueue<std::shared_ptr<CMD::MSG_T>> m_qRMSG;
	MessageQueue<std::shared_ptr<CMD::SEND_PACKET>> m_qSMSG;    

	TaskPool::TaskManager m_taskmanager;
    void* RcvMSGThread(void* arg);
    void* SndMSGThread(void* arg);
	void MakeSendMsg();
};