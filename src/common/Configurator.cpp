
/*****************************************************************************
*                                                                            *
*                            Configurator   								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : Configurator.cpp
    Author(S)       : Me Eunkyung
    Created         : 20 Jan 2022

    Description     : Managing configuration. for system & algorithm
    Notes           : 
*/

#include "Configurator.hpp"


Configurator&  Configurator::Get() {
    static Configurator _instance;
    return _instance;
}

void Configurator::SetDirectory() {
    std::filesystem::create_directories(Path::LOG);
    std::filesystem::create_directories(Path::ANLS);
    std::filesystem::create_directories(Path::DUMP);   

    
}

std::string Configurator::getCurrentDateTime(std::string s)
{
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);
    if(s == "now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if(s == "date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return std::string(buf);
}

std::string Configurator::GenerateToken() {
    char tk[100];
    std::string base = getCurrentDateTime("date");
    if(serial >= 9999)
        serial = 0;
    serial ++;
    sprintf(tk, "%s_%04d", base.c_str(), serial);
    std::string str_tk(tk);
    return str_tk;    
}

void Configurator::StartTimer(TIMER *times)
{
    times->last_check = true;
    times->last_time = clock();
}

int Configurator::EndTimer(TIMER *times)
{
    if(times->last_check == 0) return -1;
    int ms = clock() - times->last_time;
    times->last_check = 0;
    return ms;
}

float Configurator::LapTimer(TIMER *times)
{
    if(times->last_check == 0) return -1;
    clock_t ctime = clock();
    float ms = ((float)ctime - (float)times->last_time)/1000.0;
    times->last_time = ctime;
    times->last_check = true;
    return ms;
}
