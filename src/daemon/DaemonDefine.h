#pragma once

//#if defined(_MSC_VER) // Microsoft Visual Studio
//typedef __int64 Int64;
//typedef unsigned __int64 UInt64;
//#else                 // if defined(_MSC_VER) // Other platforms, use long long
//typedef long long int Int64;
//typedef unsigned long long int UInt64;
//#endif // if defined(_MSC_VER)

#include <map>
#include <cstdio>
#include <string>
#include <time.h>
#include <cstring>


#define MESSAGELTRANSFER_DAEMON_PORT        0x4D01
#define ENTERPRISE_MONITOR_DAEMON_PORT      0x4D02
#define SWITCH_CONTROL_DAEMON_PORT          0x4D03
#define CAMERA_CONTROL_DAEMON_PORT          0x4D04
#define GIMBAL_CONTROL_DAEMON_PORT          0x4D05
#define PROCESS_CONTROL_DAEMON_PORT         0x4D06
#define PRE_STOARGE_DAEMON_PORT             0x4D07
#define VIDEO_PROCESS_FOR_PRESD_DAEMON_PORT  0x4D08
#define VIDEO_PROCESS_FOR_PCD_DAEMON_PORT    0x4D09
#define VIDEO_PROCESS_FOR_POSTSD_DAEMON_PORT 0x4D10
#define POST_STOARGE_DAEMON_PORT            0x4D11
#define SDI_PROCESS_DAEMON_PORT             0x4D12
#define SPD_FRAME_DAEMON_PORT               0x4D13
#define VIDEO_PROCESS_FOR_MMD_DAEMON_PORT   0x4D14
#define CM_CONTROL_DAEMON_PORT              0x4D15

#define CONTROLLER_PORT                     0x4D20
#define PRODUCING_PORT                      0x4D21
#define VAR_PORT                            0x4D22
#define EMS_PORT                            0x4D23
#define DAEMONVIEWER_PORT                   0x4D24
#define BUFFER_VIEWER_PORT                  0x4D25
#define LOGVIEWER_PORT                      0x4D26

#define VP_BUFFER_MAXSIZE	10

// Packet Separator Type
enum {
    PACKETTYPE_JSON,
    PACKETTYPE_BINALY,
    PACKETTYPE_ABAILABLE_INQUIRY_VP = 20,
    PACKETTYPE_VP_SET_BUFFERCOUNT,
    PACKETTYPE_VP_ALIVE,
    PACKETTYPE_CALIBRATION_VALUE = 100,

    PACKETTYPE_SIZE,
};

//Client Port
enum {
    MESSAGELTRANSFER_DAEMON,
    ENTERPRISE_MONITOR_DAEMON,
    SWITCH_CONTROL_DAEMON,
    CAMERA_CONTROL_DAEMON,
    GIMBAL_CONTROL_DAEMON,
    SDI_PROCESS_DAEMON,
    PROCESS_CONTROL_DAEMON,
    PRE_STOARGE_DAEMON,
    POST_STOARGE_DAEMON,
    VIDEO_PROCESS_FORPRESD_DAEMON,
    VIDEO_PROCESS_FORPCD_DAEMON,
    VIDEO_PROCESS_FORPOSTSD_DAEMON,
    CORE_MODULE_DAEMON,
    SOCKNUM_CLIENT_SIZE,
};

// Server Port
enum {
    SOCKNUM_CONTROLLER_APP,
    SOCKNUM_PRODUCING_APP,
    SOCKNUM_VAR_APP,
    SOCKNUM_EMS_APP,
    SOCKNUM_DAEMONVIEWER_APP,
    SOCKNUM_BUFFERVIWER_APP,
	SOCKNUM_LOGVIEWER_APP,
    SOCKNUM_SERVER_SIZE,
};

enum {
    FRAME_TYPE_None,
    FRAME_TYPE_IFRAME,
    FRAME_TYPE_PFRAME,
    FRAME_TYPE_END,
	FRAME_TYPE_FRAME,       //File to frame
	FRAME_TYPE_STREAM,      //Stream to frame
};

static char arrDaemonObject[20][100] = {
    "MTd",            
    "EMd",            
    "SCd",            
    "CCd",            
    "GCd",
    "SPd",
    "PCd",            
    "PreSd",
    "PostSd",
    "VPd",
    "VPd",
    "VPd",
    "CMd"
};


static char arrModelObject[11][100] = {
    "4DDM",                     // Controller Daemon
    "4DPD",                     // Producing Daemon
    "VARApp",                   // VAR Daemon
    "EMSApp",                   // EMS Daemon
    "DaemonViewer",             // DaemonViwer
    "4DBUFFER",             // DaemonBufferViewer
	"4DLOG",                    // LogViewer
};

#define MTDPROTOCOL_SECTION1    "Section1"
#define MTDPROTOCOL_SECTION2    "Section2"
#define MTDPROTOCOL_SECTION3    "Section3"
#define MTDPROTOCOL_SENDSTATE   "SendState"
#define MTDPROTOCOL_FROM        "From"
#define MTDPROTOCOL_TO          "To"
#define MTDPROTOCOL_ACTION      "Action"
#define MTDPROTOCOL_TOKEN       "Token"
#define MTDPROTOCOL_RESULTCODE  "ResultCode"
#define MTDPROTOCOL_ERRORMSG    "ErrorMsg"
#define MTDPROTOCOL_TARGETIP    "TargetIP"

#define MTDPROTOCOL_PRESET      "Preset"

#define MTDPROTOCOL_SENDSTATE_REQUEST       "request"
#define MTDPROTOCOL_SENDSTATE_RESPONSE      "response"

#define MTDHEADERSIZE           5
//#define MTDPROTOCOL                           "YJS"

#pragma pack(push, 1)  
struct MTdProtocolHeader
{
    int nSize;
    char cSeparator;
};

#pragma pack(pop)  

struct Version {
    Version(std::string versionStr)
    {
#ifdef _WIN32
        sscanf_s(versionStr.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);
#else
        sscanf(versionStr.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);
#endif
    }
    bool operator<(const Version& otherVersion)
    {
        if (major < otherVersion.major)
            return true;
        if (minor < otherVersion.minor)
            return true;
        if (revision < otherVersion.revision)
            return true;
        if (build < otherVersion.build)
            return true;
        return false;
    }
    bool operator == (const Version& other)
    {
        return major == other.major
            && minor == other.minor
            && revision == other.revision
            && build == other.build;
    }
    int major, minor, revision, build;
};
