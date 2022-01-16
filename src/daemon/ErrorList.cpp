#include "ErrorList.h"

void GetCommonErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case COMMON_ERR_NONE:					strError = ""; break;
	case COMMON_ERR_PARSE_FAIL:				strError = "Message Parsing Fail"; break;
	case COMMON_ERR_NOT_FOUND_SEC1:			strError = "Not Found: Section 1"; break;
	case COMMON_ERR_NOT_FOUND_SEC2:			strError = "Not Found: Section 2"; break;
	case COMMON_ERR_NOT_FOUND_SEC3:			strError = "Not Found: Section 3"; break;
	case COMMON_ERR_NOT_FOUND_SENDSTATE:	strError = "Not Found: Sendstate"; break;
	case COMMON_ERR_NOT_FOUND_TOKEN:		strError = "Not Found: Token"; break;
	case COMMON_ERR_NOT_FOUND_FROM:			strError = "Not Found: From"; break;
	case COMMON_ERR_NOT_FOUND_TO:			strError = "Not Found: To"; break;
	case COMMON_ERR_NOT_FOUND_ACTION:		strError = "Not Found: Action"; break;
	case COMMON_ERR_UNKNOWN_SEC1:			strError = "Unknown: Section 1"; break;
	case COMMON_ERR_UNKNOWN_SEC2:			strError = "Unknown: Section 2"; break;
	case COMMON_ERR_UNKNOWN_SEC3:			strError = "Unknown: Section 3"; break;
	case COMMON_ERR_UNKNOWN_SENDSTATE:		strError = "Unknown: Sendstate"; break;
	case COMMON_ERR_UNKNOWN_TOKEN:			strError = "Unknown: Token"; break;
	case COMMON_ERR_UNKNOWN_FROM:			strError = "Unknown: From"; break;
	case COMMON_ERR_UNKNOWN_TO:				strError = "Unknown: To"; break;
	case COMMON_ERR_UNKNOWN_ACTION:			strError = "Unknown: Action"; break;
	case COMMON_ERR_UNKNOWN_TYPE:			strError = "Unknown: Type"; break;
	case COMMON_ERR_TYPE_NAME_STRING:		strError = "Value Name is not String"; break;
	default:								strError = "Undefined Error"; break;
	}
}

void GetMtdErrorString(const int nErrorCode, std::string& strError) 
{
	switch (nErrorCode) {
	case MTD_ERR_DAEMON_NOT_CONNECTED:		strError = "Daemon is not Connected"; break;
	case MTD_ERR_APP_NOT_CONNECTED:			strError = "Application is not Connected"; break;
	case MTD_ERR_NOT_FOUND_DAEMONLIST:		strError = "Not Found: DaemonList"; break;
	default:								strError = "Undefined Error"; break;
	}
}

void GetPcdErrorString(const int nErrorCode, std::string& strError) 
{
	switch (nErrorCode) {
	case PCD_ERR_NOT_FOUND_ADJUSTID:		strError = "Not Found: AdjustID"; break;
	case PCD_ERR_NOT_CONNECTED_PRESD:		strError = "PreSd is not Connected"; break;
	case PCD_ERR_NOT_CONNECTED_POSTSD:		strError = "PostSd is not Connected"; break;
	case PCD_ERR_NOT_CONNECTED_VPD:			strError = "VPd is not Connected"; break;
	case PCD_ERR_INVALID_VPD_INFO:			strError = "Invalid VPd Info"; break;
	case PCD_ERR_PRESD_LOOPBACK_ADDR:		strError = "PreSd has Loopback Address(127.0.0.1)"; break;
	case PCD_ERR_POSTSD_LOOPBACK_ADDR:		strError = "PostSd has Loopback Address(127.0.0.1)"; break;
	case PCD_ERR_VPD_LOOPBACK_ADDR:			strError = "VPd has Loopback Address(127.0.0.1)"; break;
	case PCD_ERR_PRESD_COUNT_ZERO:			strError = "PreSd Count: 0"; break;
	case PCD_ERR_PRESD_ASSIGNED_VPD_COUNT:	strError = "Incorrect Presd-VPd Mapping"; break;
	default:								strError = "Undefined Error"; break;
	}
}

void GetEmdErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case EMD_ERR_NOT_FOUND_IPARR:					strError = "Not Found: IPArray"; break;
	case EMD_ERR_NOT_FOUND_IPARR_IP:				strError = "Not Found: IP(IPArray)"; break;
	case EMD_ERR_NOT_FOUND_IPARR_INDEX:				strError = "Not Found: Index(IPArray)"; break;
	case EMD_ERR_NOT_FOUND_IPARR_TYPE:				strError = "Not Found: Type(IPArray)"; break;
	case EMD_ERR_NOT_FOUND_RECORD:					strError = "Not Found: Record"; break;
	case EMD_ERR_NOT_FOUND_DELLIST:					strError = "Not Found: DelList"; break;
	case EMD_ERR_NOT_FOUND_CALID:					strError = "Not Found: CalID"; break;
	case EMD_ERR_NOT_FOUND_CALDATA:					strError = "Not Found: CalData"; break;
	case EMD_ERR_NOT_FOUND_DELCALID:				strError = "Not Found: DelCalID"; break;
	case EMD_ERR_NOT_FOUND_TYPE:					strError = "Not Found: Type"; break;
	case EMD_ERR_NOT_FOUND_DATA:					strError = "Not Found: Data"; break;
	case EMD_ERR_NOT_FOUND_MOVIE:					strError = "Not Found: Movie"; break;
	case EMD_ERR_NOT_FOUND_MOVIEARR:				strError = "Not Found: MovieArray"; break;

	case EMD_ERR_EMPTY_IPARR:						strError = "IPArray is Empty"; break;
	case EMD_ERR_EMPTY_MOVIEARR:					strError = "MovieArray is Empty"; break;

	case EMD_ERR_TYPE_IPARR_IP:						strError = "Invalid Type: IP(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_INDEX:					strError = "Invalid Type: Index(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_TYPE:					strError = "Invalid Type: Type(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_GCDID:					strError = "Invalid Type: GCd_id(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_SCDID:					strError = "Invalid Type: SCd_id(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_VPDID:					strError = "Invalid Type: VPd_id(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_PRESDID:				strError = "Invalid Type: PreSd_id(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_POSTSDID:				strError = "Invalid Type: PostSd_id(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_SWITCHPORT:				strError = "Invalid Type: SwitchPort(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_PRESDPATH:				strError = "Invalid Type: PreSd_path(IPArray)"; break;
	case EMD_ERR_TYPE_IPARR_POSTSDPATH:				strError = "Invalid Type: PostSd_path(IPArray)"; break;
	case EMD_ERR_TYPE_RECORD_NAME:					strError = "Invalid Type: Name(Record)"; break;
	case EMD_ERR_TYPE_RECORD_CALID:					strError = "Invalid Type: CalID(Record)"; break;
	case EMD_ERR_TYPE_DELLIST:						strError = "Invalid Type: DelList"; break;
	case EMD_ERR_TYPE_DELLIST_VALUE:				strError = "Invalid Type: Delist Value"; break;
	case EMD_ERR_TYPE_CALID:						strError = "Invalid Type: CalID"; break;
	case EMD_ERR_TYPE_CALDATA_SUMMARY_NAME:			strError = "Invalid Type: Name(summary)"; break;
	case EMD_ERR_TYPE_CALDATA_SUMMARY_COMMENT:		strError = "Invalid Type: Comment(summary)"; break;
	case EMD_ERR_TYPE_CALDATA_SUMMARY_PTSTYPE:		strError = "Invalid Type: Pts_type(summary)"; break;
	case EMD_ERR_TYPE_CALDATA_SUMMARY_WIDTH:		strError = "Invalid Type: width(summary)"; break;
	case EMD_ERR_TYPE_CALDATA_SUMMARY_HEIGHT:		strError = "Invalid Type: height(summary)"; break;
	case EMD_ERR_TYPE_CALDATA_SUMMARY_STADIUM:		strError = "Invalid Type: Stadium(summary)"; break;
	case EMD_ERR_TYPE_CALDATA_SUMMARY_WORLDCOORDS:	strError = "Invalid Type: world_coords(summary)"; break;
	case EMD_ERR_TYPE_CALDATA_PTSLIST:				strError = "Invalid Type: PtsList"; break;
	case EMD_ERR_TYPE_CALDATA_PTSLIST_DSCID:		strError = "Invalid Type: DscID(PtsList)"; break;
	case EMD_ERR_TYPE_CALDATA_PTSLIST_PTS:			strError = "Invalid Type: pts(PtsList)"; break;
	case EMD_ERR_TYPE_CALDATA_PTSLIST_IMAGE:		strError = "Invalid Type: image(PtsList)"; break;
	case EMD_ERR_TYPE_DELCALID:						strError = "Invalid Type: DelCalID"; break;
	case EMD_ERR_TYPE_DELCALID_VALUE:				strError = "Invalid Type: DelCalID Value"; break;
	case EMD_ERR_TYPE_TYPE:							strError = "Invalid Type: Type"; break;
	case EMD_ERR_TYPE_DATA:							strError = "Invalid Type: Data"; break;
	case EMD_ERR_TYPE_MOVIE_NAME:					strError = "Invalid Type: Name(Movie)"; break;
	case EMD_ERR_TYPE_MOVIE_SAVEPATH:				strError = "Invalid Type: SavePath(Movie)"; break;
	case EMD_ERR_TYPE_MOVIE_RECORDNAME:				strError = "Invalid Type: RecordName(Movie)"; break;
	case EMD_ERR_TYPE_MOVIE_USESDI:					strError = "Invalid Type: UseSDI(Movie)"; break;
	case EMD_ERR_TYPE_MOVIE_DURATION:				strError = "Invalid Type: Duration(Movie)"; break;
	case EMD_ERR_TYPE_MOVIE_STATUS:					strError = "Invalid Type: Status(Movie)"; break;
	case EMD_ERR_TYPE_MOVIEARR:						strError = "Invalid Type: MovieArray"; break;

	case EMD_ERR_INVALID_RECORD_NAME:				strError = "Invalid Value: Name(Record)"; break;
	case EMD_ERR_INVALID_RECORD_CALID:				strError = "Invalid Value: CalID(Record)"; break;
	case EMD_ERR_INVALID_CALID:						strError = "Invalid Value: CalID"; break;

	case EMD_ERR_DB_OPEN:							strError = "DB: Open"; break;
	case EMD_ERR_DB_SELECT:							strError = "DB: Select"; break;
	case EMD_ERR_DB_EXECUTE:						strError = "DB: Execute"; break;
	case EMD_ERR_DB_RESULT:							strError = "DB: Result"; break;
	case EMD_ERR_DB_NOT_EXIST_GCD:					strError = "DB: Not Exist GCd"; break;
	case EMD_ERR_DB_NOT_EXIST_POSTSD:				strError = "DB: Not Exist PostSd"; break;
	case EMD_ERR_DB_NOT_EXIST_PRESD:				strError = "DB: Not Exist PreSd"; break;
	case EMD_ERR_DB_NOT_EXIST_SCD:					strError = "DB: Not Exist SCd"; break;
	case EMD_ERR_DB_NOT_EXIST_REPLAYMOVIE:			strError = "DB: Not Exist ReplayMovie"; break;
	case EMD_ERR_DB_NOT_EXIST_VPD:					strError = "DB: Not Exist VPd"; break;
	default:										strError = "Undefined Error"; break;
	}
}

void GetScdErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case SCD_ERR_NOT_FOUND_SWITCHARRAY:		strError = "Not Found: Switches"; break;

	case SCD_ERR_TELNET_CONNECT:			strError = "Telnet Connect Error"; break;
	case SCD_ERR_TELNET_LOGIN:				strError = "Telnet Login Error"; break;
	case SCD_ERR_TELNET_ID:					strError = "Telnet ID Error"; break;
	case SCD_ERR_TELNET_PASSWORD:			strError = "Telnet Password Error"; break;
	case SCD_ERR_TELNET_CONFIG:				strError = "Telnet Config Error"; break;
	case SCD_ERR_TELNET_POEOFF:				strError = "Telnet Switch Off Error"; break;
	case SCD_ERR_TELNET_POEON:				strError = "Telnet Switch On Error"; break;
	case SCD_ERR_TELNET_GETMAC:				strError = "Telnet Get MacAddress Error"; break;
	case SCD_ERR_TELNET_CLEAR:				strError = "Telnet Clear Error"; break;
	case SCD_ERR_TELNET_FINDPOWER:			strError = "Telnet Find Power Port Error"; break;
	case SCD_ERR_TELNET_SHOW_MACADDRESS:	strError = "Telnet Show MacAddress Error"; break;
	case SCD_ERR_TELNET_SHOWPORT:			strError = "Telnet Show Port Error"; break;
	case SCD_ERR_TELNET_FIND_PORTNUM:		strError = "Telnet Find Port Number(Size) Error"; break;
	case SCD_ERR_TELNET_SET_RANGE:			strError = "Telnet Set Range Error Error"; break;
	case SCD_ERR_TELNET_COPY_CONFIG:		strError = "Telnet Copy Config Error"; break;
	case SCD_ERR_TELNET_RELOAD:				strError = "Telnet Reload Error"; break;
	case SCD_ERR_TELNET_NEGOTIATE:			strError = "Telnet Negotiate Error"; break;
	case SCD_ERR_TELNET_STATE_AGREEMENT:	strError = "Telnet State Agreement Error"; break;
	case SCD_ERR_TELNET_PRIVILEGE:			strError = "Telnet Privilege Error"; break;

	case SCD_ERR_SWITCH_MODEL:				strError = "Telnet: Unknown Switch Model"; break;
	case SCD_ERR_SWITCH_COMMAND:			strError = "Telnet: Command Error"; break;
	case SCD_ERR_TELNET_GET_SWITCH_MODEL:	strError = "Telnet: Get Switch Model Fail"; break;
	case SCD_ERR_PING_CHECK_ERR:			strError = "Telnet: Ping Check Error"; break;
	case SCD_ERR_TELNET_END:				strError = "Telnet: End Error"; break;
	case SCD_ERR_NOT_MATCH_IPARRAY:			strError = "The Switch does not have IP matching the IP array."; break;
	case SCD_SWITCHARRAY_SIZE_ZERO:			strError = "Switch Array Size Zero"; break;
	default:								strError = "Undefined Error"; break;
	}
}

void GetCcdErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case CCD_ERR_NOT_FOUND_CAMERAS:				strError = "Not Found: Cameras"; break;
	case CCD_ERR_NOT_FOUND_IPADDR:				strError = "Not Found: IPAddress"; break;
	case CCD_ERR_NOT_FOUND_TYPE:				strError = "Not Found: Type"; break;
	case CCD_ERR_NOT_FOUND_STATUS:				strError = "Not Found: Status"; break;
	case CCD_ERR_NOT_FOUND_DELAY_IP:			strError = "Not Found: ip(delay)"; break;
	case CCD_ERR_NOT_FOUND_DELAY_DELAY:			strError = "Not Found: delay(delay)"; break;
	case CCD_ERR_NOT_FOUND_CAMERA_MODEL:		strError = "Not Found: CameraModel"; break;
	case CCD_ERR_NOT_FOUND_FIRMWARE_DIR:		strError = "Not Found Firmware Directory"; break;
	case CCD_ERR_NOT_FOUND_FIRMWARE_VERSION:	strError = "Not Found: FirmwareVersion"; break;
	case CCD_ERR_NOT_FOUND_PRESET:				strError = "Not Found: Preset"; break;

	case CCD_ERR_TYPE_CAMERAS:					strError = "Invalid Type: Cameras"; break;
	case CCD_ERR_TYPE_CAMERAS_VALUE:			strError = "Invalid Type: Cameras value"; break;
	case CCD_ERR_TYPE_CAMERAS_TYPE:				strError = "Invalid Type: Type"; break;
	case CCD_ERR_TYPE_STATUS:					strError = "Invalid Type: Status"; break;
	case CCD_ERR_TYPE_IPADDR:					strError = "Invalid Type: IPAddress"; break;
	case CCD_ERR_TYPE_CAMERA_CONFIG_INFO:		strError = "Invalid Type: Camera Config Info"; break;
	case CCD_ERR_TYPE_TIME:						strError = "Invalid Type: time"; break;
	case CCD_ERR_TYPE_SYNCTIME:					strError = "Invalid Type: synctime"; break;
	case CCD_ERR_TYPE_SYNCLIMIT:				strError = "Invalid Type: synclimit"; break;
	case CCD_ERR_TYPE_SYNCSKIP:					strError = "Invalid Type: syncskip"; break;
	case CCD_ERR_TYPE_ACTIVE:					strError = "Invalid Type: active"; break;
	case CCD_ERR_TYPE_DELAY:					strError = "Invalid Type: delay"; break;
	case CCD_ERR_TYPE_DELAY_VALUE:				strError = "Invalid Type: delay value"; break;
	case CCD_ERR_TYPE_DELAY_IP:					strError = "Invalid Type: ip(delay)"; break;
	case CCD_ERR_TYPE_DELAY_DELAY:				strError = "Invalid Type: delay(delay)"; break;
	case CCD_ERR_TYPE_FIRMWARE_VERSION:			strError = "Invalid Type: FirmwareVersion"; break;
	case CCS_ERR_TYPE_PRESET:					strError = "Invalid Type: Preset"; break;

	case CCD_ERR_EMPTY_CAMERAS:					strError = "Empty Cameras"; break;
	case CCD_ERR_EMPTY_ACTIVE:					strError = "Empty active"; break;
	case CCD_ERR_EMPTY_CAMERA_MODEL:			strError = "Empty CameraModel"; break;
	case CCD_ERR_EMPTY_FIRMWARE_VERSION:		strError = "Empty Firmware Version"; break;

	case CCD_ERR_INVALID_CAMERA_CONFIG_INFO:	strError = "Invalid Camera Config Info"; break;
	case CDD_ERR_INVALID_CAMERA_MODEL:			strError = "Invalid Camera Model"; break;

	case CCD_ERR_CREATE_PRESET_DIR:				strError = "Create Preset Diectory Error"; break;
	case CCD_ERR_WRITE_PRESET_FILE:				strError = "Write Preset File Error"; break;
	case CCD_ERR_NO_PRESET_FILE:				strError = "No Saved Preset"; break;

	default:									strError = "Undefined Error"; break;
	}
}
void GetGcdErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case GCD_ERR_NOT_FOUND_GIMBALS:		strError = "Not Found: Gimbals"; break;
	case GCD_ERR_TYPE_GIMBALS:			strError = "Invalid Type: Gimbals"; break;
	case GCD_ERR_TYPE_GIMBALS_VALUE:	strError = "Invalid Type: Gimbals Value"; break;
	case GCD_ERR_MISMATCH_MSG_CMD:		strError = "Mismatch Message Command"; break;
	case GCD_ERR_NOT_FOUND_PRESETNUM:	strError = "Not Found: Preset"; break;
	case GCD_ERR_INVALID_PRESETNUM:		strError = "Invalid Preset Number"; break;
	default:							strError = "Undefined Error"; break;
	}
}

void GetPreSdErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case PRESD_ERR_NOT_FOUND_RECORDNAME:	strError = "Not Found: RecordName"; break;
	case PRESD_ERR_NOT_FOUND_TIME:			strError = "Not Found: Time"; break;
	case PRESD_ERR_NOT_FOUND_PRESD_IP:		strError = "Not Found: ip(PreSd)"; break;
	case PRESD_ERR_NOT_FOUND_PRESD_RUN:		strError = "Not Found: run(PreSd)"; break;
	case PRESD_ERR_MISMATCH_RECORDNAME:		strError = "Mismatch Recordname"; break;

	case PRESD_ERR_OPENSESSION:				strError = "OpenSession Fail"; break;
	case PRESD_ERR_CLOSESESSION:			strError = "CloseSession Fail"; break;
	case PRESD_ERR_PREPARE:					strError = "Prepare Fail"; break;

	case PRESD_ERR_NOT_CONNECTED:			strError = "Not Connected"; break;
	case PRESD_ERR_NOT_PREPARED:			strError = "Not Prepared"; break;

	case PRESD_ERR_NOW_PREPARING:			strError = "Now Preparing"; break;
	case PRESD_ERR_NOW_STREAMING:			strError = "Now Streaming"; break;

	case PRESD_ERR_INVALID_GOP:				strError = "Invalid GOP"; break;
	case PRESD_ERR_EMPTY_SAVEPATH:			strError = "Empty SavePath"; break;
	case PRESD_ERR_EMPTY_RECORDNAME:		strError = "Empty RecordName"; break;
	case PRESD_ERR_INVALID_CAMERA_POSTSDIP:	strError = "Invalid Camera-PostSd IP"; break;
	case PRESD_ERR_START_AGENT:				strError = "Start Agent Fail"; break;

	case PRESD_ERR_MISMATCH_IPADDRESS:		strError = "Mismatch IPAddress"; break;

	default:								strError = "Undefined Error"; break;
	}
}

void GetPostSdErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case POSTSD_ERR_NOT_FOUND_LIVEINFO:					strError = "Not Found: LiveInfo"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_LIVE:			strError = "Not Found: live(LiveInfo)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_METHOD:			strError = "Not Found: metohd(LiveInfo)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_RECORD:			strError = "Not Found: record(LiveInfo)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_GROUPS:			strError = "Not Found: groups(LiveInfo)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_GROUPID:			strError = "Not Found: groupid(LiveInfo/groups)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_MULTIVIEW:		strError = "Not Found: multiview(LiveInfo/groups)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_VIDEO:			strError = "Not Found: Video(LiveInfo/groups)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_RESOLW:			strError = "Not Found: resolution_w(LiveInfo/groups/Video)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_RESOLH:			strError = "Not Found: resolution_h(LiveInfo/groups/Video)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_FRAMERATE:		strError = "Not Found: framerate(LiveInfo/groups/Video)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_BITRATE:			strError = "Not Found: bitrate(LiveInfo/groups/Video)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_GOP:				strError = "Not Found: gop(LiveInfo/groups/Video)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_CODEC:			strError = "Not Found: codec(LiveInfo/groups/Video)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVESERVERINFO:			strError = "Not Found: LiveServerInfo"; break;
	case POSTSD_ERR_NOT_FOUND_RECORDINFO:				strError = "Not Found: RecordInfo"; break;
	case POSTSD_ERR_NOT_FOUND_CAMERAS:					strError = "Not Found: Cameras"; break;
	case POSTSD_ERR_NOT_FOUND_POSITION_SWIPE:			strError = "Not Found: position_swipe"; break;
	case POSTSD_ERR_NOT_FOUND_POSITION_SWIPE_LENGTH:	strError = "Not Found: length(position_swipe)"; break;
	case POSTSD_ERR_NOT_FOUND_POSITION_SWIPE_DATA:		strError = "Not Found: position_swipe_data(position_swipe)"; break;
	case POSTSD_ERR_NOT_FOUND_POSITION_SWIPE_CHANNEL:	strError = "Not Found: positionswipe_channel(position_swipe)"; break;
	case POSTSD_ERR_NOT_FOUND_MULTIVIEW_ROI:			strError = "Not Found: multiview_roi"; break;
	case POSTSD_ERR_NOT_FOUND_MULTIVIEW_ROI_CHANNEL:	strError = "Not Found: multiview_roi_channel(multiview_roi)"; break;
	case POSTSD_ERR_NOT_FOUND_MULTIVIEW_ROI_LENGTH:		strError = "Not Found: length(multiview_roi)"; break;
	case POSTSD_ERR_NOT_FOUND_MULTIVIEW_ROI_DATA:		strError = "Not Found: multiview_roi_data(multiview_roi)"; break;
	case POSTSD_ERR_NOT_FOUND_TIME:						strError = "Not Found: Time"; break;
	case POSTSD_ERR_NOT_FOUND_POSTSD_IP:				strError = "Not Found: ip(PostSd)"; break;
	case POSTSD_ERR_NOT_FOUND_POSTSD_RUN:				strError = "Not Found: run(PostSd)"; break;

	case POSTSD_ERR_TYPE_LIVEINFO_GROUPS:				strError = "Invalid Type: groups(LiveInfo)"; break;
	case POSTSD_ERR_TYPE_POSITION_SWIPE:				strError = "Invalid Type: position_swipe"; break;
	case POSTSD_ERR_TYPE_POSITION_SWIPE_CHANNEL:		strError = "Invalid Type: positionswipe_channel"; break;
	case POSTSD_ERR_TYPE_MULTIVIEW_ROI:					strError = "Invalid Type: multiview_roi"; break;

	case POSTSD_ERR_CONNECT_FAIL:						strError = "Connect Fail"; break;
	case POSTSD_ERR_PREPARE_FAIL:						strError = "Prepare Fail"; break;

	case POSTSD_ERR_CONFIG_LIVE_ENABLE:					strError = "Config Live Enable Fail"; break;
	case POSTSD_ERR_CONFIG_RECORD_ENABLE:				strError = "Config Record Enable Fail"; break;
	case POSTSD_ERR_AVCONTROL_TIME:						strError = "Not Found: AV_Control Time"; break;
	case POSTSD_ERR_AVCONTROL_STATUS:					strError = "Invalid status: AV_Control Time"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_AR_FEATURE:      strError = "Not Found: AR_Feature(LiveInfo)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_AV_SYNCTIME:     strError = "Not Found: AV_Sync_Time(LiveInfo)"; break;

	case POSTSD_ERR_NOT_FOUND_LIVEINFO_CHANNEL:			strError = "Not Found: channel(LiveInfo/groups/audio)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_SAMPLEBIT:		strError = "Not Found: samplebit(LiveInfo/groups/audio)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_SAMPLERATE:		strError = "Not Found: samplerate(LiveInfo/groups/audio)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVEINFO_AUDIOCODEC:		strError = "Not Found: codec(LiveInfo/groups/audio)"; break;
	case POSTSD_ERR_NOT_FOUND_LIVESERVERINFO_MEDIA:		strError = "Not Found: media(LiveServerInfo)"; break;

	case POSTSD_ERR_NOT_FOUND_SYNCCONTROL_LIVEINDEX :   strError = "Not Found: (Sync_Control/liveindex)"; break;
	case POSTSD_ERR_NOT_FOUND_SYNCCONTROL_LIVE :        strError = "Not Found: (Sync_Control/live)"; break;
	case POSTSD_ERR_NOT_FOUND_SYNCCONTROL_CONTROLTIME : strError = "Not Found: (Sync_Control/av_control_time)"; break;
	case POSTSD_ERR_CONTROLTIME_APPLY_FAIL :            strError = "Sync Fail: Can't be applied"; break;
	case POSTSD_ERR_CONTROLTIME_OUTOFRANGE :            strError = "Sync Fail: Out of range Sync_time value"; break;

	default:											strError = "Undefined Error"; break;
	}
}

std::string GetErrorCodeToString(const int nErrorCode)
{
	std::string strError = "";
	if (nErrorCode != COMMON_ERR_NONE) {
		if (nErrorCode > COMMON_ERR_NONE && nErrorCode < COMMON_ERR_END) { GetCommonErrorString(nErrorCode, strError); }
		else if (nErrorCode > MTD_ERR_START && nErrorCode < MTD_ERR_END) { GetMtdErrorString(nErrorCode, strError); }
		else if (nErrorCode > PCD_ERR_START && nErrorCode < PCD_ERR_END) { GetPcdErrorString(nErrorCode, strError); }
		else if (nErrorCode > EMD_ERR_START && nErrorCode < EMD_ERR_END) { GetEmdErrorString(nErrorCode, strError); }
		else if (nErrorCode > SCD_ERR_START && nErrorCode < SCD_ERR_END) { GetScdErrorString(nErrorCode, strError); }
		else if (nErrorCode > CCD_ERR_START && nErrorCode < CCD_ERR_END) { GetCcdErrorString(nErrorCode, strError); }
		else if (nErrorCode > GCD_ERR_START && nErrorCode < GCD_ERR_END) { GetGcdErrorString(nErrorCode, strError); }
		else if (nErrorCode > PRESD_ERR_START && nErrorCode < PRESD_ERR_END) { GetPreSdErrorString(nErrorCode, strError); }
		else if (nErrorCode > POSTSD_ERR_START && nErrorCode < POSTSD_ERR_END) { GetPostSdErrorString(nErrorCode, strError); }
		else strError = "Undefined Error";
	}

	return strError;
}
