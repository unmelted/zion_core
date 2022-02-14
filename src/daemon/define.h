#pragma once
#include "DaemonDefine.h"
#include "Util/logger.h"
#include "ErrorList.h"

using namespace std;
using namespace toolkit;


struct MTdProtocol {
	std::string Section1;
	std::string Section2;
	std::string Section3;
	std::string SendState;
	std::string From;
	std::string To;
	std::string action;
	std::string Token;
};

#define CURRENTVERSION "4.1.0.2"