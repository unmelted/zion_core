
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
    POST_STABILIZATION = 7,

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
}