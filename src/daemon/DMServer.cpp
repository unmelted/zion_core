////////////////////////////////////////////////////////////////////////////////
//
//	DMServer.cpp
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

#include "DMServer.h"


DMServer::DMServer()
{
	bMainSocketThread = false;
	m_mainSocketThread = nullptr;
	m_clientReceiveThread = nullptr;
	m_ServerSockets = 0;
	m_ClientSockets = 0;
	m_ServerPorts = 0;
	m_nSockType = 0;
	m_strIP = "";
	m_nSendBufferSize = 0;
	m_pSendBuffer = nullptr;
	m_strClientIP = "";
}

DMServer::~DMServer()
{
	bMainSocketThread = false;
	CloseSocket(m_ServerSockets);
	CloseSocket(m_ClientSockets);

	if (m_mainSocketThread != nullptr)
	{
		m_mainSocketThread->join();
		m_mainSocketThread = nullptr;
	}

	if (m_clientReceiveThread != nullptr)
	{
		m_clientReceiveThread->join();
		m_clientReceiveThread = nullptr;
	}
	if (m_pSendBuffer != nullptr)
		delete[] m_pSendBuffer;

}

void DMServer::CloseSocket(int nSock)
{
#ifdef WIN32
	closesocket(nSock);
#else
	//close(nSock);
	shutdown(nSock, SHUT_RDWR);
#endif
}

bool DMServer::IsConnected()
{
	if (m_ClientSockets > 0)
		return true;
	else
		return false;
}


bool DMServer::BeginSocket(int nPort, int nType)
{
	if (bMainSocketThread == true)
		return false;

	m_ServerPorts = nPort;
	m_nSockType = nType;
	bMainSocketThread = true;
	m_mainSocketThread = new std::thread(&DMServer::RunSocketThread, this, this);

	return true;
}


void* DMServer::RunSocketThread(void* arg)
{
	DMServer* pSocketMgr = (DMServer*)arg;
	((DMServer*)pSocketMgr)->RunSocket();

	InfoL << "BeginSocketThread end";
	return 0;
}

void DMServer::RunSocket()
{
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;

	m_ServerSockets = (int)socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(m_ServerPorts);

	if (::bind(m_ServerSockets, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	{
		ErrorL << "bind() error";
		return;
	}
	if (listen(m_ServerSockets, 5) == -1)
	{
		ErrorL << "listen() error";
		return;
	}
	int nClientSocket = 0;
	while (bMainSocketThread)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		nClientSocket = (int)(accept(m_ServerSockets, (struct sockaddr*)&clnt_adr, (socklen_t*)&clnt_adr_sz));

		if (nClientSocket <= 0)
			continue;

		m_Sockmutx.lock();
		if (m_ClientSockets != 0)
		{
			m_Sockmutx.unlock();
			std::string strAcceptIP = inet_ntoa(clnt_adr.sin_addr);
			WarnL << "Already Connected MTd IP : " << m_strClientIP << ", Connection Request IP : " << strAcceptIP;
			CloseSocket(nClientSocket);
			continue;
		}
		m_ClientSockets = nClientSocket;
		m_Sockmutx.unlock();

		m_strClientIP = inet_ntoa(clnt_adr.sin_addr);
		InfoL << "Connected IP : " << m_strClientIP;

		m_strIP = GetLocalCompare(m_strClientIP);
		InfoL << "Local IP Address : " << m_strIP;

		//clnt_socks[clnt_sock] = clnt_sock;
		InfoL << "accept socket " << m_ClientSockets;

		ClientSockThreadData* threaddata = new ClientSockThreadData;
		threaddata->pthis = this;
		threaddata->strClientIP = m_strClientIP;
		threaddata->nSocket = m_ClientSockets;

		m_clientReceiveThread = new std::thread(&DMServer::handle_clnt, this, (void*)threaddata);
#ifndef _WIN32
		m_clientReceiveThread->detach();
		usleep(100);
#endif
		//InfoL << "Connected client IP : " << inet_ntoa(clnt_adr.sin_addr);
		//cout<<"Connected client IP: "<< inet_ntoa(clnt_adr.sin_addr);
	}
	InfoL << "Close ServerSocket !!!!!!!!!!!!!!!!!!!!!!!! :" << inet_ntoa(clnt_adr.sin_addr);
}

void* DMServer::handle_clnt(void* arg)
{
	ClientSockThreadData* threadData = (ClientSockThreadData*)arg;
	int clnt_sock = threadData->nSocket;
	std::string clnt_IP = threadData->strClientIP;
	DMServer* pSocketMgr = (DMServer*)threadData->pthis;
	delete threadData;

	while (pSocketMgr->bMainSocketThread)
	{
		int str_len = 0;
		int nPacketSize = 0;
		MTdProtocolHeader mtdProtoHeader;
		bool nRecvOK = true;
		if ((str_len = pSocketMgr->RECV(clnt_sock, (char*)&mtdProtoHeader, sizeof(mtdProtoHeader), 0)) == 0)
		{
			nRecvOK = false;
			break;
		}
		if (str_len < sizeof(MTdProtocolHeader))
			continue;
		nPacketSize = mtdProtoHeader.nSize;
		if (nPacketSize < 1 || nPacketSize > 5000000 || mtdProtoHeader.cSeparator >= PACKETTYPE_SIZE)
		{
			ErrorL << "Invaild Header Packet!!!, Size : " << nPacketSize << ", Separator : " << mtdProtoHeader.cSeparator;
			continue;
		}

		char* pData = nullptr;
		if (nPacketSize >= 0)
		{
			pData = new char[nPacketSize + 1];
			memset(pData, 0, nPacketSize + 1);
			if ((str_len = pSocketMgr->RECV(clnt_sock, (char*)pData, nPacketSize, 0)) == 0)
			{
				nRecvOK = false;
				break;
			}
		}

		if (pSocketMgr->m_onClassfication != 0)
		{
			int nErrorCode = pSocketMgr->m_onClassfication(mtdProtoHeader.cSeparator, pData, nPacketSize);
			//if (nErrorCode != MTD_PROTOCOL_OK)
			//{
			//	std::string strMessage = GetErrorMessage(nErrorCode);
			//	char* pMessage = new char[strMessage.length() + 200];

			//	sprintf(pMessage, "{ \"ResultCode\":\"%d\",\"ErrorMsg\" : \"%s\" }", nErrorCode, strMessage.c_str());

			//	std::string strErrorReturn = pMessage;
			//	if (pMessage != 0)
			//		delete[] pMessage;

			//	pSocketMgr->SendData(strErrorReturn);
			//}
		}

		//bool bJsonParse = pSocketMgr->Classification(pData);
		if (pData != nullptr)
		{
			delete[] pData;
		}
	}

	pSocketMgr->m_Sockmutx.lock();
	pSocketMgr->m_ClientSockets = 0;
	pSocketMgr->m_Sockmutx.unlock();
	pSocketMgr->CloseSocket(clnt_sock);

	InfoL << "Close Socket IP : " << clnt_IP << ", Type : " << arrDaemonObject[pSocketMgr->m_nSockType] << ", SockNum : " << clnt_sock;
	return NULL;
}

bool DMServer::SendData(std::string strJson)
{
	int nSize = (int)strlen(strJson.c_str());
	char cType = PACKETTYPE_JSON;
	m_SendMutex.lock();

	int nSendSize = sizeof(int) + 1 + nSize;
	if (m_nSendBufferSize < nSendSize)
	{
		if (m_pSendBuffer != nullptr)
			delete[] m_pSendBuffer;

		m_pSendBuffer = new char[nSendSize];
		m_nSendBufferSize = nSendSize;
	}
	memcpy(m_pSendBuffer, (char*)&nSize, sizeof(int));
	int nBufPos = sizeof(int);

	memcpy(m_pSendBuffer + nBufPos, &cType, 1);
	nBufPos++;
	memcpy(m_pSendBuffer + nBufPos, strJson.c_str(), nSize);
	m_Sockmutx.lock();
#ifdef _WIN32
	int nSend = send(m_ClientSockets, m_pSendBuffer, nSendSize, 0);
#else
	int nSend = send(m_ClientSockets, m_pSendBuffer, nSendSize, MSG_NOSIGNAL);
#endif
	m_Sockmutx.unlock();
	m_SendMutex.unlock();
	if (nSend != nSendSize)
	{
		ErrorL << "[ERROR]Send Fail MTD";
		return false;
	}

	return true;

}

int DMServer::RECV(int clnt_sock, char* pRecv, int nSize, int flags)
{
	int nReclen = 0, nTotalRecvSize = 0, nRemainSize = nSize;
	int nRecvSize = 1024;
	if (nRecvSize > nSize)
		nRecvSize = nSize;
	while (1)
	{
#ifdef _WIN32
		if ((nReclen = recv(clnt_sock, pRecv, nRecvSize, 0)) == 0)
			return 0;
#else
		if ((nReclen = recv(clnt_sock, pRecv, nRecvSize, MSG_NOSIGNAL)) == 0)
			return 0;
#endif
		if (nReclen <= 0)
			break;

		pRecv += nReclen;
		nTotalRecvSize += nReclen;
		nRemainSize -= nReclen;
		if (nRemainSize <= 0)
			break;

		if (nRecvSize > nRemainSize)
			nRecvSize = nRemainSize;
	}

	return nTotalRecvSize;
}

std::list<std::string> DMServer::GetIPList()
{
	std::list<std::string> lstIPAddress;

#ifdef WIN32
	char name[255];
	PHOSTENT pHostinfo;
	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((pHostinfo = gethostbyname(name)) != NULL)
		{
			if (pHostinfo)
			{
				for (int i = 0; pHostinfo->h_addr_list[i] != NULL; i++)
				{
					IN_ADDR in;
					memcpy(&in, pHostinfo->h_addr_list[i], pHostinfo->h_length);
					lstIPAddress.push_back(inet_ntoa(in));
				}
			}
		}
	}
#else
	int             sock, nRet, nFamily = PF_INET;
	size_t          nNIC;
	const size_t    nMaxNIC = 256;
	struct ifconf   ifc;
	struct ifreq    ifr[nMaxNIC];
	struct sockaddr* pAddr(NULL);
	sock = socket(nFamily, SOCK_STREAM, 0);
	if (sock == -1)
		return lstIPAddress;

	ifc.ifc_len = sizeof(ifr);
	ifc.ifc_ifcu.ifcu_req = ifr;

	nRet = ioctl(sock, SIOCGIFCONF, &ifc);
	if (nRet == -1)
		return lstIPAddress;

	close(sock);
	nNIC = ifc.ifc_len / sizeof(struct ifreq);
	for (size_t i = 0; i < nNIC; i++)
	{
		if (nFamily == ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr.sa_family)
		{
			pAddr = (&ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr);
			sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(pAddr);
			lstIPAddress.push_back(inet_ntoa(sin->sin_addr));
		}
	}
#endif
	return lstIPAddress;
}

std::string DMServer::GetLocalCompare(std::string strIP)
{
	std::string _strIP;

	std::list<std::string> _list = GetIPList();
	std::list<std::string>::iterator iter;

	std::string strC = strIP.substr(0, strIP.rfind('.'));

	for (iter = _list.begin(); iter != _list.end(); iter++)
	{
		std::string strData = *iter;

		std::string strDiff = strData.substr(0, strData.rfind('.'));

		if (strDiff == strC)
		{
			_strIP = strData;
			break;
		}
	}

	return _strIP;
}
