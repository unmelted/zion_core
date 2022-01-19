  
/*****************************************************************************
*                                                                            *
*                            Util         					    			 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : ExpUtil.cpp
    Author(S)       : Me Eunkyung
    Created         : 10 Jan 2022

    Description     : ExpUtil.Cpp
    Notes           : Uility - json im/export
*/

#include "ExpUtil.hpp"

using json = nlohmann::json;
using namespace dove;

int ExpUtil::ImportVideoInfo(const string js, VIDEO_INFO* info) {
    json j;    

    if(json_file_mode == true) {
        ifstream json_file(js);
        json_file >> j;
    } else {
        j = json::parse(js);
    }

    info->input = j["input"];
    info->output = j["output"];
    string ev = j["event"];
    cout << "event string : "<< ev << endl;
    if( ev.compare("FIGURE") == 0 )
        info->event = dove::FIGURE;
    else
        info->event = dove::HOCKEY;
        
    info->width = j["width"];
    info->height = j["height"];

    for(auto& elm : j["swipeperiod"]) {
        SWIPE_INFO swi;
        swi.order = elm["no"];
        swi.start = elm["start"];
        swi.end = elm["end"];
        swi.target_x = elm["target_x"];
        swi.target_y = elm["target_y"];
        swi.zoom = elm["zoom"];
        cout<< "SWIPE PERIOD start " << swi.start << " end " << swi.end << endl;;
        info->swipe_period.push_back(swi); 
    }

    std::sort(info->swipe_period.begin(), info->swipe_period.end(), [](SWIPE_INFO a, SWIPE_INFO b) {
              return a.order < b.order;
    });
    cout<< "input : "<<info->input << endl;
    cout<< "output : " <<info->output << endl;
    cout<< "event : " <<info->event << endl;
    cout <<" swipe cnt : " << info->swipe_period.size() << endl;
    return ERR_NONE;
}

void ExpUtil::TestGetSwipeInfo(string _in, PARAM* p) {
#if defined _MAC_
    if(_in == "movie/4dmaker_600.mp4" || _in == "movie/4dmaker_600_out2.mp4") {
#else
    if (_in == "movie\\4dmaker_600f.mp4" || _in == "movie\\600.mp4") {
#endif
        printf(" ------------ 600 !\n");        
        p->swipe_start = 79; //600 OK        
        p->swipe_end = 180;     
    } else if (_in == "movie/4dmaker_603.mp4") {
        printf(" ------------ 603 !\n");
        p->swipe_start = 79;
        p->swipe_end = 181; //603 -- should conquer - couple gracking -- square merge needed 
    } else if (_in == "movie/4dmaker_626.mp4") {
        printf(" ------------ 626 !\n");        
        p->swipe_start = 79;
        p->swipe_end = 183; //626 OK emerald onepiece single
    } else if (_in == "movie/4dmaker_639.mp4") {
        p->swipe_start = 78;
        p->swipe_end = 130; //639 white shirts man single -- square merge needed
    } else if (_in == "movie/4dmaker_598.mp4") {
        p->swipe_start = 79;
        p->swipe_end = 165; //598 -- frame drop severe
    } else if (_in == "movie/4dmaker_607.mp4") {
        p->swipe_start = 79;
        p->swipe_end = 178; //silver onepiece single - missing during swipe because character
    }else if (_in == "movie/4dmaker_622.mp4") {
        p->swipe_start = 79;
        p->swipe_end = 165; //white onepiece single - missing during swipe because character
    } else if (_in == "movie/4NylanderGoal.mp4") {
        p->swipe_start = 153;
        p->swipe_end = 188; 
        p->roi_sx = 960;
        p->roi_sy = 540;           
    } else if (_in == "movie/BUT_TATAR_4-0.mp4") {
        p->swipe_start = 188;
        p->swipe_end = 232; 
        p->roi_sx = 960;
        p->roi_sy = 540;        
    } else if (_in == "movie/2018_02_09_17_58_50.mp4") {
        p->swipe_start = 64;
        p->swipe_end = 88; //short track
        p->roi_sx = 960;
        p->roi_sy = 540;
    } else if (_in == "movie/2018_02_25_09_55_28.mp4") {
        p->swipe_start = 58;
        p->swipe_end = 91; //short track
        p->roi_sx = 1160; 
        p->roi_sy = 730;
    } else if (_in == "movie/2018_02_13_19_37_53_0.mp4") {
        p->swipe_start = 51;
        p->swipe_end = 84; //short track
        p->roi_sx = 430; 
        p->roi_sy = 850;
    }
}