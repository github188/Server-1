#ifndef __NET_SDK_H__
#define __NET_SDK_H__ 

#include "net_sdk_error.h"
#include "net_sdk_define.h"

/*
**sdk utils
*/
NET_SDK_API OS_BOOL SDKCALL NET_DEV_Init(OS_VOID);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_Cleanup(OS_VOID);
NET_SDK_API OS_UINT32 SDKCALL NET_DEV_GetLastError(OS_VOID);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_SetExceptionCallback(EXCP_INFO_CBF fExceptionCallback, void* pUser);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_ConfigSDKLog(NET_DEV_SDK_LOG* pLogPara);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_SetConnectTime(OS_UINT32 dwWaitTime);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_SetReconnect( OS_BOOL bEnable, OS_UINT32 dwInterval);
NET_SDK_API OS_INT32 SDKCALL NET_DEV_HandelConvert(ipcHandle handle);

NET_SDK_API OS_BOOL SDKCALL NET_DEV_StartSdkProxy(char* sIP1, OS_UINT16 wPort1, char* sIP2, OS_UINT16 wPort2);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_StopSdkProxy();

/*
**logon/logoff
*/
NET_SDK_API OS_INT32 SDKCALL NET_DEV_LoginWithProxy(char const *pDeviceIP, OS_UINT16 wDevicePort, OS_INT32 nDeviceID, char const *pUserName, char const *pPassword, char* pRes);
NET_SDK_API OS_INT32 SDKCALL NET_DEV_Login(char const *pDeviceIP, OS_UINT16 wDevicePort, char const *pUserName, char const *pPassword, char* pRes);
NET_SDK_API OS_BOOL SDKCALL  NET_DEV_Logout(OS_INT32 lUserID);
NET_SDK_API OS_BOOL SDKCALL  NET_DEV_PingDevice(char* sIP);


/*
**device alarm
*/
NET_SDK_API OS_INT32 SDKCALL NET_DEV_SetupAlarmChan(OS_INT32 lUserID, ALARM_CHAN_ARA* pPara, ALARM_INFO_CBF fDataCallback, OS_VOIDPTR pUserData) ;
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_CloseAlarmChan(OS_INT32 lAlarmHandle);
NET_SDK_API OS_INT32 SDKCALL NET_DEV_StartListen(char *sLocalIP, OS_UINT16 wLocalPort, ALARM_INFO_CBF  fDataCallback, OS_VOIDPTR pUserData);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_StopListen(OS_INT32 lListenHandle);

/*
**config
*/
NET_SDK_API OS_BOOL SDKCALL NET_DEV_SetDevConfig(OS_INT32 lUserID, OS_UINT32 dwCommand, OS_INT32 lIndex, OS_VOIDPTR lpInBuffer, OS_UINT32 dwInBufferSize, OS_UINT32 dwTimeOut = 0);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_GetDevConfig(OS_INT32 lUserID, OS_UINT32 dwCommand, OS_INT32 lINdex, OS_VOIDPTR lpOutBuffer, OS_UINT32 dwOutBufferSize, OS_UINT32 dwTimeOut = 0);

/*
system control
*/
NET_SDK_API OS_BOOL SDKCALL NET_DEV_ForceIDR(OS_INT32 lUserID, OS_INT32 nChannelID, OS_INT32 nStreamID);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_SystemControl(OS_INT32 lUserID, NET_DEV_SYSTEM_CONTROL* pControlPara);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_GetEncodeStreamCombine(OS_INT32 lUserID, OS_INT32 nChannelID, NET_DEV_ENCODE_STREAM_COMBINE* pConfig, OS_UINT32 dwTimeOut);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_SetEncodeStreamCombine(OS_INT32 lUserID, OS_INT32 nChannelID, NET_DEV_ENCODE_STREAM_COMBINE* pConfig, OS_UINT32 dwTimeOut);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_GetEncodeCfg(OS_INT32 lUserID, OS_INT32 nChannelID, NET_DEV_ENCODE_CFG* pConfig, OS_UINT32 dwTimeOut);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_SetEncodeCfg(OS_INT32 lUserID, OS_INT32 nChannelID, NET_DEV_ENCODE_CFG* pConfig, OS_UINT32 dwTimeOut);

/*
**matrix
*/
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_GetCurrentMonitorList(OS_INT32 lUserID, NET_MATRIX_MONITOR_LIST* pMonitorList);
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_GetCurrentLayout(OS_INT32 lUserID, OS_UINT32 dwMonitor, NET_MATRIX_LAYOUT *pLayout);
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_GetCurrentLayoutInfo(OS_INT32 lUserID, OS_UINT32 dwMonitor, OS_UINT32 dwWindow, char* pCameraName, OS_UINT32 dwMaxNameLen);
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_GetCurrentPlayMode(OS_INT32 lUserID, OS_UINT32 dwMonitor, OS_INT32 *pPlayMode);
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_StartLive(OS_INT32 lUserID, OS_UINT32 dwMonitor, NET_MATRIX_CAMERA_INFO *pCameraInfo);
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_StartLiveEx(OS_INT32 lUserID, OS_UINT32 dwMonitor, NET_MATRIX_CAMERA_INFO_EX *pCameraInfoEx);
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_StopLive(OS_INT32 lUserID, OS_UINT32 dwMonitor, OS_UINT32 dwWindow);

NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_SwitchLayout(OS_INT32 lUserID, OS_UINT32 dwMonitor, NET_MATRIX_LAYOUT *pLayout);    
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_StartSubgroup(OS_INT32 lUserID, OS_UINT32 dwMonitor, NET_MATRIX_SUBGROUP *pSubgroup);    
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_StartSwitchgroup(OS_INT32 lUserID, OS_UINT32 dwMonitor, NET_MATRIX_SUBGROUP_ARRAY *pSubgroupArray); 
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_ToggleFullScreen(OS_INT32 lUserID, OS_UINT32 dwMonitor, OS_UINT32 dwWindow);

NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_PauseSwitchgroup(OS_INT32 lUserID, OS_UINT32 dwMonitor);
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_ResumeSwitchgroup(OS_INT32 lUserID, OS_UINT32 dwMonitor);    
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_PlayNextSubgroup(OS_INT32 lUserID, OS_UINT32 dwMonitor);    
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_PlayPreviousSubgroup(OS_INT32 lUserID, OS_UINT32 dwMonitor);    
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_ResetSwitchgroup(OS_INT32 lUserID, OS_UINT32 dwMonitor);  
NET_SDK_API OS_BOOL SDKCALL NET_MATRIX_StopSwitchgroup(OS_INT32 lUserID, OS_UINT32 dwMonitor);    

/*
**play control command
*/
#define NET_DEV_PLAYSTART		        1        //start play
#define NET_DEV_PLAYPAUSE		        2        //pause
#define NET_DEV_PLAYFAST	        	3        //play fast
#define NET_DEV_PLAYSLOW	        	4        //play slow
#define NET_DEV_PLAYNORMAL		        5        //play 1X speed
#define NET_DEV_PLAYSETTIME		        6        //seek with absolute time
#define NET_DEV_PLAYGETTIME             7        //get played time
#define NET_DEV_PLAY_RESUME             8        //resume from pause

#define NET_DEV_PLAYFRAME		        10        //frame step
#define NET_DEV_PLAYSTARTAUDIO	        11       //open sound
#define NET_DEV_PLAYSTOPAUDIO	        12       //close sound
#define NET_DEV_PLAYAUDIOVOLUME     	13       //set sound volume 
#define NET_DEV_THROWBFRAME		        14       //throw b frame

NET_SDK_API OS_INT32 SDKCALL NET_DEV_PlayBack(OS_INT32 lUserID, NET_DEV_PALYBACK_PARA* pPara, PLAY_DATA_CBF fDataCallBack, void* pUser);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_PlayBackControl(OS_INT32 lPlayHandle, OS_UINT32 dwControlCode, OS_VOIDPTR lpInBuffer, OS_UINT32 dwInLen, OS_VOIDPTR lpOutBuffer, OS_UINT32* lpOutLen);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_StopPlayBack(OS_INT32 lPlayHandle);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_GetPlayBackPos(OS_INT32 lPlayHandle, OS_UINT32* pPosition);

/*
**remote realplay
*/
NET_SDK_API OS_INT32 SDKCALL NET_DEV_StartRealPlay(OS_INT32 lUserID, NET_DEV_REALPLAY_PARA *lpClientInfo, REAL_DATA_CBF fRealDataCallBack, void* pUser);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_StopRealPlay(OS_INT32 lRealHandle);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_PlayPause(OS_INT32 lRealHandle);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_PlayResume(OS_INT32 lRealHandle);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_GetPlayRect(OS_INT32 lRealHandle, NET_DEV_RECT* pRect);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_SetPlayRect(OS_INT32 lRealHandle, NET_DEV_RECT* pRect);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_SetDisplayRegion(OS_INT32 lRealHandle, OS_UINT32 lRegionNum, NET_DEV_RECT rcSrc, OS_BOOL bEnable);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_SetBufferTime(OS_INT32 lRealHandle, const OS_UINT32 lTime);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_SetPlayMode(OS_INT32 lRealHandle, const OS_UINT32 iMode);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_SetVolume(OS_INT32 lRealHandle, OS_UINT16 nVolume);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_StartImageSharpen(OS_INT32 lRealHandle,float fAmount);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_ChangeSharpenDegree(OS_INT32 lRealHandle,float fAmount);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_StopImageSharpen(OS_INT32 lRealHandle);
NET_SDK_API OS_BOOL  SDKCALL  NET_DEV_RegisterDrawCallBack(OS_INT32 lRealHandle, PLAY_DRAW_CBF pCallBack, OS_VOIDPTR pUser);
NET_SDK_API OS_BOOL  SDKCALL  NET_DEV_GetPictureSize(OS_INT32 lRealHandle,  OS_INT32* pWidth, OS_INT32* pHeight);
NET_SDK_API OS_BOOL  SDKCALL  NET_DEV_RealplayCapturePicture(OS_INT32 lRealHandle, OS_INT32 iPicType, char* sfileName);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_SaveRealPlayData(OS_INT32 lRealHandle, NET_DEV_SAVE_FILE_PARA* pPara);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_StopSaveRealPlayData(OS_INT32 lRealHandle);

/*
**remote file search result
*/
enum FILE_SERACH_RESULT
{
    NET_DEV_FILE_SUCCESS   = 1,	                                       //get file info
    NET_DEV_FILE_NOFIND	   = 2,	                                       //no file in time range
    NET_DEV_ISFINDING      = 3,		                                   //is searchinf file
    NET_DEV_NOMOREFILE	   = 4,		                                   //there is no more file
    NET_DEV_FILE_EXCEPTION = 5		                                   //exception when searching file
};

/*
 *search command
 */
enum SEARCH_CMD
{
	SEARCH_RECORD_FILE = 1,
	SEARCH_PICTURE,
};

/*
 *record file type
 */
enum RECORD_FILE_TYPE
{
    SEARCH_FILE_ALL       =    0x0,                                    //all type
    SEARCH_FILE_SCHEDULE  =    0x1,                                    //record by schedule
    SEARCH_FILE_MANUAL    =    0x2,                                    //manual record
    
    SEARCH_FILE_MOTION    =    0x100,                                  //all event
    SEARCH_FILE_MASK      =    0x101,                                  //motion detect

    SEARCH_PIC_ALL        =    0x1000,                                 //all picture
    SEARCH_PIC_MOTION     =    0x1001,                                 //motion picture
};

NET_SDK_API OS_INT32 SDKCALL NET_DEV_FindFile(OS_INT32 lUserID, NET_DEV_FIND_FILE_PARA  *pFindCond);
NET_SDK_API OS_INT32 SDKCALL NET_DEV_FindNextFile(OS_INT32 lFindHandle, NET_DEV_FILE_INFO* lpFindData);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_FindClose(OS_INT32 lFindHandle);

/*
**remote voice talk
*/
enum G711_LAW
{
    G711_ULAW  =   0,
    G711_ALAW  =   1
};

NET_SDK_API OS_INT32 SDKCALL NET_DEV_StartVoiceCom(OS_INT32 lUserID, OS_UINT32 dwChannel, OS_BOOL bNeedCBNoEncData, VOICE_DATA_CBF fVoiceDataCallBack, OS_VOIDPTR pUser);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_SetVoiceComClientVolume(OS_INT32 lVoiceComHandle, OS_UINT16 wVolume);

NET_SDK_API OS_INT32 SDKCALL NET_DEV_StartVoiceCom_MR(OS_INT32 lUserID, OS_UINT32 dwVoiceChan, VOICE_DATA_CBF fVoiceDataCallBack, OS_VOIDPTR pUser);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_VoiceComSendData(OS_INT32 lVoiceComHandle, char *pSendBuf, OS_UINT32 dwBufSize);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_SendWaveFile(OS_INT32 lVoiceComHandle, const char *FileName);
NET_SDK_API OS_BOOL  SDKCALL NET_DEV_StopVoiceCom(OS_INT32 lVoiceComHandle);

NET_SDK_API OS_BOOL SDKCALL NET_DEV_EncodeG711Frame(OS_UINT32 iType, OS_UINT8 *pInBuffer, OS_UINT8 *pOutBuffer);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_DecodeG711Frame(OS_UINT32 iType, OS_UINT8 *pInBuffer, OS_UINT8 *pOutBuffer);

/*
ptz control command
*/
//direction
#define PTZ_GOTO_HOME						29 		//移到机械云台起始位置
#define PTZ_UP								25 		//机械云台上移
#define PTZ_DOWN							26 		//机械云台下移
#define PTZ_LEFT							27 		//机械云台左转
#define PTZ_RIGHT							28 		//机械云台右转
#define PTZ_RIGHTUP						    56      //
#define PTZ_LEFTUP						    57      //
#define PTZ_RIGHTDOWN						58      //
#define PTZ_LEFTDOWN						59      //

//cruise
#define PTZ_CRUISE_START				    60      //云台巡航控制  
#define PTZ_CRUISE_STOP						61      //云台巡航控制 
#define PTZ_SET_CRUISE_ALLDATA              62      //设置所有预设巡航路径
#define PTZ_SET_PRESET_CRUISE               63      //设置预设巡航点
#define PTZ_DEL_PRESET_CRUISE               64      //删除预设巡航点
#define PTZ_CLEAR_CRUISE                    65      //清除单个巡航路径
#define PTZ_CLEAR_ALL_CRUISE                66      //清除所有巡航路径
#define PTZ_FILL_PRE_SEQ	                67      //将预置点加入巡航序列
#define PTZ_SET_SEQ_DWELL                   68      //设置巡航点停顿时间
#define PTZ_SET_SEQ_SPEED                   69      //设置巡航速度
#define PTZ_CLE_PRE_SEQ                     70      //将预置点从巡航序列中删除
#define PTZ_MAX_PRESET_CRUISE_POINTS	    32		//最多支持的巡航点个数

//focus
#define PTZ_FOCUS_NEAR						21 		//缩小机械云台焦距
#define PTZ_FOCUS_FAR						22 		//放大机械云台焦距
#define PTZ_ZOOM_WIDE						23 		//放大机械云台视角   
#define PTZ_ZOOM_TELE						24 		//缩小机械云台视角
#define PTZ_IRIS_AUTO						62 		//自动光圈
#define PTZ_FOCUS_AUTO						63 		//自动聚焦

//move
#define PTZ_GOTO_REL_POSITION				50  	//从相对当前坐标位置运动指定的偏移量
#define PTZ_GOTO_ABS_POSITION				51  	//运动到指定的绝对坐标位置
#define PTZ_GET_ABS_POSITION				52  	//获取云台当前的绝对坐标
#define PTZ_IRIS_OPEN						53 		//光圈
#define PTZ_IRIS_CLOSE						54 		//光圈
#define PTZ_3DPOSBOX                        55 		//区域缩放

//pan
#define PTZ_AUTOPAN							49  	//自动水平旋转
#define PTZ_AUXIOPEN						47  	//辅助开  支持辅助点 1~8
#define PTZ_AUXICLOSE						48  	//辅助关
#define PTZ_AUTOPAN							49  	//自动水平旋转

//pattern
#define PTZ_START_PATTERN                   80      //启动运行模式路径
#define PTZ_STOP_PATTERN                    81      //停止运行模式路径
#define PTZ_START_RECORD_PATTERN            82      //启动记录模式路径
#define PTZ_STOP_RECORD_PATTERN             83      //停止记录模式路径
#define PTZ_DEL_PATTERN                     84      //删除模式路径

NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZControl(OS_INT32 lUserID, OS_INT32 nChannel, NET_DEV_PTZ_PARA* pControlPara);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZ3DControl(OS_INT32 lUserID, OS_INT32 nChannel, NET_DEV_PTZ_3DCTRL* pControlPara);

NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZSetPreset(OS_INT32 lUserID, OS_INT32 nChannel, OS_INT32 nPresetIndex, char* sPresetName);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZGotoPreset(OS_INT32 lUserID, OS_INT32 nChannel, OS_INT32 nPresetIndex, OS_INT32 iHSpeed , OS_INT32 iVSpeed);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZClearPreset(OS_INT32 lUserID, OS_INT32 nChannel, OS_INT32 nPresetIndex);

NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZGetCruise(OS_INT32 lUserID,OS_INT32 nChannel, OS_INT32 nCruiseID, NET_DEV_PTZ_CRUISE_PARAM* pCruisePara, OS_INT32* pCount);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZSetCruiseControl(OS_INT32 lUserID,OS_INT32 nChannel, OS_INT32 nCruiseID, OS_INT32 nCtrl);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZSetCruise(OS_INT32 lUserID, OS_INT32 nChannel, OS_INT32 nSetCruiseCmd, OS_INT32 nCruiseID, OS_VOID *pConfig);

NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZ3DCameraLinkage(OS_INT32 lUserID, OS_INT32 nChannel, NET_DEV_PTZ_3DCTRL* pControlPara);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_ReadCOM(OS_INT32 lUserID, const NET_DEV_READ_COM_DATA_INFO *Request, NET_DEV_COM_DATA* pData);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_WriteCOM(OS_INT32 lUserID, const NET_DEV_COM_DATA* pData);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_GetCOMInitiativeUploadConfig(OS_INT32 lUserID, NET_DEV_COM_INITIATIVE_UPLOAD_CONFIG* pConfig);
NET_SDK_API OS_BOOL SDKCALL NET_DEV_SetCOMInitiativeUploadConfig(OS_INT32 lUserID, const NET_DEV_COM_INITIATIVE_UPLOAD_CONFIG* pConfig);

/*iPTZCommand：PTZ_GET_ABS_POSITION-获取PTZ位置， PTZ_GOTO_ABS_POSITION-设置PTZ位置*/
NET_SDK_API OS_BOOL SDKCALL NET_DEV_PTZPositionControl(OS_INT32 lUserID, OS_INT32 nChannel, NET_DEV_PTZ_POS_PARA* pPosition );

#endif