
/*****************************************************************************
*                                                                            *
*                            Configurator   								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : Configurator.hpp
    Author(S)       : Me Eunkyung
    Created         : 20 Jan 2022

    Description     : Managing configuration. for system & algorithm
    Notes           : 
*/
#pragma once
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <filesystem>

class Configurator {

public:
    typedef struct _TIMER {
        unsigned int expire;
        void* routine;
        void* arg;
        short timer_id;
        clock_t last_time;
        bool last_check = 0;
        
    } TIMER;

    static Configurator& Get();
    void SetDirectory();   
    std::string GenerateToken();     
    std::string getCurrentDateTime(std::string s);   
    void UpdateConfiture(int mode, int category, double value); 

    void StartTimer(TIMER *times);
    int EndTimer(TIMER *times);
    float LapTimer(TIMER *times);

    struct Path {
        static inline const std::filesystem::path LOG {std::filesystem::current_path().append("log") };
        static inline const std::filesystem::path ANLS {std::filesystem::current_path().append("analysis") };
        static inline const std::filesystem::path DUMP {std::filesystem::current_path().append("dump") };
    };

    struct ConfigureValue{
        enum cfg_mode {
            SERVER      = 10,
            STABIL      = 11,
        };
        enum category {
            ROI_WIDTH    = 100,
            ROI_HEIGHT   = 101,
            SWIPE_PT_X   = 102,
            SWIPE_PT_Y   = 103,
            DUMP_TRUE    = 104,
        };        
    };

private :
    int serial = 0;
};