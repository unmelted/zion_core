
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

} CMD_ERR;

typedef enum _cmdmode {
    DEFAULT_NONE = 0,
    POST_STABILIZATION = 7,

} CMD_MODE;

}