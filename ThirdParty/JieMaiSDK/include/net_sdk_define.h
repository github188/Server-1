#ifndef  __NET_SDK_DEFINE_H__
#define  __NET_SDK_DEFINE_H__

#if (defined(_WIN32) ) || (defined(_WIN64))
	#include <windows.h>
#elif (defined(__linux__) )
	#include <unistd.h>	
	#include <sys/types.h>
#endif

#if (defined(_WIN32) ) || (defined(_WIN64))
   #ifndef OS_WINDOWS
   #define OS_WINDOWS  1
   #endif 
#elif (defined(__linux__))
   #define OS_POSIX    1
#endif

#ifdef  NET_SDK_PROJ
	#if defined OS_WINDOWS
	#define NET_SDK_API  extern "C"__declspec(dllexport)
	#elif 
	#define NET_SDK_API  extern "C"
	#endif
#else
	#if (defined(_WIN32))
	#define NET_SDK_API  extern "C" __declspec(dllimport)
	#elif defined(__linux__) //linux
	#define NET_SDK_API  extern "C"
	#else
	#error OS unsupport!
	#endif
#endif  //end NET_SDK_PROJ

#ifndef  NET_SDK_PROJ
#define OS_TRUE  1
#define OS_FALSE 0

#ifndef OS_VOID
	#if defined WIN32 || defined _WIN64
		typedef unsigned __int64 OS_UINT64;
		typedef DWORD     OS_UINT32;
		typedef WORD      OS_UINT16;
		typedef int       OS_INT32;
        typedef short     OS_INT16;
		typedef void      OS_VOID;
		typedef BOOL      OS_BOOL;
		typedef unsigned char OS_UINT8;
		#define OS_VOIDPTR   void*
	#elif defined __linux__
		typedef unsigned long long OS_UINT64;
		typedef unsigned int     OS_UINT32;
		typedef unsigned short   OS_UINT16;
		typedef int              OS_INT32;
        typedef short            OS_INT16;
		typedef bool             OS_BOOL;
		typedef unsigned char    OS_UINT8;
		#define OS_VOID          void
		#define OS_VOIDPTR       void*
	#else
	#error OSnot support 
	#endif
#endif  //end OS_VOID
#endif  //NET_SDK_PROJ

#ifdef OS_WINDOWS
#define SDKCALL                         _stdcall                       //default stack call
#else
#define SDKCALL
#endif

#ifndef __HWND_defined
#define __HWND_defined
#if defined(__linux__)
typedef unsigned int HWND;
#endif
#endif

#ifndef __HDC_defined
#define __HDC_defined
#if defined(__linux__)
typedef struct __DC
{
	void*   surface;        //SDL Surface
	HWND    hWnd;           // HDC window handle
}DC;
typedef DC* HDC;
#endif
#endif

typedef void*  ipcHandle;

#ifdef NET_SDK_PROJ
#include "OS_Netdb.h"
#endif

static void T_Hton(OS_INT16 &data)
{
#ifdef NET_SDK_PROJ
	data = OS_Htons(data);
#endif
}

static void T_Hton(OS_UINT16 &data)
{
#ifdef NET_SDK_PROJ
	data = OS_Htons(data);
#endif
}

static void T_Hton(OS_UINT32 &data)
{
#ifdef  NET_SDK_PROJ
	data = OS_Htonl(data);
#endif
}

static void T_Hton(OS_INT32 &data)
{
#ifdef  NET_SDK_PROJ
	data = OS_Htonl(data);
#endif
}

static void T_Hton(OS_UINT64 &data)
{
#ifdef  NET_SDK_PROJ
	data = data;
#endif
}

/*
*basic macro define 
*/
#define NET_GROUP_CHAN_NUM                     32                      //max channel number in one group
#define NET_NAME_LEN                           32                      //name length
#define NET_MAC_ADDR_LEN                       20                      //MAC address string len 
#define NET_IPV4_LEN                           16                      //IPV4 address length
#define NET_IPV6_LEN                           128                     //IPV6 address length
#define NET_MAX_ALARM_CENTER                   8                       //max alarm center number
#define NET_MAX_SCHEDULE_DAY                   8                       //week(include 7 days) + holiday
#define NET_MAX_STREAM                         8                       //max stream in one channel
#define NET_MAX_CHAN                           512                     //max channel number 
#define NET_MAX_PATH_LEN                       256                     //max file path length
#define NET_MAX_MOTION_AREA                    16                      //max support motion area number
#define NET_MAX_SCHEDULE_SEGMENT               8                       //max support schedule segment number
#define NET_MAX_HIDE_AREA                      8                       //max hide area number device support  
#define MAX_SMART_DETECT_NUM                   13  
#define MAX_SMART_DETECT_SUPPORT_NUM           3  
#define MAX_SMART_AREA                         4
#define MAX_SMART_AREA_POINT                   4

/*
*matrix macro define 
*/
#define MATRIX_MAX_WINDOW_NUM   	           32                      //matrix define
#define MATRIX_MAX_NAME_LEN     	           64                      //matrix define
#define MATRIX_MAX_IP_LEN       	           32                      //matrix define
#define MATRIX_MAX_URI_LEN      	           256                     //matrix define
#define MATRIX_MAX_MONITOR_NUM  	           4                       //matrix define
#define MATRIX_MAX_SUBGROUP_NUM 	           128                     //matrix define

/*
**config command
*/
#define NET_DEV_GET_DEVICE_INFO                   0x0001               //device info,struct:NET_DEV_DEVICE_INFO
#define NET_DEV_SET_DEVICE_INFO                   0x0002        

#define NET_DEV_GET_IP_CHAN_GROUP_INFO            0x0003               //ip channel config(by channel group id),struct:NET_DEV_IP_CHAN_GROUP_INFO
#define NET_DEV_SET_IP_CHAN_INFO                  0x0004        

/*
**nvr config
*/
#define NET_DEV_GET_IP_CHAN_GROUP_STATUS          0x0005               //channel status(by channel group id),struct:NET_DEV_IP_CHAN_GROUP_STATUS

/*
**nvr config(delay)
*/
#define NET_DEV_GET_NET_ADAPTER                   0x0007               //net adapter info(by net card id),struct:NET_DEV_NET_ADAPTER
#define NET_DEV_SET_NET_ADAPTER                   0x0008        

#define NET_DEV_GET_SYNC_TIME                     0x0009               //time adjust config,struct:NET_DEV_SYNC_TIME
#define NET_DEV_SET_SYNC_TIME                     0x0010        

#define NET_DEV_GET_RECORD_SCHEDULE               0x0011               //file record schedule,struct:NET_DEV_RECORD_SCHEDULE
#define NET_DEV_SET_RECORD_SCHEDULE               0x0012        

/*
**ipc config
*/             
#define NET_DEV_GET_NET_PORT                      0x0013               //net port config,struct:NET_DEV_NET_PORT
#define NET_DEV_SET_NET_PORT                      0x0014    

#define NET_DEV_GET_DAY_NIGHT                     0x0015               //day-night config,struct:NET_DEV_DAY_NIGHT
#define NET_DEV_SET_DAY_NIGHT                     0x0016      

#define NET_DEV_GET_ADVANCE_IMAGING               0x0017               //advance imaging config,struct:NET_DEV_AVANCE_IMAGING
#define NET_DEV_SET_ADVANCE_IMAGING               0x0018       

#define NET_DEV_GET_SHOW_CFG                      0x0019               //ipc channnel osd show config, struct:NET_DEV_SHOW_CFG
#define NET_DEV_SET_SHOW_CFG                      0x001a        

#define NET_DEV_GET_AUDIO_TALK                    0x0020               //audio talk parameter,struct:NET_DEV_AUDIO_TALK_PARA
#define NET_DEV_SET_AUDIO_TALK                    0x0021      

#define NET_DEV_GET_AUTO_FOCUS                    0x0022               //auto focus,struct:NET_DEV_FOCUS
#define NET_DEV_SET_AUTO_FOCUS                    0x0023        

#define NET_DEV_GET_ALARM_CENTER                  0x0024               //alarm center,struct:NET_DEV_ALARM_CENTER
#define NET_DEV_SET_ALARM_CENTER                  0x0025      

#define NET_DEV_GET_PTZ_DECODE                    0x0026               //ptz decode config,struct:NET_DEV_PTZ_DECODER
#define NET_DEV_SET_PTZ_DECODE                    0x0027      

#define NET_DEV_GET_ALARMIN_CFG                   0x0028               //alarm in config(by alarm in id),struct:NET_DEV_ALARMIN_CFG
#define NET_DEV_SET_ALARMIN_CFG                   0x0029       

#define NET_DEV_GET_MOTION_DETECT                 0x002a               //motion detection config(by channel id), struct:NET_DEV_MOTION_DETECT
#define NET_DEV_SET_MOTION_DETECT                 0x002b

#define NET_DEV_GET_PIR_DETECT                    0x002c               //passive infrared detection config(by channel id), struct:NET_DEV_PIR_DETECT
#define NET_DEV_SET_PIR_DETECT                    0x002d       

#define NET_DEV_GET_VIDEO_LOST_DETECT             0x002e               //video lost detection config(by channel id), struct:NET_DEV_VIDEO_LOST_DETECT
#define NET_DEV_SET_VIDEO_LOST_DETECT             0x0030

#define NET_DEV_GET_AUDIO_DETECT                  0x0031               //audio detection config(by channel id), struct:NET_DEV_AUDIO_DETECT
#define NET_DEV_SET_AUDIO_DETECT                  0x0032

#define NET_DEV_GET_ALARMIN_SCHEDULE              0x0035               //alarmin schedule config(by channel id), struct:NET_DEV_ALARM_SCHEDULE
#define NET_DEV_SET_ALARMIN_SCHEDULE              0x0035

#define NET_DEV_GET_ALARMOUT_SCHEDULE             0x0037               //alarmout schedule config(by channel id), struct:NET_DEV_ALARM_SCHEDULE
#define NET_DEV_SET_ALARMOUT_SCHEDULE             0x0038

#define NET_DEV_GET_PIR_DETECT_SCHEDULE           0x0039               //PIR schedule config(by channel id), struct:NET_DEV_ALARM_SCHEDULE
#define NET_DEV_SET_PIR_DETECT_SCHEDULE           0x003a

#define NET_DEV_GET_ABNORMAL_DETECT_SCHEDULE      0x003b               //video exception schedule config(by channel id), struct:NET_DEV_ALARM_SCHEDULE
#define NET_DEV_SET_ABNORMAL_DETECT_SCHEDULE      0x003c

#define NET_DEV_GET_MOTION_DETECT_SCHEDULE        0x003d               //motion detection exception schedule config(by channel id), struct:NET_DEV_ALARM_SCHEDULE
#define NET_DEV_SET_MOTION_DETECT_SCHEDULE        0x003e

#define NET_DEV_GET_ALARMOUT_CFG                  0x0040               //alarmout config(by channel id), struct:NET_DEV_ALARM_SCHEDULE
#define NET_DEV_SET_ALARMOUT_CFG                  0x0041     

#define NET_DEV_GET_HIDE_CFG                      0x0042               //hide area config(by channel id), struct:NET_DEV_HIDE_CFG
#define NET_DEV_SET_HIDE_CFG                      0x0043

#define NET_DEV_GET_AUDIO_DETECT_SCHEDULE         0x0044               //audio detection exception schedule config(by channel id), struct:NET_DEV_ALARM_SCHEDULE
#define NET_DEV_SET_AUDIO_DETECT_SCHEDULE         0x0045

#define NET_DEV_GET_ALARM_LINE_DECTECT            0x0046               //alarm smart line config(by channel id), struct:NET_DEV_SMART_DETECT
#define NET_DEV_SET_ALARM_LINE_DECTECT            0x0047               

#define NET_DEV_GET_ALARM_FIELD_DECTECT           0x0048               //alarm smart field config(by channel id), struct:NET_DEV_SMART_DETECT
#define NET_DEV_SET_ALARM_FIELD_DECTECT           0x0049               

#define NET_DEV_GET_ALARM_OBJECT_LEFT             0x004a               //alarm smart objecet left config(by channel id), struct:NET_DEV_SMART_DETECT
#define NET_DEV_SET_ALARM_OBJECT_LEFT             0x004b              

#define NET_DEV_GET_ALARM_TEMP                    0x004c               //alarm smart env config(by channel id), struct:NET_DEV_ALARM_
#define NET_DEV_SET_ALARM_TEMP                    0x004d

#define NET_DEV_GET_ALARM_HUMIDITY                0x004e               //alarm env config(by channel id), struct:NET_DEV_ALARM_ENV
#define NET_DEV_SET_ALARM_HUMIDITY                0x0050

#define NET_DEV_GET_VEHICLE_DETECT                0x0051               
#define NET_DEV_SET_VEHICLE_DETECT                0x0052

#define NET_DEV_GET_FLOAT_DETECT                  0x0053               
#define NET_DEV_SET_FLOAT_DETECT                  0x0054

#define NET_DEV_GET_VEHICLE_SERVER                0x0055
#define NET_DEV_SET_VEHICLE_SERVER                0x0056

#define NET_DEV_SET_PLATE_CMD_INFO                0x0057

#define NET_DEV_GET_LIFTARM                       0x0061                           
#define NET_DEV_SET_LIFTARM                       0x0062

//camera linkage
#define NET_DEV_GET_CAMERALINKAGE_CFG             0x0076
#define NET_DEV_SET_CAMERALINKAGE_CFG             0x0077

//electric power project
#define NET_DEV_GET_ELECTRIC_POWER_ALARM_CFG            0x0078
#define NET_DEV_SET_ELECTRIC_POWER_ALARM_CFG            0x0079
#define NET_DEV_GET_ELECTRIC_POWER_BATTERY_BASE_INFO    0x007a
#define NET_DEV_SET_ELECTRIC_POWER_BATTERY_BASE_INFO    0x007b
#define NET_DEV_GET_ELECTRIC_POWER_BATTERY_DEFAULT_CFG  0x007c
#define NET_DEV_SET_ELECTRIC_POWER_BATTERY_DEFAULT_CFG  0x007d
#define NET_DEV_GET_ELECTRIC_POWER_BATTERY_CFG          0x007e
#define NET_DEV_SET_ELECTRIC_POWER_BATTERY_CFG          0x007f

#define NET_DEV_GET_3DCTRL_CAPABILITY             0x0080

#define NET_DEV_GET_VEHICLE_FLOW_DETECT          0x0081
#define NET_DEV_SET_VEHICLE_FLOW_DETECT          0x0082
enum UPLOAD_TYPE
{
    UPLOAD_IPC_ALARM				= 0x2000,   //IPC alarm type, struct:NET_DEV_IPC_ALARM
    UPLOAD_PLATE					= 0x2001,   //upload plate info,struct:NET_DEV_PLATE_CAPTURE
	UPLOAD_ELECTRICAL_LINE			= 0x2002,   // upload electrical line, struct: NET_DEV_ALARM_HEADER_EX
    UPLOAD_TRAFFIC_FLOW  			= 0x2003,   // upload traffic flow detect, struct: NET_DEV_ALARM_HEADER_EX
	UPLOAD_NVR_DISK_ALARM			= 0x2010,
};

enum _PLATE_COLOR_
{ 
	COLOR_BLACK = 1,                                     //黑牌 
	COLOR_YELLOW,                                        //黄牌 
	COLOR_BLUE,                                          //蓝牌 
	COLOR_WHITE,                                         //白牌 
	COLOR_GREEN,                                         //绿牌 
	COLOR_OTHER = 9,                                     //其他 
};

enum IPC_ALARM_TYPE
{
    IPC_ALARM_INPUTPORT			=	(0x500f),		                   //外部输入报警
    IPC_ALARM_CAMERATAMPERING	=	(0x5010),		                   //视频异常报警
    IPC_ALARM_MONTIONDETECTION	=	(0x5011),                          //移动侦测报警
    IPC_ALARM_VIDEO_LOST		=	(0x5026),
    IPC_ALARM_VIDEO_BLIND		=	(0x5027),
    IPC_ALARM_OUTPUT			=	(0x5030),		                   //报警输出 
    IPC_ALARM_PRI				=	(0x5036),	                       //PRI报警
    IPC_ALARM_VIOLATE_BORDER	=	(0x503c),		                   //越界报警
    IPC_ALARM_AREA_INTRUDE		=	(0x503d),	                       //区域入侵报警
    IPC_ALARM_PROPERTY_LEFT		=	(0x503e),                          //物体遗留报警
    IPC_ALARM_AUDIO_DETECT      =   (0x503f),                          //声音报警
	IPC_ALARM_TEMP              =   (0x5040),                          //温度报警 
	IPC_ALARM_HUMIDITY          =   (0x5041),                          //湿度报警 
	IPC_ALARM_TRANS_COM         =   (0x5042)                           //串口透传
};

enum LINK_STRATEGY
{
    STRATEGY_ALARMOUT      = 0x1,                                      //trigger alarm out
    STRATEGY_ALARMCENTER   = 0x2,                                      //upload to alarm center
    STRATEGY_AUDIO         = 0x4,                                      //output audio
    STRATEGY_EMAIL         = 0x8,                                      //send email
    STRATEGY_FTP           = 0x10,                                     //upload to ftp server
    STRATEGY_RECORD        = 0x20,                                     //trigger record video 
    STRATEGY_PTZ           = 0x40,                                     //trigger ptz 
    STRATEGY_CAPTURE       = 0x80                                      //trigger capture picture
};

typedef struct  
{
    OS_UINT8 byLogLevel;                                               //1:ERROR_LEVEL 2:DEBUG_LEVEL 3:INFO_LEVEL
    OS_UINT8 byEnableConsole;                                          //1 output to console 0:not
    OS_UINT8 byEnableDebug;                                            //1:output to debug 0:not
    OS_UINT8 byEnableFile;                                             //1:output to file 0:not
    char     sFilePath[256];                                           //log file path
    OS_UINT8 byRes[100];                                               //reserved
}NET_DEV_SDK_LOG;

typedef union __NET_PLAY_HWND
{
#ifdef OS_WINDOWS
	HWND hWnd;	                                                       //play handle on windows OS
#else
    OS_INT32 hWnd;
#endif
}NET_PLAY_HWND, *PNET_PLAY_HWND;

typedef struct 
{
	OS_UINT16 wYear;                                                   //year, eg.2015
	OS_UINT8  byMonth;                                                 //month, 1~12
	OS_UINT8  byDay;                                                   //day, 1~31
	OS_UINT8  byHour;                                                  //hour,1~12
	OS_UINT8  byMinute;                                                //minute,0~59
	OS_UINT8  bySecond;                                                //second, 0~29
	OS_UINT8  byRes;                                               
}NET_DEV_TIME, *LPNET_DEV_TIME;

typedef struct
{
	OS_UINT32   dwDeviceId;                                            //device id ,could be changed by user    
	char	 sDeviceName[128];                                         //device name,could be changed by user        
	char	 sDeviceType[32];                                          //device type, read only      
	char	 sDeviceSN[48];                                            //serial number, read only   
	char	 sManufacturer[64];                                        //manufacturer, read only    
	char	 sHardwareVersion[64];                                     //hardware version, read only          
	char	 sSoftwareVersion[64];                                     //software version, read only          
    OS_UINT16 wIPChanNum;                                              //ip channel number 
    OS_UINT8 byNetAdapterNum;		                                   //net adapter number            
    OS_UINT8 byDiskNum;			                                       //hard disk number
	OS_UINT8 byAlarmInPortNum;		                                   //alarm in number                                                           
	OS_UINT8 byAlarmOutPortNum;		                                   //alarm out number            					
	OS_UINT8 byRes[202];                                               //reserved     
}NET_DEV_DEVICE_INFO;

typedef struct
{  
	char    sIpV4[16];                                                 //ipv4 address string
	char    sIpV6[128];                                                //ipv6 address string
}NET_DEV_IP_ADDR;

typedef struct  
{
    NET_DEV_IP_ADDR strIpAddr;                                         //ip address
    OS_UINT16   wPort;                                                 //ONVIF server port of source device
    OS_UINT8    byMainTransPortocol;                                   //trans protocol of main stream,0:TCP, 1:UDP  
    OS_UINT8    bySubTransPortocol;                                    //trans protocol of sub stream,0:TCP, 1:UDP    
}NET_DEV_ONVIF_SOURCE;

typedef struct  
{
	OS_UINT32 dwChannelID;                                             //channel ID
	OS_UINT32 dwStreamID;                                              //stream ID
}NET_DEV_GB_SOURCE;

typedef struct  
{
	OS_UINT8    strMainUrl[260];                                       //URL of main stream 
	OS_UINT8    strSubUrl[260];                                        //URL of sub stream
	OS_UINT8    byMainTransPortocol;                                   //protocol of main stream,0:TCP, 1:UDP
	OS_UINT8    bySubTransPortocol;                                    //protocol of sun stream,0:TCP, 1:UDP
}NET_DEV_RTSP_SOURCE;

typedef struct 
{
	OS_UINT16     wChanID;                                             //channel ID, start from 1
	OS_UINT8	  byEnable;                                            //chanel is enabled or not,0:enabled, 1:disabled
	OS_UINT8	  byProType;                                           //access protocol type,1:ONVIF, 2:RTSP, 3:GB28181

    char  sUserName[NET_NAME_LEN];                                     //user name of ip device
    char  sPassword[NET_NAME_LEN];                                     //password of ip device

	union
	{
		NET_DEV_ONVIF_SOURCE struOnvifSource;                          //ONVIF source parameter
		NET_DEV_RTSP_SOURCE  struRtspSource;                           //RTSP source parameter
		NET_DEV_GB_SOURCE    struGbSource;                             //gb28181 source parameter
		OS_UINT8 byRes[600];                                           //reserved
	}uSource;                                                          //stream source parameter
}NET_DEV_IP_CHAN_INFO;

typedef struct
{
	OS_UINT8   byChanNum;                                              //valid channel number
	OS_UINT8   byRes1[203];                                            //reserved
	NET_DEV_IP_CHAN_INFO  struChanInfo[NET_GROUP_CHAN_NUM];            //channel info 
}NET_DEV_IP_CHAN_GROUP_INFO;

typedef struct
{ 
	OS_UINT16            iChanID;                                      //channel ID
	OS_UINT8             byEnabled;                                    //channel is enabled or not,0:enabled, 1:disabled
	OS_UINT8             byRecordStatic;                               //0:not recording, 1:recording 
	OS_UINT8             bySignalStatic;                               //signal state：0:normal,1:signal lost,2:get stream failed,3:authentication failed
	OS_UINT8             byClientNum;                                  //link client number 
	OS_UINT8             byRes1[2];                                    //reserved 
	OS_UINT32            dwBitRate;                                    //bit rate
	OS_UINT8             byRes2[20];                                   //reserved
}NET_DEV_IP_CHAN_STATUS;

typedef struct
{
	OS_UINT8	     byChanNum;                                        //valid channel number
	OS_UINT8         byRes1[203];                                      //reserved 
	NET_DEV_IP_CHAN_STATUS  struChanInfo[NET_GROUP_CHAN_NUM];          //channel status
}NET_DEV_IP_CHAN_GROUP_STATUS;

typedef struct
{  
	char                  sName[NET_NAME_LEN];                         //net adapter name
	NET_DEV_IP_ADDR       struIP;                                      //ip address 
	NET_DEV_IP_ADDR       struIPMask;                                  //subnet mask
	OS_UINT32             dwAdapterType;                               //type:1-10MBase-T;2-10MBase-TFull Duplex;3-100MBase-TX;4-100MFull Duplex;5-10M/100M/1000Mself-adaption     
	OS_UINT8              byUseDhcp;                                   //0:not use DHCP,1:use DHCP
	OS_UINT16             wMTU;                                        //MTU
	char                  sMACAddr[NET_MAC_ADDR_LEN];                  //mac address
	OS_UINT8              byRes[102];                                  //reserved
	NET_DEV_IP_ADDR       struGatewayIpAddr;                           //gate way address
	NET_DEV_IP_ADDR       struDnsServer1IpAddr;                        //main DNS server
	NET_DEV_IP_ADDR       struDnsServer2IpAddr;                        //backup DNS server
}NET_DEV_NET_ADAPTER, *LPNET_DEV_NET_ADAPTER;

typedef struct
{
	OS_UINT16   wSearchCommand;                                        //see enum SEARCH_CMD
	OS_UINT16   wFileType;                                             //file type, 0xffff:all files
	OS_UINT32   dwChannel;                                             //channel ID, start from 1
	NET_DEV_TIME struStartTime;                                        //search start time  
	NET_DEV_TIME struStopTime;                                         //search end time
	OS_UINT8    byRes[636];                                            //reserved
}NET_DEV_FIND_FILE_PARA;

typedef struct
{
	NET_DEV_TIME       struStartTime;                                  //file start time  
	NET_DEV_TIME       struStopTime;                                   //file end time
	OS_UINT32          dwFileSize;                                     //file size, unit:Byte
	OS_UINT16          wFileType;                                      //file type
	OS_UINT8           byRes1[338];                                    //reserved
}NET_DEV_FILE_INFO;

typedef struct
{
    NET_DEV_IP_ADDR strIpAddr;                                         //ip address of alarm device
    OS_UINT8 byRes[200];                                               //reserved
}NET_DEV_ALARMER;

typedef struct  
{
    OS_UINT8 byRes[200];                                               //reserved
}ALARM_CHAN_ARA;

typedef struct 
{
    OS_UINT32     dwIdHigh;										       //eg. alarm message id = dwIdHigh<<32 + dwIdLow
    OS_UINT32     dwIdLow;                  				           //see dwIdHigh
    OS_UINT32	  dwLevel;                 						       //alarm level, 0>1>2>...
    OS_UINT32	  dwState; 										       //1:alarm begin 0:alarm end
    char	      sStamp[32];									       //alarm time stamp,format:YYYYMMDDHHMMSS,eg.:20120802235502
    char	      sDescription[64];								       //alarm description
    char	      sReserved[64];		    					       //reserved
}NET_DEV_ALARMINFO;


typedef struct
{  
    OS_UINT8    byStartHour;                                           //hour value of start time                                     
    OS_UINT8    byStartMin;                                            //minute value of start time 
    OS_UINT8    byStartSecond;                                         //second value of start time  
    OS_UINT8    byStopHour;                                            //hour value of stop time   
    OS_UINT8    byStopMin;                                             //minute value of stop time   
    OS_UINT8    byStopSecond;                                          //second value of stop time   
    OS_UINT8    byRes[2];                                              //reserved
}NET_DEV_SCHEDULE_TIME;                                   

typedef struct
{
    OS_UINT8    byStartHour;                                           //hour value of start time    
    OS_UINT8    byStartMin;                                            //minute value of start time 
    OS_UINT8    byStopHour;                                            //hour value of stop time   
    OS_UINT8    byStopMin;                                             //minute value of stop time   
}NET_DEV_SCHEDLUE;                                   

typedef struct
{
    OS_UINT8  byAllDay;                                                //0:record by time segment,1:all day record
    OS_UINT8  byRes[15];                                               //reserved
    union
    {
        struct
        {
            NET_DEV_SCHEDULE_TIME struTimeSeg[8];                      //record time segment             
            OS_UINT8  byRes[16];                                       //reserved 
        }struTimeSegment;                                              //record segment parameter
        struct
        {
            OS_UINT8 byRes[48];                                        //reserved 
        }struAllDay;                                                   //record all day parameter
    }uPara;                                                            //record time parameter
}NET_DEV_RECORD_DAY;                                   

typedef struct
{
    OS_UINT8  byEnableRecord;                                          //1:enable record 0:not record
    OS_UINT8  byReserved1[3];                                          //reserved 
    NET_DEV_RECORD_DAY  strRecordDays[NET_MAX_SCHEDULE_DAY];           //0:Sunday,1-Monday,...6:Saturday,7:reserved
    OS_UINT8  byStreamID;                                              //1:stream1, 2:stream2, 3:stream3, 4:stream4.. 
    OS_UINT8  byReserved2[99];                                         //reserved        
}NET_DEV_RECORD_SCHEDULE;                                   

typedef struct
{
    NET_DEV_IP_ADDR struIpAddr;                                        //NTP server ip address
    OS_UINT16   wInterval;                                             //adjust time interval,unit:minute,value range:1~100000
    OS_UINT8    byRes[102];                                            //reserved
}NET_DEV_NTP;                                   

typedef struct
{
    OS_UINT32	 dwSyncMode;                                           //0:adjust time with NTP server 1:manual
    NET_DEV_TIME struTime;                                             //time value when manual adjust time
    char         sTimeZone[32];                                   	   //Time Zone，format:CST+HH:MM:SS  or CST-HH:MM:SS，eg.GMT+8 = "CST+08:00:00"， GMT-8 = "CST-08:00:00"
    NET_DEV_NTP  struNtpInfo;                                          //NTP server info
    OS_UINT8     byRes[100];                                           //reserved
}NET_DEV_SYNC_TIME;                                   

typedef enum 
{
    PLAY_MODE_NORMAL      = 1,
    PLAY_MODE_SWITCHGROUP = 2,
}MATRIX_PLAY_MODE;                                   

typedef enum 
{
    CAMERA_TYPE_GMI = 0,
    CAMERA_TYPE_VISKING = 1,
    CAMERA_TYPE_ONVIF = 2,
    CAMERA_TYPE_RTSP = 3
}MATRIX_CAMERA_TYPE;                                   

typedef struct 
{
    OS_INT32 dwX;                                   
    OS_INT32 dwY;                                   
    OS_INT32 dwWidth;                                   
    OS_INT32 dwHeight;                                   
}NET_MATRIX_RECT;                                   

typedef struct 
{
    float fX;                                   
    float fY;                                   
    float fWidth;                                   
    float fHeight;                                   
}NET_MATRIX_RECT_EX;                                   

typedef struct 
{
    OS_UINT32  dwMonitorNum;                                   
    NET_MATRIX_RECT struRect[MATRIX_MAX_MONITOR_NUM];                                   
}NET_MATRIX_MONITOR_LIST;                                   

typedef struct 
{
    OS_UINT32 dwWindowNum;                                   
    NET_MATRIX_RECT_EX struRectEx[MATRIX_MAX_WINDOW_NUM];                                   
}NET_MATRIX_LAYOUT;                                   

typedef struct 
{
    char  sName[MATRIX_MAX_NAME_LEN];                                   
    char  sIP[MATRIX_MAX_IP_LEN];                                   
    OS_UINT32  dwPort;                                   
    OS_UINT32  dwChannel; //0~15 channel id, 16~31 stream id
    char  sUserName[MATRIX_MAX_NAME_LEN];                                   
    char  sPassword[MATRIX_MAX_NAME_LEN];                                   
    OS_UINT32 dwCameraType;                                   
    char  sURI1[MATRIX_MAX_URI_LEN]; 
    char  sURI2[MATRIX_MAX_URI_LEN]; 
    char  sReserved[MATRIX_MAX_URI_LEN];                                   
}NET_MATRIX_CAMERA_INFO;                                   

typedef struct 
{
    OS_UINT32      dwWindow;                                   
    NET_MATRIX_CAMERA_INFO struCameraInfo;                                   
}NET_MATRIX_CAMERA_INFO_EX;                                   

typedef struct 
{
    OS_UINT32       dwCameraNum;                                   
    NET_MATRIX_CAMERA_INFO_EX struCameraInfoEx[MATRIX_MAX_WINDOW_NUM];                                   
}NET_MATRIX_CAMERA_INFO_EX_ARRAY;                                   

typedef struct 
{
    char  sName[MATRIX_MAX_NAME_LEN];                                   
    NET_MATRIX_LAYOUT struLayout;                                   
    OS_UINT32     dwInterval;                                   
    NET_MATRIX_CAMERA_INFO_EX_ARRAY struCameraInfoExArray;                                   
}NET_MATRIX_SUBGROUP;                                   

typedef struct 
{
    char  sSwitchgroupName[MATRIX_MAX_NAME_LEN];                                   
    OS_UINT32 dwSubgroupNum;                                   
    NET_MATRIX_SUBGROUP struSubgroup[MATRIX_MAX_SUBGROUP_NUM];                                   
}NET_MATRIX_SUBGROUP_ARRAY;                                   

typedef struct
{
    HWND hWnd;                                                         //play video windows, set hWnd =  NULL if show is not needed
    NET_DEV_TIME struStartTime;                                        //video start time 
    NET_DEV_TIME struStopTime;                                         //video end time
    OS_INT32  nChannel;                                                //channel ID, start form 1
    OS_UINT8  byRes[200];                                              //reserved
}NET_DEV_PALYBACK_PARA;                                   

typedef struct
{
    OS_INT32 nChannel;                                   		       //channel ID, start form 1
    OS_INT32 nLinkMode;                                   	           //0 RTP OVER RTSP  1 RTP OVER UDP, 2-private gssp stream
    OS_INT32 nStreamID;                                                //1:stream1, 2:stream2, 3:stream3, 4:stream4.. 
    OS_BOOL  bBlocked;                                                 //0:non-block, function will return before connection,1:block,function will return after connection
    HWND hPlayWnd;                                   	               //play video windows, set hWnd =  NULL if show is not needed
    OS_UINT16 wTimeout;                                                //rtsp timeout, unit:mile second 
    OS_UINT8  byRes[398];                                              //reserved
}NET_DEV_REALPLAY_PARA;                                   

#define  CALLBACK_STD_MEDIA_DATA      1                                 //realplay std data call back, struct:NET_DEV_STD_MEDIA_INFO
#define  CALLBACK_STREAM_INTERUPT     2                                 //realplay stream interupted, struct:NULL
#define  CALLBACK_FILE_END            3                                 //playback to end time
#define  CALLBACK_STREAM_EXCEPTION    4                                 //play exception

typedef struct
{
    OS_UINT32         dwMediaType;                                     //media type, see CALLBACK_STD_MEDIA_DATA
    OS_UINT32         dwSerialNumber;                                  //serial number
    OS_UINT8*         pDataBuf;                                        //media data buffer
    OS_UINT32         dwDataSize;                                      //media data size
    OS_UINT32         dwPTS;                                           //PTS from RTP packet
    OS_UINT8          byRes[60];                                       //reserved
}NET_DEV_STD_MEDIA_INFO;                                   
 
typedef struct 
{
    OS_INT32 iPTZCommand;                                              //command: see PTZ_GOTO_HOME..
    OS_INT32 iPTZControl;                                              //0 start  1 stop
    OS_INT32 iHSpeed;                                                  //horizontal speed
    OS_INT32 iVSpeed;                                                  //vertical speed
    OS_UINT8 byRes[120];                                               //reserved
}NET_DEV_PTZ_PARA;                                   

typedef struct 
{                                                                     //start point = (x0, y0), end point = (x1, y1), w:rect width, h:height, W:video width, H:video height
    OS_INT32     iTopX;                                               //(x0 / W) * 8192
    OS_INT32     iTopY;                                               //(y0 / H) * 8192
    OS_INT32     iWidth;                                              //(w / W) * 8192
    OS_INT32     iHeight;                                             //(h / H) * 8192
                                                                      //if iWidth> 0 and iHeight > 0, enlarge, if  iWidth< 0 and iHeight < 0, narrow
    OS_UINT8     byRes[64];                                            //reserved //byRes[0]画框方式:0-框选 1-滚轮缩小 2-滚轮放大 
}NET_DEV_PTZ_3DCTRL;                                   

typedef struct
{
    OS_INT32     nPreIndex;                                            //preset Index, range: 1~255
    OS_INT32     nStaySec;                                             //stay time,unit:second
    OS_INT32     nMoveSpeed;                                           //speed, range:0~100
    OS_UINT8     byRes[100];                                           //reserved
}NET_DEV_PRESET_CRUISE_POINT;                                   

typedef struct
{
    OS_INT32                       nCruiseID;                          //cruise id,range:1~32
    char                           sCruName[64];                       //cruise path name
    OS_INT32                       nCruEnable;                         //0 disabled 1enabled
    OS_UINT32                      nCruCount;                          //preset point number 
    NET_DEV_PRESET_CRUISE_POINT    pCruPoint[32];                      //preset point info
   OS_UINT8     byRes[200];                                            //reserved
}NET_DEV_PTZ_CRUISE_PARAM;                                   

enum SYSTEM_CONTROL
{
    REBOOT_DEVICE  = 1,	 
	CAMERALINKAGE_CALIBRATION = 100,  //camera linkage calibration
};                                   

typedef struct 
{
    SYSTEM_CONTROL eControlCode;                                       //see enum SYSTEM_CONTROL 
    OS_UINT8       byRes[200];                                         //reserved
}NET_DEV_SYSTEM_CONTROL;                                   

typedef struct 
{
    OS_UINT16 wHttpServerPort;                                         //device HTTP server port
    OS_UINT16 wRtspServerPort;                                         //device RTSP server port
    OS_UINT16 wSdkServerPort;                                          //device SDK server port
    OS_UINT16 wUpgradePort;                                            //device upgrade server port
    OS_UINT16 wOnvifPort;                                              //device ONVIF server port
    OS_UINT16 wSnmpPort;                                               //device SNMP server port
	OS_UINT16 wCurrentRtspPort;                                        //rtsp port which is used now
    OS_UINT8  byRes[198];                                              //reserved
}NET_DEV_NET_PORT;                                   

typedef struct 
{
    OS_INT32  iMode;                                   				   //0-day mode,1-night mode, 2-auto ,3- timing mode.default 0
    OS_INT32  iDurationTime;                                   		   //valid range is 3~30s.default 5
    OS_INT32  iNightToDayThreshold;                                    //valid range is 0~100.default 60
    OS_INT32  iDayToNightThreshold;                                    //valid range is 0~100.default 40
    OS_UINT32 dwScheduleEnable[NET_MAX_SCHEDULE_DAY];                  //if struSchedule[i] is enabled
    NET_DEV_SCHEDULE_TIME struSchedule[NET_MAX_SCHEDULE_DAY];          //weekday schedule, 0:Sunday,1:Monday...7:holiday reserved  
    OS_UINT8  sReserved[200];                                   
}NET_DEV_DAY_NIGHT;                                    

typedef struct 
{
    OS_UINT8  byMeteringMode;                                   	   //0-spot metering, 1-center metering, 2-average metering,3-custom metering
    OS_UINT8  byBackLightCompFlag;                                     //0-disable back light compensate, 1-enable back light compensate
    OS_UINT8  bySupportDcIris;                                         //0:not support, 1:support
    OS_UINT8  byDcIrisFlag;                                   		   //0-disable DC IRIS, 1-enable DC IRIS                   
    OS_UINT16 wLocalExposure;                                          //wide dynamic :0-disable, 1-Auto, unit in 64, 128 is 2x, valid range is 0~256
    OS_UINT16 wMctfStrength;                                           //3D DBR:0-disable,unit in 32, 64 is 2x, valid range is 0~512
    OS_UINT16 wAeTargetRatio;                                          //(目标亮度系数)valid range is 25~400,unit in percentage
	OS_UINT16 wDcIrisDuty;                                             //valid range is 100~999,
    OS_UINT8  byRes[2];                                                //reserved    
    OS_INT32  iDcIrisFonx1000;                                         //(自动光圈打开比例) 
    OS_INT32  iDcIrisMask;                                   	       //(自动光圈的工作模式)
    OS_UINT8  byReserved[400];                                         //reserved
}NET_DEV_ADVANCE_IMAGING;                                   

typedef struct 
{
    OS_INT32  iEnable;                                   			   //0-disable, 1-enable
    OS_INT32  iLanguage;                                               //1-English, 2-Chinese, 3-Korean
    OS_INT32  iDisplayX;                                   			   //range:0~100
    OS_INT32  iDisplayY;                                   			   //range:0~100
    OS_INT32  iDateStyle;                                   		   //date display style, 0-year-month-day,1-month-day-year etc.
    OS_INT32  iTimeStyle;                                   		   //time display style, 0-hour:minute:sencond
    OS_INT32  iFontColor;                                   		   //font color type, 0-black, 1-red, 2-blue,3-green,4-yellow,5-magenta,6-cyan,7-white,8-auto(self-adaption according to backgroud,range between white and balck)
    OS_INT32  iFontSize;                                   			   //Font size, range in[8,16,24,32]
    OS_INT32  iFontBlod;                                   		       //0-no-blod, 1-blod
    OS_INT32  iFontRotate;                                   		   //0-no-rotate, 1-rotate
    OS_INT32  iFontItalic;                                   		   //0-no-italic, 1-italic
    OS_INT32  iFontOutline;                                   		   //font outline width	
    OS_UINT8   byRes[60];                                              //reserved
}NET_DEV_SHOW_TIMESTAMP;                                   

typedef struct 
{
    OS_INT32  iEnable;                                                 //0-disable, 1-enable
    OS_INT32  iDisplayX;                                               //(0,0)~(100,100) 
    OS_INT32  iDisplayY;                                               //(0,0)~(100,100)
    OS_INT32  iFontColor;                                              //font color type, 0-black, 1-red, 2-blue,3-green,4-yellow,5-magenta,6-cyan,7-white,8-auto(self-adaption according to backgroud,range between white and balck)
    OS_INT32  iFontSize;                                               //Font size, range in [8, 16, 24, 32]
    OS_INT32  iFontBlod;                                               //0-no-bold, 1-bold
    OS_INT32  iFontRotate;                                             //0-no-rotate, 1-rotate
    OS_INT32  iFontItalic;                                             //0-no-italic, 1-italic
    OS_INT32  iFontOutline;                                            //font outline width
    char      sText[128];                                              //null means no OSD	
    OS_UINT8   byRes[60];                                              //reserved
}NET_DEV_SHOW_TEXT;     

typedef struct 
{
    OS_UINT32  dwStreamID;                                             //stream id, range 1~4
    NET_DEV_SHOW_TIMESTAMP struTimeInfo;                               //show time stamp parameter    
    NET_DEV_SHOW_TEXT      struChanInfo;                               //show channel parameter      
    OS_UINT8  byRes[100];                                              //reserved
}NET_DEV_STREAM_SHOW;                                   

typedef struct  
{
    OS_UINT32 dwStreamNum;                                             //support stream number, read only                   
    NET_DEV_STREAM_SHOW struStreamShow[NET_MAX_STREAM];                //show info                    
    OS_UINT8 byRes[200];                                               //reserved
}NET_DEV_SHOW_CFG;                                   

typedef enum
{
    FOCUS_MODE_MANUAL = 0,                                             //Manual mode
    FOCUS_MODE_AUTO,                                                   //Auto mode
    FOCUS_MODE_ONCE_AUTO                                               //once auto mode
}FOCUS_MODE;                                   

typedef struct
{
    OS_INT32 iFocusMode;                                   		       //see FOCUS_MODE
    OS_INT32 iFocusDisLimit;                                           //1000mm,1500mm,3000mm,5000mm,6000m
    OS_UINT8 byReserved[200];                                          //reserved
}NET_DEV_FOCUS;                                     

typedef struct 
{ 
    OS_INT32  iExposureMode;                                           //0-EC_ANTI_FLICKER_50HZ, 1-EC_ANTI_FLICKER_60HZ 2 auto
    OS_INT32  iExposureValueMin;                                       //unit: microsecond, need change value in Q9 format
    OS_INT32  iExposureValueMax;                                       //unit: microsecond, need change value in Q9 format
    OS_INT32  iGainMax;                                   			   //0-ISO_AUTO/ISO_100,3-ISO_150,6-ISO_200,9-ISO_300,12-ISO_400, 15-ISO_600,	
                                                                       //18-ISO_800, 24-ISO_1600, 30-ISO_3200, 36-ISO_6400, 42-ISO_12800, 48-ISO_25600,
                                                                       //54-ISO_51200,60-ISO_102400 
    OS_UINT8  byRes[200];                                              //reserved
}NET_DEV_EXPOSE;                                   

typedef struct 
{
    OS_INT32  iBrightness;                                             //valid range 0~255
    OS_INT32  iContrast;                                               //valid range 0~255
    OS_INT32  iSaturation;                                             //valid range 0~255
    OS_INT32  iHue;                                                    //valid range 0~255
    OS_INT32  iSharpness;                                              //valid range 0~255
    NET_DEV_EXPOSE struExpos;                                          //device expose
    OS_UINT32  byRes[400];                                             //reserved
}NET_DEV_IMAGING;                                    

typedef struct
{
    OS_INT32 iBaudRate;                                   			   //300, 1200, 2400, 4800, 9600,19200, 38400, 115200
    OS_UINT8 byDataBit;                                   			   //7, 8
    OS_UINT8 byStopBit;                                   			   // 1, 2
    OS_UINT8 byParity;                                   			   //0:no parity check, 1:odd 2: even parity check
    OS_UINT8 byFlowcontrol;                                   		   //0:no flow control,1:hard flow control,2:soft flow control
    OS_INT32 iDecoderType;                                   		   // 1:pelco-d, 2:pelo-p, 3:aip-start
    OS_INT32 iDecoderAddress;                                   	   // 1,2,3,4,5,...255
    OS_UINT8 sRerserved[400];                                          //reserved
}NET_DEV_PTZ_DECODER;                                    

typedef struct 
{   
    OS_UINT8   byEncodeType;                                   		   //1--G.711A,2--G.711U,3--G.726
    OS_UINT8   byChan;                                   			   //1--mon,2--stero
    OS_UINT8   byBitsPerSample;                                   	   //8bit,16bit
    OS_UINT8   byReserved1;                                            //Reserved
    OS_UINT32  dwSamplesPerSec;                                   	   //8000Hz	
    OS_UINT16  wCapVolume;                                   		   //10% 20% 30%...100%
    OS_UINT16  wPlayVolume;                                   		   //10% 20% 30%...100%
    OS_UINT8   byPlayEnable;                                   		   //0—disable, 1:enable;                                   
    OS_UINT8   byAecFlag;                                   		   //0--disable,1--enable
    OS_UINT16  wAecDelayTime;                                   	   //ms
    OS_UINT16  wBitRate;                                   			   //default 64kbps
    OS_UINT16  wFrameRate;                                   		   //default 50
    OS_UINT8   sReserved[400];                                         //reserved
}NET_DEV_AUDIO_TALK_PARA;                                   

typedef struct 
{
    OS_UINT32  dwCenterEnable;                                         //1-enable, 0-disable
    OS_UINT16  wCenterPort;                                            //port number of device alarm center information 
    NET_DEV_IP_ADDR strIpAddr;                                         //IP of device alarm center
    OS_UINT8   sReserved[100];                                         //reserved
}NET_DEV_CENTER_INFO;                                   

typedef struct 
{
    OS_UINT32            dwSupportAlarmCenterNum;                      //Numbers of supporting alarm center, [read only]             
    NET_DEV_CENTER_INFO  struCenterInfo[NET_MAX_ALARM_CENTER];         //alarm center information                           
    OS_UINT8             byReserved[400];                              //reserved    
}NET_DEV_ALARM_CENTER;                                   

typedef struct 
{
    OS_UINT32  dwEnableStreamNum;                                      //Number of open stream
    OS_UINT32  dwStreamCombineNo;                                      //Code stream combination number, can be obtained from the ability
    OS_UINT8   byReserved[400];                                   	   //Reserve
}NET_DEV_ENCODE_STREAM_COMBINE;                                   

typedef struct
{
    OS_UINT8   byOperateCmd;                                   		   //Link PTZ operation command,0-no,1-preset,2-Preset point Cruise,3-Pattern scan
    OS_UINT8   byRes1[3];                                              //reserved
    OS_UINT16  wOperateSeqNum;                                   	   //Order squence：Preset point number/Cruise track number/Pattern scan number
    OS_UINT16  wPTZDelayTime;                                   	   //PTZ movement duration, unit: S
    OS_UINT8   byRes[20];                                              //reserved
}NET_DEV_LINK_PTZ;                                   

typedef struct
{
    OS_UINT32  dwAlarmOut;                                   		   //positional representation：0th place-Alarm output port 0,1th-Alarm output port 1,2th-Alarm output port 2,3th-Alarm output port 3。postion 0 denotes no setting,postion 1 denotes setting。
    OS_UINT32  dwSupportChannel;                                       //Channel number for supporting PTZ linkage(Read Only) 
    NET_DEV_LINK_PTZ  strLinkPtz[NET_MAX_CHAN];                        //Link PTZ
    OS_UINT16  wWhiteLightDelay;                                   	   //White light output duration, unit: seconds 
    OS_UINT16  wCapPicInterval;                                        //The capture time interval, unit: S
    OS_UINT16  wReserved[600];                                   	 
}NET_DEV_ALARM_LINK_INFO;                                   

typedef struct 
{
    OS_UINT32 dwLinkAlarmStrategy;                                     //Link strategy,see LINK_STRATEGY
    NET_DEV_ALARM_LINK_INFO struLinkInfo;                              //Link information      
    OS_UINT8  byRes[200];                                              //reserved
}NET_DEV_ALARM_HANDLE;                                   

typedef struct  
{
    OS_UINT32 dwEnable;                                      		   //0—disable, 1—enable
    OS_UINT32 dwCheckTime;                                   		   //Time interval for detecting alarm status, unit: ms
    NET_DEV_ALARM_HANDLE struAlarmHandle;                                  //alarm basic info
    char      sName[NET_NAME_LEN];                                     //alarm input name
    OS_UINT32 dwNormalStatus;                                   	   //0—close on common,1—open on common
    OS_UINT8  byReserved[200];                                         //reserved
}NET_DEV_ALARMIN_CFG;                                   

typedef struct 
{
    OS_UINT16 wMdX;                                                    //rect area top x coordinate                               
    OS_UINT16 wMdY;                                                    //rect area top y coordinate 
    OS_UINT16 wMdWidth;                                                //rect area width
    OS_UINT16 wMdHeight;                                               //rect area height
}NET_DEV_MOTION_AREA;                                   

typedef struct 
{
    OS_UINT32 dwEnable;                                      		   //0—disable, 1—enable
    OS_UINT32 dwCheckTime;                                   		   //Time interval for detecting alarm status, unit: ms
    NET_DEV_ALARM_HANDLE struAlarmHandle;                                  //alarm basic info
    OS_UINT16 wSensitive;                                   	       //0~10, 10-high,0-low
    OS_UINT16 wThreshold;                                   	       //0~100, 0-high,100-low
    OS_UINT32 dwValidArea;                                             //valid motion area num
    NET_DEV_MOTION_AREA struMotionArea[NET_MAX_MOTION_AREA];           //Motion area is a rect area,reference to struct:NET_DEV_MOTION DETECT                          
    OS_UINT8 byReserved[200];                                          //reserved
}NET_DEV_MOTION_DETECT;                                   

typedef struct 
{
    OS_UINT32 dwEnable;                                      		   //0—disable, 1—enable
    OS_UINT32 dwCheckTime;                                   		   //Time interval for detecting alarm status, unit: ms
    NET_DEV_ALARM_HANDLE struAlarmHandle;                                  //alarm basic info
    OS_UINT16  wSensitive;                                   		   //sensitivity range 0~100
    OS_UINT8   byReserved[202];                                        //reserved
}NET_DEV_PIR_DETECT;                                   

typedef struct 
{
    OS_UINT32 dwEnable;                                      		   //0—disable, 1—enable
    OS_UINT32 dwCheckTime;                                   		   //Time interval for detecting alarm status, unit: ms
    NET_DEV_ALARM_HANDLE struAlarmHandle;                                  //alarm basic info
    OS_UINT16  wDurationTime;                                   	   //0~5, 5-high,0-low
    OS_UINT8   byReserved[202];                                        //reserved
}NET_DEV_VIDEO_LOST_DETECT;                                   

typedef struct 
{
    OS_UINT32 dwEnable;                                      		   //0—disable, 1—enable
    OS_UINT32 dwCheckTime;                                   		   //Time interval for detecting alarm status, unit: ms
    NET_DEV_ALARM_HANDLE struAlarmHandle;                                  //alarm basic info
    OS_UINT16  wDetectType;                                            //0-menxian, 1-dousheng, 2-doujiang, 3-tubian
    OS_UINT16  wThreshold;                                             //0~100, 0-high,100-low
    OS_UINT8   byReserved[200];                                        //reserved
}NET_DEV_AUDIO_DETECT;                                   

typedef struct 
{
    OS_UINT32 dwEnable;                                      		   //0—disable, 1—enable
    OS_UINT32 dwCheckTime;                                   		   //Time interval for detecting alarm status, unit: ms
    NET_DEV_ALARM_HANDLE struAlarmHandle;                              //alarm basic info
    OS_UINT8   byReserved[200];                                        //reserved
}NET_DEV_NETWORK_BROKEN_DETECT; 

typedef struct
{
	OS_UINT16 wX;
	OS_UINT16 wY;
}NET_DEV_SMART_NAME_POINT; 

typedef struct
{
	OS_UINT16 wX;
	OS_UINT16 wY;
}NET_DEV_SMART_AREA_POINT;

typedef struct 
{
	char      sSmartName[32];
	NET_DEV_SMART_NAME_POINT stSmartNamePoint; 

	OS_UINT8  bySmartAreaVisible;
	OS_UINT8  bySmartDirection;

	NET_DEV_SMART_AREA_POINT stSmartAreaPoint[MAX_SMART_AREA_POINT];

	OS_UINT16    wSmartSensitivity; 
	OS_UINT16    wSmartTime; 
	OS_UINT16    wSmartSizeRate; 
	OS_UINT32    byReserved[20];
}NET_DEV_SMART_PARAM_AREA;

typedef struct 
{
	OS_UINT8    bySmartAreaNum;
	OS_UINT8    bySmartPointPerArea;
	OS_UINT16   wSmartRefWidth;
	OS_UINT16   wSmartRefHeight;
	OS_UINT8    bySmartRefRotate;
	OS_UINT8    bySmartColor; 

	NET_DEV_SMART_PARAM_AREA stSmartParamArea[MAX_SMART_AREA];
	OS_UINT32   byReserved[20];
}NET_DEV_SMART_DETECT_DATA;

typedef struct 
{
	OS_UINT32 dwEnable;//0--disable, 1--enable
	OS_UINT32 dwCheckTime; //ms	
	NET_DEV_SMART_DETECT_DATA stSmartDetectInfo;	
	NET_DEV_ALARM_HANDLE struAlarmHandle; 
	OS_UINT32 byReserved[200];
}NET_DEV_SMART_DETECT;

typedef struct 
{
	OS_UINT32 dwEnable;                                      		   //0—disable, 1—enable                               		   //Time interval for detecting alarm status, unit: ms
	char   sName[32];
	OS_UINT32 dwCheckTime;    
	OS_INT32  iCurrentValue;
	OS_UINT32 iLowerLimitEnable;
	OS_INT32  iLowerLimit;
	OS_UINT32 iUpperLimitEnable;
	OS_INT32  iUpperLimit;
	NET_DEV_ALARM_HANDLE struAlarmHandle;                              //alarm basic info
	OS_UINT8   byReserved[200];                                        //reserved
}NET_DEV_ENV_DETECT;

//the modes of video compression
typedef enum 
{
    COMPRESS_H264  = 16,                                               //H.264 Mode
    COMPRESS_MPEG4,                                                    //MPEG4 Mode
    COMPRESS_MJPEG,                                                    //MJPEG Mode
}ENUM_VIDEO_COMPRESSION;                                   

//the modes of bit rates control
typedef enum 
{
    BRC_CBR  = 0,                                                      //Fixed bit rates
    BRC_VBR,                                                           //variable bit rates
    BRC_CVBR,                                                          //Combination of variable rate and fixed rate,In actually, the specified maximum, minimum, average rate
    BRC_CVBR1,                                                         //Fixed bit rates 1
    BRC_NR  = 255,                                               
}ENUM_BITRATE_CONTROL;                                   

typedef struct 
{
    OS_INT32  iStreamID;                                               //code stream tab,1-Main stream,2-Auxiliary stream 1,3-Auxiliary stream 2,4-Auxiliary stream 3 
    OS_INT32  iStreamType;                                   		   //1-Video stream,2-Complex stream
    OS_INT32  iCompression;                                            //Compression mode:H264/MPEG4/MJEG       (References to ENUM_VIDEO_COMPRESSION)
    OS_INT32  iPicWidth;                                               //Audio coding width
    OS_INT32  iPicHeight;                                              //Audio coding height
    OS_INT32  iBitrateCtrl;                                            //Bit rate control mode  (Reference to  ENUM_BITRATE_CONTROL)
    OS_INT32  iQuality;                                                //Stream quality,[1,6] best--6, worst--1
    OS_INT32  iFramePerSecond;                                         // [1--60]
    OS_INT32  iBitRateAverage;                                         //Average of bit rates ,kbps
    OS_INT32  iBitRateUp;                                              //bit rates up
    OS_INT32  iBitRateDown;                                            //bit rates down
    OS_INT32  iGop;                                                    //the numbers of P-Frame between two I-Frames,[1,149]
    OS_INT32  iRotate;                                                 //0—Not rotate,1—rotate
    OS_UINT8  byRes[100];                                              //reserved
}NET_DEV_ENCODE_INFO;                                   

typedef struct  
{   
    OS_UINT32 dwStreamNum;                                             //number of stream,read only
    NET_DEV_ENCODE_INFO struEncodeInfo[NET_MAX_STREAM];                //Device encode information                    
    OS_UINT8  byRes[400];                                              //reserved
}NET_DEV_ENCODE_CFG;                                   

typedef struct 
{
    OS_UINT32 dwFileFormat;                                            //1 :mp4
    char      sPath[NET_MAX_PATH_LEN];                                 //file path
    OS_UINT8  byRes[200];                                              //reserved
}NET_DEV_SAVE_FILE_PARA;                                   

typedef struct 
{
    OS_UINT32 dwValidSegmentNum;                                       //max segment number support, read only
    NET_DEV_SCHEDLUE sScheduleTime[NET_MAX_SCHEDULE_DAY][NET_MAX_SCHEDULE_SEGMENT];       //sScheduleTime[0]:Monday ...sScheduleTime[7]:reserved
    OS_UINT8  sReserved[400];                                          //reserved
}NET_DEV_ALARM_SCHEDULE;                                   

typedef struct 
{
    OS_UINT32  dwEnable;                                       		   //0—disable, 1—enable
    char       sName[NET_NAME_LEN];                                    //alarm output name
    OS_UINT32  dwNormalStatus;                                   	   //0—Closed circuit output, 1—Open circuit output
    OS_UINT32  dwDelayTime;                                   		   //Output duration, unit: seconds
    OS_UINT8   byRes[400];                                             //reserved
}NET_DEV_ALARMOUT_CFG;                                   

typedef struct
{
    OS_UINT32  wTopX;                                                  //All translated into(0,0)~(100,100)
    OS_UINT32  wTopY;                                                  //All translated into(0,0)~(100,100)
    OS_UINT32  wWidth;                                                 //All translated into(0,0)~(100,100)
    OS_UINT32  wHeight;                                                //All translated into(0,0)~(100,100)
    OS_UINT8   byColor;                                   		       //0--white, 1--black
    OS_UINT8  sReserved[79];                                           //reserved
}NET_DEV_HIDE_AREA;                                   

typedef struct  
{
    OS_UINT32 dwValidAreaNum;                                          //valid area number, read only
    NET_DEV_HIDE_AREA struHideAreas[NET_MAX_HIDE_AREA];                //hide area info
    OS_UINT8  byRes[200];                                              //reserved
}NET_DEV_HIDE_CFG;  

typedef struct 
{
	OS_UINT32 left;
	OS_UINT32 top;
	OS_UINT32 right;
	OS_UINT32 bottom;
}NET_DEV_RECT;

typedef struct
{
    OS_UINT32     dwAlarmType;                                         //IPC alarm type, see:enum IPC_ALARM_TYPE
    OS_UINT32     dwIDHeight;										   //报警消息id（64位整数）高32位数据，该id为报警信息数据库的行记录id，可以唯一地代表一条报警信息
    OS_UINT32     dwIDLow;                  						   //报警消息id（64位整数）低位32数据，该id为报警信息数据库的行记录id，可以唯一地代表一条报警信息
    OS_UINT32	  dwLevel;                 						       //报警等级(0,1,2….) ,0等级最高,数据越大等级越低
    OS_UINT32	  dwState; 										       //1报警开始0报警结束
    char	      sTimeStamp[32];								 	   //时间戳 格式YYYYMMDDHHMMSS;如：20120802235502
    char	      sDescript[64];									   //报警信息描述
    OS_UINT8      byReserved[800];		    						   //保留数据
}NET_DEV_IPC_ALARM;

typedef struct
{
	OS_UINT32	dwAlarmType;
	OS_UINT32	dwLevel;
	OS_UINT8	sDiskName[32];
	OS_UINT8	sDescription[128];
	OS_UINT8    byReserved[64];
}NET_DEV_Disk_Alarm;

//transparent com
#define MAX_COM_TRANSPORT_DATA_LENGTH  1024
typedef struct tagNetDevComData
{
    OS_UINT32 dwChannel;//not used now
    OS_UINT32 dwDataSize;
    OS_UINT8  byDataBuf[MAX_COM_TRANSPORT_DATA_LENGTH];
    OS_INT32  iReserved[8];
}NET_DEV_COM_DATA;

typedef struct tagNetDevReadComDataInfo
{
    OS_UINT32 dwChannel;//not used now
    OS_UINT32 dwDataSize;
    OS_INT32  iReserved[8];
}NET_DEV_READ_COM_DATA_INFO;

typedef struct tagNetDevComInitiativeUploadConfig
{
    OS_BOOL   bUpload2ServerEnable;
    OS_UINT32 dwMaxWaitTime;  //unit:ms, uplaod when time_out >= dwMaxWaitTime; when it is 0, upload immediately
    OS_UINT32 dwMinSendLen;   //uint:byte,value <= MAX_COM_INITIATIVE_UPLOAD_DATA_LENGTH; upload when data_vytes >= dwLenLimit; when it is 0,upload immediately
    OS_BOOL   bDetachPelco;   //default: FALSE
    OS_INT32  iReserved[8];
} NET_DEV_COM_INITIATIVE_UPLOAD_CONFIG;

#define MAX_COM_INITIATIVE_UPLOAD_DATA_LENGTH  50
typedef struct tagNetDevComInitiativeUploadData
{
    OS_UINT8 byChannel;
    OS_UINT8 byPacketIndex; 
    OS_UINT8 byDataSize;
    OS_UINT8 byDataBuf[MAX_COM_INITIATIVE_UPLOAD_DATA_LENGTH];
} NET_DEV_COM_INITIATIVE_UPLOAD_DATA;

typedef struct
{
    OS_UINT16  wYear;
    OS_UINT8  byMonth;
    OS_UINT8  byDay;
    OS_UINT8  byHour;
    OS_UINT8  byMinute;
    OS_UINT8  bySecond;
    OS_UINT8  byRes;
    OS_UINT16 wMilliSec;
    OS_UINT8  byRes1[2];
}NET_DEV_TIME_EX;

typedef struct
{
    float fX;               //边界框左上角点的X轴坐标, 0.001~1
    float fY;               //边界框左上角点的Y轴坐标, 0.001~1
    float fWidth;           //边界框的宽度, 0.001~1
    float fHeight;          //边界框的高度, 0.001~1
}NET_DEV_RECT_EX;

typedef struct
{
    OS_UINT8  byColor;						//车牌颜色 see: enum _PLATE_COLOR_
    OS_UINT8  byLicenseLen;					//车牌字符个数 
    OS_UINT8  byEntireBelieve;					//整个车牌的置信度，1-100 
    NET_DEV_RECT_EX	struPlateRect;		//车牌位置 
    char sLicense[16];		//车牌号码 
}NET_DEV_PLATE_INFO;

typedef struct
{
    OS_UINT32   dwDataLen;		    //媒体数据长度 
    OS_UINT8    byType;			    //0:车牌图;1:场景图;
    OS_UINT8    byFormat;           //1 jpeg 2 YUV
    OS_UINT8    byRes1[2];
    OS_UINT64	lDataPtr;	            //数据指针
    OS_UINT8    byRes2[12];   		//保留
}NET_DEV_PLATE_PICTURE;

typedef struct
{
    OS_UINT32            dwSize;		//结构长度
    NET_DEV_TIME_EX      struCaptureTime;//端点时间(ms)（抓拍第一张图片的时间）
    NET_DEV_PLATE_INFO   struPlateInfo; 	//车牌信息结构
    OS_UINT32            dwPicNum;		//图片数量（与picGroupNum不同，代表本条信息附带的图片数量，图片信息由struVehicleInfoEx定义	
    NET_DEV_PLATE_PICTURE struPicInfo[6]; 		//图片信息,单张回调，最多6张图，由序号区分
    OS_UINT8             byRes[200];
}NET_DEV_PLATE_CAPTURE;

#define MAX_POLYGON_POINTS     8     //多边形最多的点个数
#define MAX_AREA_NUM           12

typedef struct
{
	OS_UINT16 wX;
	OS_UINT16 wY;

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		T_Hton(wX);
		T_Hton(wY);
	}
}NET_DEV_DETECT_POINT;

typedef struct
{
	OS_UINT16 wX;
	OS_UINT16 wY;
	OS_UINT16 wWidth;
	OS_UINT16 wHeight;

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		T_Hton(wX);
		T_Hton(wY);
		T_Hton(wWidth);
		T_Hton(wHeight);
	}
}NET_DEV_DETECT_AREA;

typedef struct
{
	OS_UINT8  byValidPointNum;
	OS_UINT8  byRes[3];
	NET_DEV_DETECT_POINT points[MAX_POLYGON_POINTS];

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		for(int i = 0; i < MAX_POLYGON_POINTS; i++)
		{
			points[i].HostNetTrans();
		}
	}
}NET_DEV_DETECT_POLYGON;

typedef struct
{
	OS_UINT8  byEnable;    //1 启用  2 不启用
	OS_UINT8  byRes[3];    //保留

	NET_DEV_DETECT_POLYGON polygon;

	OS_UINT32 nHideAreaNum;    //遮挡区域个数
	NET_DEV_DETECT_AREA struHideArea[MAX_AREA_NUM];  //遮挡区域防止影响计算

	OS_UINT32 nMinArea;      //面积小于该区域不作判断
	OS_UINT32 nMaxObjectNum;    //一个画面中存在超过此数目的目标是则不解析

	OS_UINT8  byRes2[4];    //保留

	struct
	{
		OS_UINT8 byKeyPointRatio; //角点覆盖率,1~100
		OS_UINT8  byRes[3];    //保留
	}struTypeParam;

	struct
	{
		OS_UINT8 nThresholdTime;    //多长时间判定为停车,单位:秒
		OS_UINT8  byRes[3];    //保留
	}struStayParam;

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		polygon.HostNetTrans();
		T_Hton(nHideAreaNum);
		for(int i = 0; i < MAX_AREA_NUM; i++)
		{
			struHideArea->HostNetTrans();
		}

		T_Hton(nMinArea);
		T_Hton(nMaxObjectNum);
	}
}NET_DEV_VEHICLE_DETECT;

typedef struct
{
	OS_UINT8  byEnable;    //1 启动  2 不启用
	OS_UINT8  byFloatThreshold;    //存在漂浮物的比例占多大判定为存在漂浮物, 1~100
	OS_UINT8  byRes[2];    //保留
	NET_DEV_DETECT_POLYGON polygon;

	OS_UINT32 nMinArea;      //面积小于该区域不作判断

	OS_UINT32 nFloatWindowLen;    //漂浮物判定的时间窗口长度

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		polygon.HostNetTrans();
		T_Hton(nMinArea);
		T_Hton(nFloatWindowLen);
	}
}NET_DEV_FLOAT_DETECT;


typedef struct
{
	OS_UINT8 byEnable;
    OS_UINT8 byLiftArmRatio;//default value:30, unit:%
	OS_UINT8 byRes[10];

	NET_DEV_DETECT_POLYGON polygon;

	OS_UINT16 wMinWidth;   //单位:像素
	OS_UINT16 wMaxWidth; 
	OS_UINT16 wMinHeight;  //单位:像素
	OS_UINT16 wMaxHeight; 

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		polygon.HostNetTrans();

		T_Hton(wMinWidth);
		T_Hton(wMaxWidth);
		T_Hton(wMinHeight);
		T_Hton(wMaxHeight);
	}
}NET_DEV_LIFTARM_AREA;

typedef struct
{
	NET_DEV_LIFTARM_AREA area[4];
	OS_UINT8  byRes2[60];

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		for(int i = 0; i< 4; i++)
		{
			area[i].HostNetTrans();
		}
	}
}NET_DEV_LIFTARM_DETECT;

#define NET_DEV_DEVICE_ID_LENGTH 17
typedef struct
{
	OS_UINT8  byEnable;   //是否上传
	OS_UINT16 wPort;     //端口号
	OS_UINT8  byHeartbeatInterval;
	char      sDeviceId[NET_DEV_DEVICE_ID_LENGTH];
	OS_UINT8  byRes[15];  //保留
	char sIP[16];

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		T_Hton(wPort);
	}
}NET_DEV_VEHICLE_SERVER;

typedef enum
{
	E_PLATE_NONE = 0,
	E_PLATE_OPEN_GATE = 1,
	E_PLATE_EXT1,
	E_PLATE_EXT2,
	E_PLATE_EXT3,
	E_PLATE_EXT4,
	E_PLATE_EXT5,
}PLATE_CMD_ID;

typedef struct
{
	OS_UINT32 dwCmdId;
	OS_UINT32 dwReserved[8];
	OS_UINT32 dwDisLen;
	char 	sDisStr[256];

	void HostNetTrans(OS_BOOL bToNet = OS_TRUE)
	{
		T_Hton(dwCmdId);
		T_Hton(dwDisLen);
	}
}NET_DEV_PLATE_CMD_INFO;

#define NET_DEV_ELECTRIC_POWER_CABLE_NUMBER_LENGTH 32
#define NET_DEV_ELECTRIC_POWER_INTEGRATION_NUMBER_LENGTH 32
#define NET_DEV_ELECTRIC_POWER_TOWER_NUMBER_LENGTH 32
#define NET_DEV_ELECTRIC_POWER_DEVICE_NUMBER_LENGTH 32
#define NET_DEV_ELECTRIC_POWER_FORMAT_VERSION_LENGTH 32
typedef struct
{
    char      sCableNumber[NET_DEV_ELECTRIC_POWER_CABLE_NUMBER_LENGTH];
    char      sIntegratioNumber[NET_DEV_ELECTRIC_POWER_INTEGRATION_NUMBER_LENGTH];
    char      sTowerNumber[NET_DEV_ELECTRIC_POWER_TOWER_NUMBER_LENGTH];
    char      sDeviceNumber[NET_DEV_ELECTRIC_POWER_DEVICE_NUMBER_LENGTH];
    char      sFormatVersion[NET_DEV_ELECTRIC_POWER_FORMAT_VERSION_LENGTH];
    void HostNetTrans()
	{
	}
}NET_DEV_ELECTRIC_POWER_ALARM_CONFIG;

typedef struct
{
    char      sDeviceNumber[NET_DEV_ELECTRIC_POWER_DEVICE_NUMBER_LENGTH];
    OS_UINT16 wRatedVoltage;
    OS_UINT16 wRatedOverloadCurrent;
    OS_UINT16 wRatedChargeCurrent;
    void HostNetTrans()
	{
		T_Hton(wRatedVoltage);
		T_Hton(wRatedOverloadCurrent);
		T_Hton(wRatedChargeCurrent);
	}
}NET_DEV_ELECTRIC_POWER_BATTERY_BASE_INFO;

#define NET_DEV_ELECTRIC_POWER_BATTERY_CONFIG_RESERVE_LENGTH 24
typedef struct
{
    OS_UINT8  byBatteryType;
    OS_UINT8  byChargeWay;
    OS_UINT16 wBatteryVolume;
    OS_INT16  wTemperatureCompensateCoefficient;
    OS_UINT16 wOverrunBreakVoltage;
    OS_UINT16 wOverrunBreakRecoveryVoltage;
    OS_UINT16 wBalancedChargeVoltage;
    OS_UINT16 wLiftingChargeVoltage;
    OS_UINT16 wFloatingChargeVoltage;
    OS_UINT16 wLiftingRecoveryVoltage;
    OS_UINT16 wBatteryChangePercent;
    OS_UINT8  byRatedVoltageLevel;
    OS_UINT8  byReserve1;
    OS_UINT16 wLiftingSustainedTime;
    OS_UINT16 wBalancedSustainedTime;
    OS_UINT16 wChangeLimitVoltage;
    OS_UINT16 wDischangeLimitVoltage;
    OS_UINT16 wLowVoltageBreakVoltage;
    OS_UINT16 wLowVoltageBreakRecoveryVoltage;
    OS_UINT16 wUnderrunAlarmVoltage;
    OS_UINT16 wUnderrunAlarmRecoveryVoltage;
    OS_UINT16 wBatteryDischangePercent;
    OS_UINT8  byReserve2[NET_DEV_ELECTRIC_POWER_BATTERY_CONFIG_RESERVE_LENGTH];
    void HostNetTrans()
	{
		T_Hton(wBatteryVolume);
		T_Hton(wTemperatureCompensateCoefficient);
		T_Hton(wOverrunBreakVoltage);
		T_Hton(wOverrunBreakRecoveryVoltage);
		T_Hton(wBalancedChargeVoltage);
		T_Hton(wLiftingChargeVoltage);
		T_Hton(wFloatingChargeVoltage);
		T_Hton(wLiftingRecoveryVoltage);
		T_Hton(wBatteryChangePercent);
		T_Hton(wLiftingSustainedTime);
		T_Hton(wBalancedSustainedTime);
		T_Hton(wChangeLimitVoltage);
		T_Hton(wDischangeLimitVoltage);
		T_Hton(wLowVoltageBreakVoltage);
		T_Hton(wLowVoltageBreakRecoveryVoltage);
		T_Hton(wUnderrunAlarmVoltage);
		T_Hton(wUnderrunAlarmRecoveryVoltage);
		T_Hton(wBatteryDischangePercent);
	}
}NET_DEV_ELECTRIC_POWER_BATTERY_CONFIG;

#define NET_DEV_MAKE_32BITS_DATA(b1, b2, b3, b4)    (((b1) << 24) | ((b2) << 16) | ((b3) << 8) | (b4))

/// Unknown media type
#define NET_DEV_MEDIA_TYPE_UNKNOWN  NET_DEV_MAKE_32BITS_DATA('U', 'N', 'K', 'N')
/// H.264
#define NET_DEV_MEDIA_TYPE_H264     NET_DEV_MAKE_32BITS_DATA('H', '2', '6', '4')
/// MJPEG
#define NET_DEV_CLIENT_MEDIA_TYPE_MJPEG    NET_DEV_MAKE_32BITS_DATA('J', 'P', 'E', 'G')
/// G.711 aLaw
#define NET_DEV_MEDIA_TYPE_G711A    NET_DEV_MAKE_32BITS_DATA('G', '7', '1', '1')
/// G.711 uLaw
#define NET_DEV_MEDIA_TYPE_G711U    NET_DEV_MAKE_32BITS_DATA('g', '7', '1', '1')

typedef void (SDKCALL *REAL_DATA_CBF)(OS_INT32 lRealHandle, OS_UINT32 dwDataType, OS_UINT8 *pBuffer, OS_UINT32 dwBufSize, OS_VOIDPTR pUser);                                   
typedef void (SDKCALL *VOICE_DATA_CBF)(OS_INT32 lVoiceComHandle, char *pRecvDataBuffer, OS_UINT32 dwBufSize, OS_UINT8 byAudioFlag, OS_VOIDPTR pUser);                                   
typedef void (SDKCALL *ALARM_INFO_CBF)(OS_INT32 lAlarmType, NET_DEV_ALARMER *pAlarmer,char *pAlarmInfo, OS_UINT32 dwBufLen, OS_VOIDPTR pUser);                                   
typedef void (SDKCALL *EXCP_INFO_CBF)(OS_UINT32 dwType, OS_INT32 lUserID, OS_UINT32 lHandle, OS_VOIDPTR pUser);                                   
typedef void (SDKCALL *PLAY_DATA_CBF)(OS_INT32 lPlayHandle, OS_UINT32 dwDataType, OS_UINT8 *pBuffer, OS_UINT32 dwBufSize, OS_VOIDPTR pUser);                                   
typedef void (SDKCALL *PLAY_DRAW_CBF)(OS_INT32 lPlayHandle, HDC hdc, OS_VOIDPTR pContext);                                   

//camera linkage
typedef struct
{
	NET_DEV_IP_ADDR  struIP;
	OS_INT32         nPort;
	char             strUserName[20];
	char             strPassword[20];
}NET_DEV_TRACEIPC_INFO;

#define MAX_TRACEIPC_NUM   (4)
typedef struct
{
	NET_DEV_TRACEIPC_INFO   struTraceIPC[MAX_TRACEIPC_NUM]; 
	OS_UINT8                byTraceTimeout;
	OS_UINT8                byTraceSensitive;
	OS_UINT8                byTraceFlag;
	OS_UINT8                byReserved;
}NET_DEV_CAMERA_LINKAGE_CFG;


typedef struct 
{
    OS_INT32	iPTZCommand;													
    OS_INT32	iX;													
    OS_INT32    iY;													
    OS_INT32	iZ;													
    OS_INT32    iReserved[2];
}NET_DEV_PTZ_POS_PARA;
   

// alarm expand common structure
typedef struct
{
    OS_INT32 dwType;
    OS_INT32 dwLength;
}NET_DEV_ALARM_HEADER_EX;

/*************electrical line*********/
typedef enum tagElectricalLineType
{
    eElectricalLinePic = 0,
    eElectricalLineAlarm,
    eElectricalLineHB,
} ElectricalLineType;
typedef struct
{
    NET_DEV_ALARM_HEADER_EX struHdr;
	char	 sCmdId[20];
    OS_UINT8 byChannelNo;
    OS_UINT8 byPresentNo;
	OS_UINT8 byReserved[2];
	OS_INT32 dwFileLen;
    char     *sFileData;  
}NET_DEV_ELECTRICAL_LINE_PIC;

typedef struct
{
    NET_DEV_ALARM_HEADER_EX struHdr;
    char	  sCmdId[20];
    OS_UINT8  byChannelNo;
    OS_UINT8  byAlarmType; // 0-火山 1-异物 2-机械外破
    OS_UINT8  byAlarmLevel;// 0-一般 1-严重 2-危急
	OS_UINT8  byReserved[1];
    OS_UINT32 dwTimeStamp;
    char	  sMediaName[256];
}NET_DEV_ELECTRICAL_LINE_ALARM;

typedef struct
{
    NET_DEV_ALARM_HEADER_EX struHdr;
    char	  sCmdId[20];
    OS_UINT8  byFloatingCharge;
	OS_UINT8  byConnState;
	OS_UINT8  byReserved[2];
    OS_UINT32 dwTimeStamp;
    OS_UINT32 dwTotalTime;
    OS_UINT32 dwWorkTime;
    OS_UINT32 dwSendFlow;
    OS_UINT32 dwRecvFlow;
    float	  fBatteryVoltage;
    float     fTemperature; 
    float     fBatteryCapacity;
}NET_DEV_ELECTRICAL_LINE_HB;
/*************electrical line end*********/

#define MAX_LANE_NUM 6
#define MAX_VEHICLE_TYPE_NUM 10
typedef struct
{
    /*统计间隔时间*/
    OS_UINT32 s_StatisticalInterval; //单位：秒
    /*统计流量值*/
    OS_UINT32 s_VehicleTotalCount;
    OS_UINT32 s_VehicleCountByType[MAX_VEHICLE_TYPE_NUM]; //每个数组元素代表不同车型的流量计数，暂时保留
    //其他
    OS_UINT32 s_Reserve[6];
}NET_DEV_VEHICLE_FLOW_INFO;

typedef struct
{
    NET_DEV_ALARM_HEADER_EX struHdr;
    OS_UINT8        s_SceneId[32];
    OS_UINT32       s_StatisticalTime; //当前上报的时间点,UTC时间
    OS_INT32        s_ValidLaneNum;
    NET_DEV_VEHICLE_FLOW_INFO  s_FlowStatInfo[MAX_LANE_NUM];  
    
}NET_DEV_VEHICLE_FLOW_ALARM;

typedef struct 
{
    OS_UINT32 dwEnable;//0--disable, 1--enable
    OS_UINT32 dwCheckTime; //unit:s,上报间隔时间
    OS_UINT32 byReserved[200];
}NET_DEV_VEHICLE_FLOW_DETECT;

typedef enum
{
	e_VIDEO_H264_I = 1,
	e_VIDEO_H264_P,
	e_AUDIO_G711A,
	e_AUDIO_G711U,
}PrivateStreamCBType;

#endif
