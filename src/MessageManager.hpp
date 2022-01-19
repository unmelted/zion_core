
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

#include "json.hpp"
#include "TaskManager.hpp"

class MsgManager {

public:
	typedef struct PACKET_TYPE {
		static const int TEXT = 0;
		static const int BINARY = 1;
	} PACKET_TYPE;
    
	typedef struct SEND_PACKET {
		int		type;
		int		length;
		char*		payload;
		SEND_PACKET(void)
			: type(PACKET_TYPE::TEXT)
			, length{ 0 }
			, payload(nullptr)
		{}

		~SEND_PACKET(void) {
			if (payload && length > 0)
				free(payload);
			payload = nullptr;
			length = 0;
		}
	} SEND_PACKET;

	typedef struct _MSG_T {
		int32_t		type;
		std::string txt;
		char*		bin;
		int32_t		bin_size;
		_MSG_T(void)
			: type(PACKET_TYPE::TEXT)
			, txt{}
			, bin(nullptr)
			, bin_size(0)
		{}

		~_MSG_T(void) {
			if (bin && bin_size > 0)
				free(bin);
			bin = nullptr;
			bin_size = 0;
		}
	} MSG_T;

    MsgManager();
    ~MsgManager();
	void OnRcvMessage(char* pData);

private :
    bool b_RMSGThread;
	std::thread* m_pRMSGThread{ nullptr };
    bool b_SMSGThread;
	std::thread* m_pSMSGThread{ nullptr };
	MessageQueue<std::shared_ptr<MSG_T>> m_qRMSG;
	MessageQueue<std::shared_ptr<SEND_PACKET>> m_qSMSG;    

	TaskPool::TaskManager m_taskmanager;
    void* RcvMSGThread(void* arg);
    void* SndMSGThread(void* arg);
	void MakeSendMsg();
};