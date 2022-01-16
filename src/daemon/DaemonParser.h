#pragma once

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "Util/logger.h"
#include "define.h"
#include "DMServer.h"
#include "curl/curl.h"

using namespace rapidjson;

class DaemonParser
{
private:
	DMServer* m_dmServer;
	bool m_bThreadStop;

public:
	DaemonParser();
	~DaemonParser();

	void SetDMServer(DMServer* dmServer);

	DMServer* GetDMServer();
	bool IsThreadStop();
	void RunParse(std::string strMessage);
private:
	void ParseThread(void* param, std::string strMessage);
	int GetBasicReturnJson(Document& document, MTdProtocol& mtdProtocol);
	std::string GetDocumentToString(Document& document);
};
