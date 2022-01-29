#include "DaemonParser.h"
#include "define.h"
#include "ErrorList.h"
#include <rapidjson/prettywriter.h>
#include <set>
#include <future>
#include "Configurator.hpp"
#include "CMdLogger.hpp"

//#define PRINT_HTTP_REQ
//#define PRINT_HTTP_RES

namespace
{
	template<typename ... Args>
	std::string string_format(const std::string& format, Args ... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}
}

DaemonParser::DaemonParser()
{

}

DaemonParser::~DaemonParser()
{

}

void DaemonParser::RunParse(std::string strMessage)
{
	std::thread th(&DaemonParser::ParseThread, this, this, strMessage);

	th.join();
}

bool DaemonParser::IsThreadStop()
{
	return m_bThreadStop;
}

void DaemonParser::ParseThread(void* param, std::string strMessage)
{
	DaemonParser* pMain = (DaemonParser*)param;
	if (pMain->IsThreadStop())
		return;

	InfoL << "Recv : " << strMessage;
	Document document;
	document.Parse(strMessage.c_str());

	string strError;
	MTdProtocol mtdProtocol;
	int nResultCode = GetBasicReturnJson(document, mtdProtocol);
	Document sendDocument(kObjectType);
	Document::AllocatorType& allocator = sendDocument.GetAllocator();

	if (document.HasMember(MTDPROTOCOL_SENDSTATE) == true)
	{
		std::string strSendState = document[MTDPROTOCOL_SENDSTATE].GetString();
		if (strSendState.compare(MTDPROTOCOL_SENDSTATE_RESPONSE) == 0) // Return ó��
			nResultCode = COMMON_ERR_UNKNOWN_SENDSTATE;
	}

	sendDocument.AddMember(MTDPROTOCOL_SECTION1, mtdProtocol.Section1, allocator);
	sendDocument.AddMember(MTDPROTOCOL_SECTION2, mtdProtocol.Section2, allocator);
	sendDocument.AddMember(MTDPROTOCOL_SECTION3, mtdProtocol.Section3, allocator);
	sendDocument.AddMember(MTDPROTOCOL_SENDSTATE, "response", allocator);
	sendDocument.AddMember(MTDPROTOCOL_TOKEN, mtdProtocol.Token, allocator);
	sendDocument.AddMember(MTDPROTOCOL_FROM, mtdProtocol.To, allocator);
	sendDocument.AddMember(MTDPROTOCOL_TO, mtdProtocol.From, allocator);
	sendDocument.AddMember(MTDPROTOCOL_ACTION, mtdProtocol.action, allocator);
	sendDocument.AddMember(MTDPROTOCOL_RESULTCODE, nResultCode, allocator);
	sendDocument.AddMember(MTDPROTOCOL_ERRORMSG, "", allocator);

	if (nResultCode != SUCCESS)
	{
		sendDocument[MTDPROTOCOL_ERRORMSG].SetString(GetErrorCodeToString(nResultCode), allocator);
		std::string strSendString = GetDocumentToString(sendDocument);
		if (pMain->GetDMServer()->SendData(strSendString.c_str()))
		{
			ErrorL << strSendString;
		}
		return;
	}

	string strSection1 = mtdProtocol.Section1;
	string strSection2 = mtdProtocol.Section2;
	string strSection3 = mtdProtocol.Section3;
	string strAction = mtdProtocol.action;
	CMd_DEBUG("sction {} {} {}", strSection1, strSection2, strSection3);

	if (strSection1.compare("Daemon") == 0)
	{
		if (strSection2.compare("Information") == 0)
		{
			if (strSection3.compare("Version") == 0)
			{
				Value ver(kObjectType);
				Value cmd(kObjectType);
				cmd.AddMember("version", CURRENTVERSION, allocator);
				cmd.AddMember("date", Configurator::Get().getCurrentDateTime("now"), allocator);
				ver.AddMember("CMd", cmd, allocator);
				sendDocument.AddMember("Version", ver, allocator);				
				CMd_INFO("version process.");
			}
		}
	}

	std::string strSendString = GetDocumentToString(sendDocument);
	pMain->GetDMServer()->SendData(strSendString.c_str());
}


DMServer* DaemonParser::GetDMServer()
{
	return m_dmServer;
}


void DaemonParser::SetDMServer(DMServer* dmServer)
{
	m_dmServer = dmServer;
}

std::string DaemonParser::GetDocumentToString(Document& document)
{
	StringBuffer strbuf;
	strbuf.Clear();
	PrettyWriter<StringBuffer> writer(strbuf);
	document.Accept(writer);
	std::string ownShipRadarString = strbuf.GetString();

	return ownShipRadarString;
}

int DaemonParser::GetBasicReturnJson(Document& document, MTdProtocol& mtdProtocol)
{
	if (document.HasMember(MTDPROTOCOL_SECTION1) == true)
		mtdProtocol.Section1 = document[MTDPROTOCOL_SECTION1].GetString();
	else
		return COMMON_ERR_NOT_FOUND_SEC1;

	if (document.HasMember(MTDPROTOCOL_SECTION2) == true)
		mtdProtocol.Section2 = document[MTDPROTOCOL_SECTION2].GetString();
	else
		return COMMON_ERR_NOT_FOUND_SEC2;

	if (document.HasMember(MTDPROTOCOL_SECTION3) == true)
		mtdProtocol.Section3 = document[MTDPROTOCOL_SECTION3].GetString();
	else
		return COMMON_ERR_NOT_FOUND_SEC3;

	if (document.HasMember(MTDPROTOCOL_SENDSTATE) == true)
		mtdProtocol.SendState = document[MTDPROTOCOL_SENDSTATE].GetString();
	else
		return COMMON_ERR_NOT_FOUND_SENDSTATE;

	if (document.HasMember(MTDPROTOCOL_TOKEN) == true)
		mtdProtocol.Token = document[MTDPROTOCOL_TOKEN].GetString();
	else
		return COMMON_ERR_NOT_FOUND_TOKEN;

	if (document.HasMember(MTDPROTOCOL_FROM) == true)
		mtdProtocol.From = document[MTDPROTOCOL_FROM].GetString();
	else
		return COMMON_ERR_NOT_FOUND_FROM;

	if (document.HasMember(MTDPROTOCOL_TO) == true)
		mtdProtocol.To = document[MTDPROTOCOL_TO].GetString();
	else
		return COMMON_ERR_NOT_FOUND_TO;

	if (document.HasMember(MTDPROTOCOL_ACTION) == true)
		mtdProtocol.action = document[MTDPROTOCOL_ACTION].GetString();
	else
		return COMMON_ERR_NOT_FOUND_ACTION;

	return SUCCESS;
}