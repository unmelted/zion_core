////////////////////////////////////////////////////////////////////////////////
//
//	DMServer.h
//
//  4dreplay, Inc. PROPRIETARY INFORMATION.
//  The following contains information proprietary to 4dreplay, Inc. and may not be copied
//  nor disclosed except upon written agreement by 4dreplay, Inc.
//
//  Copyright (C) 2020 4dreplay, Inc. All rights reserved.
//
// @author	changdo kim (cdkim@4dreplay.com)
// @Date	2020-12-23
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "iostream"
#include <functional>
#include <cstring>
#include "define.h"
#include <mutex>
#include <thread>
#include <list>
#ifdef _WIN32 // Windows Headers

#pragma warning(disable:4996)
#define HAVE_STRUCT_TIMESPEC
#pragma comment (lib, "ws2_32.lib") // Winsock ���̺귯�� �ڵ� ��ũ
#include <winsock2.h>

#define NET_INVALID_SOCKET	INVALID_SOCKET
#define NET_SOCKET_ERROR SOCKET_ERROR
typedef int socklen_t; // Unix Ÿ���� unsigned int Windows �� int

#else // Unix Headers

#include <sys/socket.h> // for socket(), bind(), connect()
#include <arpa/inet.h> // for sockaddr_in, inet_ntoa()
#include <unistd.h> // for close()
#include <stdlib.h> // for exit()
# include <sys/ioctl.h>
# include <net/if.h>

typedef int SOCKET;
#define NET_INVALID_SOCKET	-1
#define NET_SOCKET_ERROR -1

#endif

struct ClientSockThreadData
{
    void* pthis;
    int nType;
    int nSocket;
    std::string strClientIP;
};

class DMServer 
{
public : 
    DMServer();
    ~DMServer();

    bool BeginSocket(int nPort, int nType);
    //void SendData(char* pData, int nSize);
    bool SendData(std::string strJson);

    bool IsConnected();

    typedef std::function<int(char cSeparator, char* pData, int nDataSize)> callback;
    callback m_onClassfication;

    void setHandler(callback f) {
        m_onClassfication = std::move(f);
    }

	std::list<std::string> GetIPList();
	std::string GetLocalCompare(std::string strIP);
	std::string GetLocalAddress() { return m_strIP; }

private:
    std::mutex  m_Sockmutx;
    std::mutex m_SendMutex;

    bool bMainSocketThread;
    std::thread* m_mainSocketThread;
    std::thread* m_clientReceiveThread;

    int m_ServerSockets;
    int m_ClientSockets;
    int m_ServerPorts;
    int m_nSockType;
    int m_nSendBufferSize;
    char* m_pSendBuffer;

    void CloseSocket(int nSock);
    void RunSocket();

    void* RunSocketThread(void* arg);
    void* handle_clnt(void* arg);
    int RECV(int clnt_sock, char* pRecv, int nSize, int flags);

    std::string m_strIP;
    std::string m_strClientIP;
    
};

