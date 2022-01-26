
/*****************************************************************************
*                                                                            *
*                             CMDefine       								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : CMDefine.hpp
    Author(S)       : Me Eunkyung
    Created         : 17 Jan 2022

    Description     : Base Define Err or Etc
    Notes           : 
*/

#pragma once 
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <algorithm>
#include <iostream>
#include <deque>
#include <string>
#include <set>
#include <cstdint>
#include <map>

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "DMServer.h"
#include "DaemonDefine.h"
#include "CMdLogger.hpp"
#include "json.hpp"
#include "MessageQueue.h"
#include "Configurator.hpp"
#include "Stabilization.hpp"


#define TASKPOOL_SIZE 3
namespace CMD {
typedef enum _cmerr {
    ERR_NONE = 0,
    TASKMANAGER_NO_MORE_WOKER      = 10,
    STABIL_INFO_ERROR_NO_VIDEOFILE = 100, //stabil
    STABIL_INFO_ERROR_NO_SWIPE     = 101,
    STABIL_COMPLETE                = 199,
} CMD_ERR;

typedef enum _cmdmode {
    DEFAULT_NONE = 0,
    SEND_VERSION        = 6,
    POST_STABILIZATION  = 7,
    UPDATE_CONFIGURE    = 10,

} CMD_MODE;

typedef struct PACKET_TYPE {
    static const int TEXT = 0;
    static const int BINARY = 1;
} PACKET_TYPE;

typedef struct SEND_PACKET {
    int		type;
    int		length;
    char*		payload;
    SEND_PACKET(void)
        : type(PACKET_TYPE::TEXT)
        , length{ 0 }
        , payload(nullptr)
    {}

    ~SEND_PACKET(void) {
        if (payload && length > 0)
            free(payload);
        payload = nullptr;
        length = 0;
    }
} SEND_PACKET;

typedef struct _MSG_T {
    int32_t		type;
    std::string txt;
    char*		bin;
    int32_t		bin_size;
    _MSG_T(void)
        : type(PACKET_TYPE::TEXT)
        , txt{}
        , bin(nullptr)
        , bin_size(0)
    {}

    ~_MSG_T(void) {
        if (bin && bin_size > 0)
            free(bin);
        bin = nullptr;
        bin_size = 0;
    }
} MSG_T;

typedef enum _event {
    Default     = 1,
    Hockey      = 2,
    Figure      = 3,
    ShortTrack  = 4,
    SkiJump     = 5,
    Basball     = 6,
    Basketball  = 7,
    Boxing      = 8,
    Football    = 9,
    Golf        = 10,
    Soccer      = 11,
    Taekwondo   = 12,
    Tennis      = 13,
    UFC         = 14,
    Volleyball  = 15,
}EVENT;
}