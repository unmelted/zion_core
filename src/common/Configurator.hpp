
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

#include <cstdlib>
#include <time.h>
#include <chrono>
#include <filesystem>

class Configurator {

public:
    static Configurator& Get();
    void SetDirectory();
    std::string GenerateToken();
    void UpdateConfiture(int mode, int category, double value);
    std::string getCurrentDateTime(std::string s);
    
    struct Path {
        static inline const std::filesystem::path LOG {std::filesystem::current_path().append("Log") };
        static inline const std::filesystem::path DUMP {std::filesystem::current_path().append("Dump") };
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