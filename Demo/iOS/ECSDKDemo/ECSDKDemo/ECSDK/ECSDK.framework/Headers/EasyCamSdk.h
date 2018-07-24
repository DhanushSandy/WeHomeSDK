#pragma once

#ifdef _WIN32
#ifdef EASYCAM_API_EXPORTS
#define EASYCAM_API __declspec(dllexport)
#else
#define EASYCAM_API __declspec(dllimport)
#endif  
#else
#define EASYCAM_API
#endif // _WIN32



#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

	// Login result
#define LOGIN_RESULT_SUCCESS 				    0
#define LOGIN_RESULT_CONNECT_FAIL			(-1)
#define LOGIN_RESULT_AUTH_FAIL				(-2)
#define LOGIN_RESULT_EXCEED_MAX_CONNECTION	(-3)
#define LOGIN_RESULT_RESULT_FORMAT_ERROR	(-4)
#define LOGIN_RESULT_FAIL_UNKOWN			    (-5)


	// Send command result
#define CMD_SEND_RESULT_SUCCESS            (0)
#define CMD_SEND_RESULT_CONNECT_FAIL       (-1)   // Connect to remote failed
#define CMD_SEND_RESULT_SEND_TIMEOUT       (-2)   // Send data timeout
#define CMD_SEND_RESULT_BUF_FULL           (-3)   // Command send buffer full
#define CMD_SEND_RESULT_PARAM_ERROR        (-4)   // Command format malformed
#define CMD_SEND_RESULT_FAIL_UNKOWN        (-5)   // Unknown error

	// Commond exec result
#define CMD_EXEC_RESULT_SUCCESS             0
#define CMD_EXEC_RESULT_FORMAT_ERROR        (-1)  //
#define CMD_EXEC_RESULT_SEND_FAIL           (-2)  //
#define CMD_EXEC_RESULT_AUTH_FAIL           (-3)  //

	// Connect type
#define CONNECT_TYPE_LAN                    (1<<0)
#define CONNECT_TYPE_P2P                    (1<<1)
#define CONNECT_TYPE_RELAY                  (1<<2)
    
enum EC_SDK_CMD_ID
{
    // talk
    EC_SDK_CMD_ID_OPEN_TALK = 0x100,    //Open remote device’s talk function
    EC_SDK_CMD_ID_CLOSE_TALK,           //Close remote device’s talk function
    
    //cmd
    EC_SDK_CMD_ID_GET_DEV_INFO = 0x200, //Get device information, eg, firmware version, device name and etc.
    EC_SDK_CMD_ID_SET_DEV_INFO,         //Save device information to remote.
    EC_SDK_CMD_ID_CHANGE_PASSWORD,      //Change password of remote device
    EC_SDK_CMD_ID_VIDEO_MIRROR,         //Mirror the video for remote device
    EC_SDK_CMD_ID_VIDEO_FLIP,           //Flip the video of remote device
    EC_SDK_CMD_ID_GET_PIR_INFO,         //Get PIR settings
    EC_SDK_CMD_ID_SET_PIR_INFO,         //Set PIR settings
    EC_SDK_CMD_ID_RESTORE_TO_FACTORY,   //Restore remote device to factory setting
    EC_SDK_CMD_ID_FORMAT_SD_CARD,       //Format TF card mounted in remote device
    
    
    //play record
    EC_SDK_CMD_ID_GET_RECORD_BITMAP_BY_MON = 0x300, // Query which day contains record for special months
    EC_SDK_CMD_ID_GET_PLAY_RECORD_LIST,             //Query record list by date
    EC_SDK_CMD_ID_START_PLAY_EVENT_RECORD,          //Play remote video clip by event id and event time.
    EC_SDK_CMD_ID_START_PLAY_RECORD,                 //Play remote video clip by time.
    EC_SDK_CMD_ID_STOP_PLAY_RECORD,                  //Stop play remote video clip.
    
    //upgrade
    EC_SDK_CMD_ID_UPGRADE = 0x400,                   //Upgrade the firmware of a remote device
};

enum PAYLOAD_TYPE
{
    PAYLOAD_TYPE_VIDEO_H264,
    PAYLOAD_TYPE_VIDEO_H265,
    PAYLOAD_TYPE_VIDEO_MJPEG,
    PAYLOAD_TYPE_AUDIO_PCM = 0x10,
    PAYLOAD_TYPE_AUDIO_ADPCM,
    PAYLOAD_TYPE_AUDIO_G711,
    PAYLOAD_TYPE_AUDIO_AAC,
    PAYLOAD_TYPE_AUDIO_G726,
};

enum EC_NAT_TYPE
{
    EC_NAT_TYPE_UNKNOWN = 0,
    EC_NAT_TYPE_IP_RESTRICTED_CLONE = 1,
    EC_NAT_TYPE_PORT_RESTRICTED_CLONE = 2,
    EC_NAT_TYPE_SYMMETRIC = 3,
};
    
typedef enum TAG_ECSUB_RETURN
{
    ECSUB_RETURN_FALES = -1,
    ECSUB_RETURN_OK = 0,
}ECSUB_RETURN;

typedef struct tagDeviceInfo
{
    int devType; //0=Camera 1=DoorBell
    char uid[32];
    char devName[64];
    char fwVer[64];
}DeviceInfo;

typedef struct tagEC_INIT_INFO
{
    void(*lpLoginResult)(int handle, int errorCode, int seq, unsigned int notificationToken, unsigned int isCharging, unsigned int batPercent);
    void(*lpCmdResult)(int handle, char* data, int errorCode, int seq);
    void(*lpAudio_RecvData)(int handle, char *data, int len, int payloadType, long long timestamp, int seq);
    void(*lpVideo_RecvData)(int handle, char *data, int len, int payloadType, long long timestamp, int seq, int frameType, int videoWidth, int videoHeight, unsigned int wifiQuality);
    void(*lpPBAudio_RecvData)(int handle, char *data, int len, int payloadType, long long timestamp, int seq, int pbSessionNo);
    void(*lpPBVideo_RecvData)(int hanlde, char *data, int len, int payloadType, long long timestamp, int seq, int frameType, int videoWidth, int videoHeight, int pbSessionNo);
    void(*lpPBEnd)(int hanlde, int pbSessionNo);
    void(*lpFileDownload_RecvData)(int handle, char* data, int len, int sessionNo);
    void(*lpPIRData_RecvData)(int handle, long long timeMS, short adc);
}EC_INIT_INFO;

#define ONLINE_QUERY_RESULT_SUCCESS                 (0)   /* The device is online*/
#define ONLINE_QUERY_RESULT_FAIL_ALREADY_RUNNING    (-1)  /* Online query is in progress*/
#define ONLINE_QUERY_RESULT_FAIL_SERVER_NO_RESP     (-2)  /* Network error */
#define ONLINE_QUERY_RESULT_FAIL_UNKOWN             (-100) /* Unexpected error */
typedef void(*OnlineQueryResultCallback)(int queryResult, const char* uid, int lastLoginSec);

EASYCAM_API void EC_GetVersion(char* version);

/*
 * This API should be called before anything
 * */
EASYCAM_API int EC_Initialize(EC_INIT_INFO* init);
EASYCAM_API void EC_DeInitialize(void);

/* Query device online status */
EASYCAM_API int EC_QueryOnlineStatus(const char* uid, OnlineQueryResultCallback callback);

/* uid: Device UID
 * usrName: always "admin" for these products
 * password: Device access password
 * broadcastAddr: Lan broadcast address, for example: 192.168.1.255
 * seq: command sequence, will send back at Login callback function
 * needVideo: Should send video stream after login?
 * needAudio: Should send audio stream after login?
 * connectType: For normal use, should always be: CONNECT_TYPE_LAN|CONNECT_TYPE_P2P|CONNECT_TYPE_RELAY
 * timeout: login timeout value in unit of micro second
 *
 * return: Session handle
*/
EASYCAM_API int EC_Login(const char* uid, const char* usrName, const char* password, const char* broadcastAddr,
    int seq, int needVideo, int needAudio, int connectType, int timeout);

/* handle: Session handle returned by EC_Login
 *
 * return: 0=Success others mean fail
 * */
EASYCAM_API int EC_Logout(int handle);

/* Search device on the LAN
 * timeoutMS: timeout value
 * bCastAddr: Lan broadcast address, for example: 192.168.1.255
 *
 * Note:
 *   This API will block some time, should not be called in UI thread
 *   The device can not be searched when in sleep mode
 * */
EASYCAM_API int EC_StartSearchDev(int timeoutMS, char* bCastAddr);
/*
 * Interrupt the search progress
 * */
EASYCAM_API int EC_StopDevSearch(void);
/*
 * This API should be called after EC_StartSearchDev to get the device list
 * */
EASYCAM_API int EC_GetDevList(DeviceInfo* pDevInfo, int devInfoSize);

/*
 * Send command to remote device
 * handle: Session handle
 * command: the command content in json format, get detail info at document
 * seq: Sequence will be sent back at Command result callback
 * */
EASYCAM_API int EC_SendCommand(int handle, char* command, int seq);

/*
 * Send talk data to remote device
 * handle: Session handle
 * data: PCM audio data
 * dataLen: data length in bytes
 * payloadType: Not in using now
 * seq: Not in using now
 * */
EASYCAM_API int EC_SendTalkData(int handle, char* data, int dataLen, int payloadType, int seq);

// Detect NAT type
EASYCAM_API int EC_Detect_Network_Type(const char* uid);

/*
 * Subscribe & Unsubscribe APIs
 * uid: device UID
 * appName: APP name, for example: "WeHome", should be registered on Push server firstly
 * agName: Message type, Option is {"DPS", "FCM", "APNS"}
 * phoneToken: DPS Token or FCM token or APNS token
 * eventCh: The event channel returned by login callback function
 * Note: You need contact Linkwil to register your APP on Push server firstly, then you can receive notifications from the remote device
 * */
EASYCAM_API int EC_Subscribe(const char* uid, const char* appName, const char* agName, const char* phoneToken, unsigned int eventCh);
EASYCAM_API int EC_UnSubscribe(const char* uid, const char* appName, const char* agName, const char* phoneToken, unsigned int eventCh);
EASYCAM_API int EC_ResetBadge(const char* uid, const char* appName, const char* agName, const char* phoneToken, unsigned int eventCh);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */