#include "DaemonMgr.h"
#include <string.h>


DaemonMgr::DaemonMgr()
{
#ifdef _WIN32
	WSADATA wsaData;
	WORD version;
	version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData) < 0)  //���� �ʱ�ȭ
	{
		ErrorL << "WSAStartup Init Error";
		WSACleanup();
		exit(0);
	}
#endif
	m_parser.SetDMServer(&m_dmServer);
	m_dmServer.BeginSocket(CM_CONTROL_DAEMON_PORT, 0);
	m_dmServer.setHandler(std::bind(&DaemonMgr::Classfication, this, std::placeholders::_1, placeholders::_2, placeholders::_3));
	m_manger.SetDMServer(&m_dmServer);	

	Configurator::Get().SetDirectory();
}

DaemonMgr::~DaemonMgr()
{
#ifdef _WIN32
	WSACleanup();
#endif
}
int	DaemonMgr::Classfication(char cSeparator, char* pData, int nDataSize)
{
	switch (cSeparator)
	{
	case PACKETTYPE_JSON:
		RecJson(pData);
		break;

	default:
		ErrorL << "Invalid Message Separator : " << int(cSeparator) << "\n" << pData;
		break;
	}

	return 0;
}


int DaemonMgr::RecJson(std::string strMessage)
{
	Document document;
	bool bSuc = false;
	try {
		bSuc = document.Parse(strMessage.c_str()).HasParseError();
	}
	catch (...) {
		bSuc = true;
	}
	if (bSuc)
	{
		ErrorL << "Json Parsing Fail";
		ErrorL << strMessage;
		return 0;
	}

	std::string sec3 = document[MTDPROTOCOL_SECTION3].GetString();
	CMd_INFO("RecJson sec3 : {} compare {}", sec3, sec3.compare("Version"));
	if(sec3.compare("Version") == 0) {
		m_parser.RunParse(strMessage);
	}
	else if(sec3.compare("Stabilize") == 0) {
		m_parser.RunParse(strMessage);		
		m_manger.OnRcvMessage(strMessage);
	}

	return 1;
}

void DaemonMgr::GetBasicReturnJson(Document& document, MTdProtocol& mtdProtocol)
{
	std::string strTp;
	if (document.HasMember("Section1") == true)
		mtdProtocol.Section1 = document["Section1"].GetString();

	if (document.HasMember("Sectoin2") == true)
		mtdProtocol.Section2 = document["Sectoin2"].GetString();

	if (document.HasMember("Sectoin3") == true)
		mtdProtocol.Section3 = document["Sectoin3"].GetString();
	
	if (document.HasMember("SendState") == true)
		mtdProtocol.SendState = document["SendState"].GetString();

	if (document.HasMember("From") == true)
		mtdProtocol.From = document["From"].GetString();

	if (document.HasMember("To") == true)
		mtdProtocol.To = document["To"].GetString();

	if (document.HasMember("action") == true)
		mtdProtocol.action = document["action"].GetString();
}