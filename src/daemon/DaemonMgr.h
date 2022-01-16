#pragma once

#include "DMServer.h"
#include "define.h"
#include <functional>
#include "DaemonParser.h"
#include "MessageManager.hpp"

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
using namespace rapidjson; 

class DaemonMgr
{
public:
	DaemonMgr();
	~DaemonMgr();

private:
	DMServer m_dmServer;
	DaemonParser m_parser;
	MsgManager m_manger;

	void GetBasicReturnJson(Document& document, MTdProtocol& mtdProtocol);
	int	Classfication(char cSeparator, char* pData, int nDataSize);
	int RecJson(std::string strMessage);
};

