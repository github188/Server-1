#ifndef _IVHS_STRUCT_H_
#define _IVHS_STRUCT_H_

#pragma pack(push, 1)
#ifdef _MSC_VER
	#define PACKED
#else
    #define PACKED
#endif //LINUX

#include <stdio.h>
#include <string.h>

#include "ivhs_define.h"

/*----------------------------------------------------------
*通用结构体
*----------------------------------------------------------*/

//违法视频信息
typedef struct
{
	//违法视频存储类型 0: HTTP, 1: FTP
	Uint32 dwIllvideotype;
	//违法视频开始时间（20140905100010）
	Char achIllvideostarttime[MAX_LEN_VIDEOR_TIME];
	//违法视频结束时间（20140905100030）
	Char achIllvideoendtime[MAX_LEN_VIDEOR_TIME];
	//违法视频全路径
	Char achIllvideofullurl[MAX_LEN_VIDEOR_URL];
}TVideInfo;

//实时过车信息
typedef struct
{
    /*必要  过车记录编号*/
    Uint32 dwSeqNumber;
    /*可选  设备名称，可以是中文字符*/
    Char achPointNum [MAX_SIZE_POINT_NUM];
    /*必要  前端设备编号，数字字符串*/
    Char achDevId[MAX_SIZE_DEV_ID] ;
    /*必要  过车时间，即前端抓拍图片的时间*/
    Int64 qwThroughTime;
    /*必要  车道号(如1、2、3…)*/
    Uint32 dwLaneNum ;
    /*可选  车辆行驶方向*/
    Char achDirection[MAX_SIZE_DIRECTION];
    /*必要  限速值*/
    Uint32 dwSpeedLimit;
    /*必要  车辆行驶速度*/
    Uint32 dwVehicleSpeed;
    /*必要  车头车牌号码（如京A12345）*/
    Char achFrontPlateNum[MAX_SIZE_PLATE_NUM];
    /*可选  车头车牌颜色（见车牌颜色定义）*/
    Uint32 dwFrontPlateColor;
    /*可选  车头车牌种类*/
    Uint32 dwFrontPlateType;
    /*可选  车尾车牌号码（如京A12345）*/
    Char achBackPlateNum[MAX_SIZE_PLATE_NUM];
    /*可选  车尾车牌颜色（见车牌颜色定义）*/
    Uint32 dwBackPlateColor;
    /*可选  车尾车牌种类*/
    Uint32 dwBackPlateType;
    /*可选  号牌一致*/
    Uint32 dwConsistency;
    /*可选  车身颜色*/
    Uint32 nVehiclecColor;
    /*可选  车辆类型*/
    Uint32 dwVehicleType;
    /*可选  车身长度*/
    Uint32 dwVehicleLength ;
    /*必要  0为正常，1为违法，2为事件*/
    Uint32 dwIllegalState ;
    /*必要  违法或事件类型*/
    Char achIllegalType[MAX_SIZE_ILLEGAL_TYPE];
    /*必要  0-图片数据；1-图片URL*/
    Int32 nPhotoType;
    /*可选  车牌坐标左上角X坐标*/
    Uint32 dwPlatePointLeftX;
    /*可选  车牌坐标左上角Y坐标*/
    Uint32 dwPlatePointLeftY;
    /*可选  车牌坐标右上角X坐标*/
    Uint32 dwPlatePointRightX;
    /*可选  车牌坐标右上角Y坐标*/
    Uint32 dwPlatePointRightY;
    /*可选  闯红灯，红灯时间*/
    //Uint32 dwRushedTime[MAX_NUM_IMAGE];
    /*可选  红灯开始时间*/
    Int64  qwRLampStartTime;
    /*可选  红灯结束时间*/
    Int64  qwRLampStopTime;
    /*车辆标识（如大众）*/
    Char achVehiclelogo[MAX_SIZE_LEN32];
    /*细分类型（如桑塔纳3000），注意与车辆类型相区别（大、中、小）*/
    Char achDetailedtype[MAX_SIZE_LEN32];
	/*可选	类型识别 车辆种类（如汽车，参见车辆种类）*/
	Uint32 dwVehicleclass;
	/*可选	子类型识别 车辆子种类（参见车辆子类型定义）*/
	Uint32 dwVehiclesubclass;
	//违法视频个数
	Uint32 dwIllvideonum;
	//违法视频信息
	TVideInfo atVideoInfo[MAX_NUM_VIDEOR_INFO];
    /*道路名称*/
    Char szSceneName[64];
    /*道路编码*/
    Char szRoadCode[32];

	/*以下为公安视图库信息必选项*/
	/*图片采集方式,参考enum EInfoKind*/
	Int32 dwInfoKind;
	/*图片的格式,参考enum EFileFormat*/ 
	Uint32 dwFileFormat;
	/*图片信息来源,参考enum EDataSourceType*/ 
	Uint32 dwDataSourceType;
	/*车辆轮廓外接矩形在画面中的坐标左上角X坐标*/
	Uint32 dwVehcPointLeftX;
	/*车辆轮廓外接矩形在画面中的坐标左上角Y坐标*/
	Uint32 dwVehcPointLeftY;
	/*车辆轮廓外接矩形在画面中的坐标右上角X坐标*/
	Uint32 dwVehcPointRightX;
	/*车辆轮廓外接矩形在画面中的坐标右上角Y坐标*/
	Uint32 dwVehcPointRightY;
	/*图片的宽带*/
	Uint32 dwImgWidth;
	/*图片的高度*/
	Uint32 dwImgHeight;
	/*图像资料名称的汉语描述*/
	Char achTitle[STRING_MAX_LENGTH_128];
	/*对图像内容的简要描述*/
	Char achContentDescription[STRING_MAX_LENGTH_256];
	/*自动分析事件类型,参考enum EEventSort*/
	Int32 dwEventSort;
	/*可选 注册唯一PUID,*/
    Char achPuId[MAX_SIZE_DEV_ID] ;

	/*采集机关代码*/
	Char achCapDepartment[MAX_SIZE_LEN16];

}PACKED TVechileInfo;

typedef struct
{
    /*图片的大小*/
    Uint32 dwSize;
    /*图片抓拍时间*/
    Int64 qwTime;
    /*图片类型*/
    Uint32 dwType;
    /*图片防伪码*/
    Char szAntifake[16+1];
	/*图片数据的url*/
	Char achUrl[STRING_MAX_LENGTH_256];
    /*图片数据*/
    void *szBuf;
}PACKED TImageData;

typedef struct
{
    TVechileInfo tInfo;
    /*图片张数*/
    Uint32 dwImageNum;
    TImageData atImage[MAX_NUM_IMAGE];
}PACKED TVechileData;

/*接收过车数据反馈结果*/
typedef struct
{
    /*必要  过车记录编号*/
    Uint32  dwSeqNumber;
    /*必要  0-成功；非0-失败*/
    Uint32  nReturncode;
	/*必要  前端设备编号，数字字符串*/
    Char achDevId[MAX_SIZE_DEV_ID] ;

}PACKED TIPC_VechileDataAck;

typedef struct
{
    /*必要 设备编号*/
    Char achDevId[MAX_SIZE_DEV_ID];
    /*必要 校时时间（2011-04-21 17:23:22)*/
    Char achControlsetTime[64];
    /*必要 默认是：GMT+8*/
    Char achTimeZone[16];
}PACKED TTimeSycCfg;


typedef struct
{
    /*必要 设备PUID*/
    Char achDevId[MAX_SIZE_DEV_ID];
    Int32 nRetCode;
}PACKED TTimeSycRspInfo;

/*上传完毕*/
typedef struct
{
    /*必要	设备编号*/
    Int8 achDevId[64];
}PACKED TUpLoadFinishedInfo;

typedef struct
{
    /*图片抓拍时间。北京时间，见GB/T 7408-2005标准中
    5.4.1的时间扩展格式：YYYY-MM-DDTHH:mm:ss.sss*/
    Char szSnapTime[32];
    /*图片标志：0-图片数据流；1-图片URL*/
    Char szPicFlag[2];
    /*图片类别 取值为：0-全景图片，1-特写图片 ，2-车中司乘人员图片*/
    Char szPicType[2];
    /*图片URL*/
    Char szPicUrl[256];
    /*图片二进制流的ID，唯一标示*/
    Char szPicStreamId[58];
    /*图片中特写的坐标，表示为（左上X，左上Y，右下X，右下Y）?    纭?,1,100,110”。如多组坐标，使用";"分隔*/
    Char szFtrCoord[64];
}PACKED TPicDtl;

typedef struct
{
    /*图片数量*/
    Int32 nPicNum;
    TPicDtl atPicDlt[MAX_NUM_IMAGE];
}PACKED TCommonPicInfo;

typedef struct
{
    /*图片属性信息*/
    TCommonPicInfo tPicInfo;
    /*设备ID*/
    Char achDevId[50];
    /*设备通道号*/
    Int32 nChannelId;
    /*目标出现时间。北京时间，
    见GB/T 7408-2005标准中5.4.1的时间扩展格式：YYYY-MM-DDTHH:mm:ss.sss*/
    Char achInTime[32];
    /*目标消失时间。*/
    Char achOutTime[32];
    /*国标里定义的最佳时间，是目标最清晰的时间。
    目前只有视频摘要算法能给出。*/
    Char achBestTime[32];
    /*智能识别算法类型*/
    Char achIAlgType[32];
    /*车道号*/
    Int32 nLane;
    /*有无车牌：0：无、1：有*/
    Bool bIfHavePlt;
    /*车牌种类：如“01-大型汽车车牌，02-小型汽车车牌，
    03-使馆汽车车牌”，按GA24.7-2005编码*/
    Char achPltType[32];
    /*车牌颜色：0-白色;1-黄色;2-蓝色;3-黑色;4-其它(GA/T 669.9-2008表A.7)*/
    Char achPltCol[32];
    /*车牌号码*/
    Char achPltNum[32];
    /*是否涂改*/
    Bool bIsPltAlt;
    /*是否遮挡*/
    Bool bIsPltBlc;
    /*车的行驶速度：单位km/h*/
    Int32 nSpeed;
    /*车的限制速度，最高限速：单位km/h*/
    Int32 nLimitSpeed;
    /*车、运行状态：0－正常，1－嫌疑，2072超速，
    2027逆行 见GA/T 669.9-2008表A.1*/
    Char achStatus[32];
    /*被标注车辆的品牌，与车标一致*/
    Char achBrand[32];
    /*被标注车辆的款式型号，按GB/T 9417-1998命名*/
    Char achStyle[32];
    /*车辆长度，单位为CM*/
    Int32 nVLength;
    /*车辆宽度，单位为CM*/
    Int32 nVWidth;
    /*车辆高度，单位为CM*/
    Int32 nVHeight;
    /*车身颜色，按GA24.8-2005编码*/
    Char achVColType[32];
    /*违法模型，如为多项，以逗号分隔。0:超速 1:逆行 2:闯红灯 3:禁止掉头
    4:不按车道行驶  5:区间超速  6:无车牌 7:违章停车 8:压实线*/
    Char achTrafficOffence[32];
}PACKED TPicVehcInfo;

typedef struct
{
    /*姿势*/
    Char achPose[10];
    /*体表特征*/
    Char BoduSFtr[10];
    /*是否驾驶员-*/
    Bool bIsDriver;
}PACKED TPersonPicInfo;

typedef struct
{
    TCommonPicInfo tPicInfo;
    /*物体形状*/
    Char achShape[10];
    /*物体大小*/
    Char achSize[50];
    /*物体颜色*/
    Char achColour[2];
    /*物体速度*/
    Int32 nSpeed;
}PACKED TObjectPicInfo;

typedef struct xmlCommMsgData
{
    /* 透明通道消息类型：5000 闸机控制请求，5001 闸机控制确认 .... */
    Uint16 wMsgType;
    /*透明消息长度*/
    Uint16 wMsgLen;
    /*具体消息, 如为闸机控制请求，则填充TOpDoorReq*/
    Int8 xmlTransDataBuf[MAX_TRAN_DATA_NUM];
}PACKED TCommMsgData;

/*控制数据共用体*/
typedef union
{
    /*并口控制时：1:开；2:关；*/
    Uint32 POpDoor;
    /*串口控制时：设备控制协议。*/
    Int8 SOpDoor[MAX_CON_PROT_DATA];
}PACKED TOpDoorData;

/*闸机控制请求*/
typedef struct
{
    /*1：串口；2：并口*/
    Uint16 OpPortType;
    /*并口或者串口ID号*/
    Uint16 PortId;
    /*流水号*/
    Uint16 wTransNum;
    /*串口控制时：设备控制协议的长度*/
    Uint16 OpDoorDataLen;
    /*并口控制时：1:开；2:关；串口控制时：设备控制协议,address会包含在tOpDoorData.SOpDoor内。*/
    TOpDoorData  tOpDoorData;
}PACKED TOpDoorReq;

/*闸机控制确认*/
typedef struct
{
    /*是否成功  1成功，0 失败*/
    Uint32 nRetCode;
    /*流水号*/
    Uint16 wTransNum;
}PACKED TOpDoorAck;

/*串口信息*/
typedef struct
{
    /*串口id*/
    Uint16 wSerialId;
    /*串口信息*/
    Int8 achState[MAX_CON_PROT_DATA];
}PACKED TSerialStateInfo;

/*视频码流请求*/
typedef struct
{
    /*通道ID*/
    Uint8        byChnId;
    /*码流传输端口*/
    Uint16       wStreamPort;
    /*码流传输地址*/
    Uint32       dwStreamIpAddr;
    /*码流传输时的SSRc*/
    Int32        nSsrc;
}PACKED TMediaCtrl;

/*获取相机时间消息确认*/
typedef struct
{
    /*必要 设备PUID*/
    Char achDevId[MAX_SIZE_DEV_ID];
    long int CamTime_Sec;
    long int CamTime_uSec;
}PACKED TCamTimeRspInfo;

/*----------------------------------------------------------
 *协议结构体
 *----------------------------------------------------------*/
typedef struct
{
    /*高字节为主版本号，低字节为副版本*/
    Uint16 wVersion;
    /*消息类型*/
    Uint16 wMsgType;
    /*序列号*/
    Uint32 dwSerialId;
    /*设备编号，以'\0'结束*/
    Int8 achPuId[MAX_SIZE_PUID];
    /*消息体长度*/
    Uint32 dwMsgLen;
}PACKED TIvhsMsgHead;

typedef struct
{
    /*连路句柄*/
    KOSA_HANDLE dwHandle;
    /*消息类型*/
    Uint32 wMsgType;
    /*源序列号 响应消息时与请求的相同，请求可以不填*/
    Uint32 dwSrcSeqId;
    /*消息体*/
    Char *szMsgBody;
    /*消息长度*/
    Uint32 dwMsgLen;
    /*源IP地址, 响应消息时协议自动填充*/
    Uint32 dwSrcIP;
    /*安全上下文, 协议自动填充*/
    Uint32 dwContex;
    /*不必添，协议自动填充*/
    Char achTime[64];
}PACKED TItsMessage;


typedef struct
{
    KOSA_HANDLE dwHandle;
    char   szPuId[MAX_SIZE_DEV_ID];
    Uint32 dwIp;
    Uint16 wPort;
    Int16  wProtclType;
    Uint32 dwDataTypes; //上传数据类型(见EITS_DATA_TYPE)
    char   szUsername[MAX_SIZE_USRNAME];
    char   szPassword[MAX_SIZE_PASSWORD];
    char   szSavePath[MAX_SIZE_PATH_LEN];
    char   szRegulation[STRING_MAX_LENGTH_128];
}PACKED TIvhsPlatInfo;

typedef struct
{
    /*必要 设备编号*/
    Char achDevId[MAX_SIZE_DEV_ID];
    /*必要 0-前端设备,1-接入服务器，2-客户端界面 */
    Int32 nDevType;
    /*可选 用户名*/
    Char achUsername[MAX_SIZE_USRNAME];
    /*可选 密码*/
    Char achPassword[MAX_SIZE_PASSWORD];
    /*必要 是否支持图片上字符叠加，0：不支持，1：支持*/
    Uint32 dwImgoverlay;
    /*必要 产品型号*/
    Char achProductType[MAX_SIZE_PRODUCT];
    /*必要 厂家*/
    Char achVender[MAX_SIZE_VENDER];
}PACKED TIvhsDevInfo;

/*
typedef struct
{
    Int32 bUsed;
    ELinkState eState;
    TIvhsPlatInfo *ptPlatInfo;
    KOSA_SEMHANDLE hIvhsLinkLock;
    //KOSA_MutexHndl hAbppLinkLock;
    //KOSA_TskObj hFwdTaskObj;
}PACKED TIvhsLinkInfo;
*/

typedef struct
{
    Uint32 dwFunctionSupport;   //见EFUNCTION_SUPPORT枚举
    Uint8 byMaxClientSize;
}PACKED TIvhsInitParam;

typedef struct
{
    Uint32 dwIp;
    Uint16 wPort;
    ELinkState eState;
    Uint16 wLinkType;      //对端类别 见EIVHSLINKTYPE
}PACKED TIvhsNotifyInfo;

typedef struct
{
	Uint32 dwIp;
	Uint16 wPort;
	Uint8 bySendState;   // 0成功 1失败
}PACKED TIvhsSendState;

typedef struct
{
	Uint32 dwSeqNumber;
	Uint8 bySendType;    // 0实时 1历史
	TVechileData *ptData;
    Uint32 dwPlatNum;
	TIvhsSendState tSendState[MAX_NUM_SERVER];
}PACKED TIvhsSendPicCb;

/*----------------------------------------------------------
 *业务结构体（ITS200）
 *----------------------------------------------------------*/
typedef struct
{
    /*必要 设备编号*/
    Char achDevId[MAX_SIZE_DEV_ID];
    /*必要 0-前端设备,1-接入服务器*/
    Int32 nDevType;
    /*可选 用户名*/
    Char achUsername[MAX_SIZE_USRNAME];
    /*可选 密码*/
    Char achPassword[MAX_SIZE_PASSWORD];
    /*可选 本地ip地址，如192.168.1.111*/
    Char achIpAddr[MAX_SIZE_STR_IPADDR];
    /*可选 图片端口*/
    Uint16 wImagePort;
    /*必要 视频端口*/
    Uint16 wvideoPort;
    /*必要 是否支持图片上字符叠加，0：不支持，1：支持*/
    Uint32 dwImgoverlay;
    /*必要 产品型号*/
    Char achProductType[MAX_SIZE_PRODUCT];
    /*必要 厂家*/
    Char achVender[MAX_SIZE_VENDER];
}PACKED TLoginReqInfo;

typedef struct
{
    /*必要 设备编号*/
    Char achDevId[MAX_SIZE_DEV_ID];
    /*必要 0-前端设备,1-接入服务器*/
    Int32 nDevType;
    /*必要 0成功；非0-具体错误码。*/
    Int32 nRetCode;
    /*可选 0-MJPEG；1-H.264*/
    Int32 nStreamType;
}PACKED TLoginRspInfo;

typedef struct
{
    char    szUserName[MAX_SIZE_USRNAME];
    char    szPassWd[MAX_SIZE_PASSWORD];
    char*   szRemotePath[MAX_SIZE_PATH_LEN];
    //char* pFileRule;
}PACKED TITS_FtpCfg;

//ftp图片转发
typedef struct
{
    Bool    bIsData;
    //TImageData中的buf实时传imagebuffer，历史传本地路径
    TVechileData    tVechileData;
    TITS_FtpCfg     tFtpCfg;
}PACKED TITS_FtpInfo;

/*布局方案*/
typedef struct
{
    Bool    bEnable;                        // 是否启用
    Uint16  wType;                          // 布局类型(客户端用) 定义在duilib中EMLABLELAYOUTTYPE
    Uint16  wNumber;                        // 布局数目
    Uint16  wRow;                           // 行数
    Uint16  wCol;                           // 列数
    Uint16  wRowStart[MAX_MERGE_PIC_NUM];   // 行起始位置
    Uint16  wRowWidth[MAX_MERGE_PIC_NUM];   // 行宽度
    Uint16  wColStart[MAX_MERGE_PIC_NUM];   // 列起始位置
    Uint16  wColWidth[MAX_MERGE_PIC_NUM];   // 列宽度
}PACKED TLayoutInfo;

typedef struct
{
    Uint8   byImageSrc;     //图片来源  参见ENUM_ITS_IMAGE_SRC
    Bool    bSaveOriginal;  //是否保留原图
}PACKED TImageSrc;

/*合并方案*/
typedef struct
{
    Bool    bEnable;           	// 是否启用
    Uint8   byLayoutSize;       // 布局的类型数目
    Uint8   abyLayoutIndex[MAX_MERGE_LAYOUT_NUM];       // 布局的类型可以包含4种相同或不同类型
    TImageSrc   tImageSrc[MAX_MERGE_LAYOUT_NUM][MAX_MERGE_PIC_NUM]; // 不同布局类型的图片来源
}PACKED TMergeInfo;

/*图片合成配置*/
typedef struct
{
    Uint32  dwIP;   //
    Uint32  dwSizeType;         // 图片大小 参见ENUM_ITS_IMAGE_SIZE
    Uint16  wCompress;          //压缩比 eg: 75-->75%
    // 布局方案
    TLayoutInfo	atLayoutInfo[MAX_MERGE_PIC_NUM];
    //合并方案
    TMergeInfo	atMergeSrcInfo[MAX_MERGE_PIC_NUM];
}PACKED TImageMergeCfg;

/*获取图片合成响应*/
typedef struct
{
    TImageMergeCfg tImageMerge;
    Int32 nRetCode;
}PACKED TImageMergeRsp;

/*图片存储*/
typedef struct
{
    Int16 wStoreType;   //存储类型 ，1:实时存储 2:掉线存储
}PACKED TImageStore;

typedef struct
{
    TImageStore tStore;
    Int32 nRetCode;
}PACKED TImageStoreRsp;


// OSD配置信息;
typedef struct
{
    Bool    bEnable;            // 是否启用OSD叠加
    Int32   dwType;             // 类型：0: all/卡口_原图,1:卡口_合成21, 2:卡口_合成41, 3:卡口_红灯 参见ENUM_ITS_OSD_IMAGE_TYPE
    Int32   dwRowNumber;        // 1~16 行数； 大于16 指定像素高度的；
    Int32   dwBKColor;          // 背景色, 特殊颜色表示透明  参见ENUM_ITS_OSD_BKCLR
    Bool    bAutoLinefeed;      // 是否自动换行
    Int32   dwAlignType;        // 对其方式   参见ENUM_ITS_OSD_ALIGN
    Int32   dwPosType;          // 位置类型   参见ENUM_ITS_OSD_POS
    char    szWildcard[5];        // 通配符 #/*/-/@ 选择；
    char    szDateTimeFormat[33]; // YY-MM-DD hh:mm:ss; YYYY-MM-DD hh:mm:ss;MM-DD-YY hh:mm:ss;
    char    szOsdFormat[MAX_OSD_OVERLAY_COUNT][256];    // OSD显示格式;
}PACKED TOsdCfgInfo;

// OSD字体;
typedef struct
{
    Int32   dwFontSize;    //参见 ENUM_ITS_OSD_FONTSIZE
    Int32   dwFontColor;   // 增加黑白色 参见ENUM_ITS_OSD_FONTCLR
}PACKED TOsdFont;

// OSD字体设置;
typedef struct
{
    Int32       dwType;             // 类型：all/卡口_原图,卡口_合成21,卡口_合成41, 卡口_红灯
    char        szFontName[17];     // 字体名称；
    TOsdFont    atFont[MAX_OSD_TYPE_COUNT];
}PACKED TOsdFontCfg;

/*获取osd叠加响应*/
typedef struct
{
    TOsdCfgInfo tOsdCfg;
    Int32 nRetCode;
}PACKED TOsdCfgRsp;

/*获取osd字体响应*/
typedef struct
{
    TOsdFontCfg tOsdFontCfg;
    Int32 nRetCode;
}PACKED TOsdFontCfgRsp;

/*中心服务器*/
typedef struct
{
    Bool    bEnable;
    char    szPuId[MAX_SIZE_DEV_ID];
    Uint32  dwIpAddr;
    Uint16  wPort;
    Uint8   byServerType;
    Uint32  dwDataTypes;    //数据类型复选 每一bit代表一种类型,
    Uint32  dwUploadType;   // HISDATA_TRANS_MODE  0 空闲上传  1 请求上传  2 不上传
    Char    szUsername[MAX_SIZE_USRNAME];
    Char    szPassword[MAX_SIZE_PASSWORD];
    char    szSavePath[MAX_SIZE_PATH_LEN];
    char    szRegulation[STRING_MAX_LENGTH_128];   //命名规则
}PACKED TITS_CenterServer;

/*获取中心服务器配置响应*/
typedef struct
{
    Int32       nRetCode;
    Uint16      wServerSize;                    //中心服务器个数
    TITS_CenterServer tServer[MAX_NUM_SERVER];  //中心服务器
}PACKED TITS_GetServerRsp;

/*设置中心服务器配置请求*/
typedef struct
{
    Uint16      wServerSize;                        //中心服务器个数
    TITS_CenterServer tServer[MAX_NUM_SERVER];      //中心服务器
}PACKED TITS_SetServerReq;

/*设备参数*/
typedef struct
{
    Uint32  dwIpAddr;
    Uint16  wPort;
    Uint8   byDevType;
}PACKED TITS_Device;

//添加、删除设备信息
typedef struct
{
    Uint8   byDevSize;
    TITS_Device tDevice[MAX_NUM_DEVICE];
}PACKED TITS_DevList;

//获取设备列表响应
typedef struct
{
    Int32 nRetCode;
    Uint8 byDevSize;
    TITS_Device tDevice[MAX_NUM_DEVICE];
}PACKED TITS_GetDevListRsp;

/*断面过滤*/
typedef struct
{
    Bool    bUsed;
    Int16   nFilterSec;
    Uint16  wFilterLength;
}PACKED TITS_Filter;

/*辅助摄像机参数*/
typedef struct
{
    Bool    bEnable;
    Uint32  dwIpAddr;
    Uint16  wPort;
    Uint16  wSectionNumber;
    Uint8   byMatchMode;
    TITS_Filter tMatchFilter;
    Uint16  wSpeedDistance;     //测速距离
}PACKED TITS_AuxCamera;

/*辅助摄像机信息*/
typedef struct
{
    Uint16      wAuxCameraSize;     //辅助摄像机个数
    TITS_AuxCamera  tAuxCamera[MAX_NUM_AUX_CAMERA]; //辅助摄像机
}PACKED TITS_AuxCameraInfo;

/*录像截取*/
typedef struct
{
    Char    szVideoFormat[MAX_SIZE_LEN8];
    Uint16  wSectionType;       //录像截取类型,见VIDEO_CAPTURE_TYPE枚举
    Bool    bUsed;
    Uint16  wVideoPreSec;       //预录时间（s）
    Uint16  wVideoDelaySec;     //延后时间（s）
}PACKED TITS_VideoSectCfg;

/*设备参数*/
typedef struct
{
    Bool    bIsAux;             //是否是辅助相机
    char    szDevName[MAX_SIZE_USRNAME];
    char    szDevNo[MAX_SIZE_DEV_ID];
    char    szLaneNumber[MAX_SIZE_LEN64];
    Uint16  wSectionNumber;

    TITS_Filter tSectionFilter;     //断面过滤
}PACKED TITS_DeviceParam;

/*获取设备详细信息请求、获取辅助相机信息请求*/
typedef struct
{
    Uint32  dwDevId;            //当前指定为设备IP
}PACKED TITS_GetDevInfoReq;

/*获取设备详细信息响应*/
typedef struct
{
    Int32 nRetCode;
    TITS_DeviceParam tDevParam;
}PACKED TITS_GetDevInfoRsp;

typedef struct
{
    Uint32             dwDevId;      //当前指定为设备IP
    TITS_DeviceParam    tDevParam;
}PACKED TITS_SetDevInfoReq;

typedef struct
{
    Int32              nRetCode;
    TITS_AuxCameraInfo  tCameraInfo;
}PACKED TITS_GetAuxCameraRsp;

typedef struct
{
    Uint32             dwDevId;    //当前指定为设备IP
    TITS_AuxCameraInfo  tCameraInfo;
}PACKED TITS_SetAuxCameraReq;

typedef struct
{
    Uint32  dwVidSectType;     //录像截取类型
}PACKED TITS_GetVideoCfgReq;

typedef struct
{
    Int32           nRetCode;
    TITS_VideoSectCfg   tVideoCfg;
}PACKED TITS_GetVideoCfgRsp;

/**************************实时监控************************/
typedef struct
{
    //设备编号字符串，以“,”分隔每一个设备编号
    char    szDeviceNoStr[MAX_SIZE_DEV_ID*MAX_NUM_DEVICE];
}PACKED TITS_RtVehicleReq;


typedef struct
{
    TVechileData tVehicleData;
    Int32 nRetCode;
}PACKED TITS_RtVehicleRsp;


 /**************************数据查询************************/
// typedef struct
// {
//  Uint32 dwQueryType;     //查询条件
//  Uint32 dwLimit;         //每页显示的条数
//  Uint32 dwOffset;        //显示第几页
//
//  Bool IsHasAccess(Uint32 dwType)
//  {
//      return ((dwQueryType & dwType) != 0);
//  }
// }PACKED TItsQueryBase;
//
// //数据查询
// struct TDataQuery : public TItsQueryBase
// {
// public:
//  Int64   qwStartTime;                    //开始时间
//  Int64   qwStopTime;                     //结束时间
//  Uint8   byDataType;                     //数据类别: 1全部卡口; 2:全部违法; 3:...
//  Int8    szDevId[MAX_SIZE_DEV_ID];       //相机编号: 目前暂定为 设备编号指代相机编号
//  Int8    szCarPlate[MAX_SIZE_PLATE_NUM]; //车牌号码
//  Uint8   byUploadState;                  //上传状态，1:已上传，2：未上传
//  Uint16  wMinSpeed;                      //最小速度
//  Uint16  wMaxSpeed;                      //最大速度
//  Uint8   byAscType;                      //排序类型	1:时间升序 2:时间降序 3:编号升序 4:编号降序
// }PACKED;

typedef struct
{
    char    szDevId[MAX_SIZE_DEV_ID];
    Int64   qwThroughTime;
    char    szCarPlate[MAX_SIZE_PLATE_NUM];
    Uint32  dwPlateColor;
    char    szChannelNo[MAX_SIZE_LEN64];
    char    szLaneNo[MAX_SIZE_LEN64];
    Int16   wEventType;
    Uint32  dwCarType;
    Uint16  wCarSpeed;
    Uint8   byUploadState;
}PACKED TITS_QueryResult;

typedef struct
{
    Int32 nTotal;
    Int16 nResultSize;
    TITS_QueryResult tResult[MAX_NUM_PER_PAGE];
    Int32 nRetCode;
}PACKED TITS_QueryResultRsp;

/*----------------------------------------------------------
 *业务结构体（IPC V7）
 *----------------------------------------------------------*/
/*点*/
typedef struct
{
    /*车道查询使能*/
	Uint32 dwLaneNumEnable;
    /*车道号*/
	Uint32 dwLaneNum;
    /*开始时间使能*/
	Uint32 dwStartTimeEnable;
    /*开始时间*/
	Uint32 dwStartTime;
    /*结束时间使能*/
	Uint32 dwEndTimeEnable;
    /*结束时间*/
	Uint32 dwEndTime;
    /*车牌号码使能*/
	Uint32 dwFrontPlateNumEnable;
    /*是否模糊匹配*/
	Uint32 dwVaguePlateMatch;
    /*车牌号码*/
	Char achFrontPlateNum[MAX_SIZE_PLATE_NUM];
    /*违法类型使能*/
	Uint32 dwIllegalTypeEnable;
    /*违法类型*/
	Char achIllegalType[MAX_SIZE_ILLEGAL_TYPE];
    /*更新状态使能*/
	Uint32 dwUpdateFlagEnable;
    /*更新状态*/
	Uint32 dwUpdateFlag;
    /*每次查询起始位置，起始为0 limit dwBeignNum,dwQueryNum*/
	Uint32 dwQueryBeginNum;
	/*每次希望查询的数目*/
	Uint32 dwQueryNum;
} PACKED TIPC_PicQuery;

/*查询返回结构体*/
/*记录结构体*/
typedef struct
{
    /*记录编号*/
    Uint32 dwRecordID;
    /*车道编号*/
	Uint32 dwLaneNum;
	/*过车时间 time_t*/
	Uint32 dwThroughTime;
    /*车牌号码*/
    Char achFrontPlateNum[MAX_SIZE_PLATE_NUM];
    /*车牌颜色*/
	Uint32 dwPlateColor;
    /*违法类型*/
	Char achIllegalType[MAX_SIZE_ILLEGAL_TYPE];
    /*车速*/
	Uint32 dwSpeed;
	/*过车图片数目*/ 
	Uint32 dwPicNum; 
	/*关联视频数量*/ 
	Uint32 dwVioNum; 
	/*过车图片路径，根据序号区分多张*/ 
	Char achPicPath[MAX_LEN_VIDEOR_URL]; 
	/*关联视频路径，根据序号区分多条*/ 
	Char achVioPath[MAX_LEN_VIDEOR_URL]; 
    /*更新标志，取第4位，0x0011 表示第一个第二个平台已经上传*/
	Uint32 dwUploadState;
}PACKED TIPC_PicFileInfo;

typedef struct
{
    /*查询记录总数*/
    Uint32 dwMaxNum;
    /*当前查询起始位置,从0开始*/
    Uint32 dwBeignNum;
    /*实际查询到的文件数量，不要大于MAX_NVR_PIC_QUERY_NUM*/
    Uint32 dwNum;
    /*记录列表*/
    TIPC_PicFileInfo aPicFileInfo[MAX_NUM_PER_PAGE];
}PACKED TIPC_PicQueryResult;

/*----------------------------------------------------------
 *业务结构体（IPC V7）
 *----------------------------------------------------------*/
/*点*/
typedef struct
{
    Int32 X;
    Int32 Y;
}PACKED TPoint_S;

/*线*/
typedef struct
{
    Int32 X1;
    Int32 Y1;
    Int32 X2;
    Int32 Y2;
}PACKED TLine_S;

/*规则线*/
typedef struct
{
    /*使能*/
    Bool bEnable;
    /*规则线编号*/
    Uint32 LineLabel;
    /*直线*/
    TLine_S tLine;
}PACKED TRuleLine_S;

/*车道线*/
typedef struct
{
    /*车道左边线*/
    TRuleLine_S tRdLineL;
    /*车道右边线*/
    TRuleLine_S tRdLineR;
}PACKED TRoadLine_S;

/*矩形*/
typedef struct
{
    Int32 X;
    Int32 Y;
    Int32 W;
    Int32 H;
}PACKED TRECTANGLE_S;

/*梯形*/
typedef struct
{
    Int32 X1;
    Int32 Y1;
    Uint32 W1;
    Int32 X2;
    Int32 Y2;
    Uint32 W2;
}PACKED TECHELON_S;

/*违停区域*/
typedef struct
{
    Bool        bEnable;
    Uint32      dwRoiLabel;
    Uint16      wPointNum;
    TPoint_S    tPoint[ROI_AREA_POINT_MAX];
}PACKED TRoiArea_S;

/*红灯增强*/
typedef struct
{
    Bool   bEnable;
    /*红灯区域*/
    Uint32 dwStartX;
    Uint32 dwStartY;
    Uint32 dwWidth;
    Uint32 dwHeight;
    /*最小像素*/
    Uint32 dwPixMin;
    /*白天增强等级*/
    Uint32 dwLevelDay;
    /*夜间增强等级*/
    Uint32 dwLevelNight;
    /*颜色*/
    Uint8 byR;
    Uint8 byG;
    Uint8 byB;
    /*参考相位: REDLAMP_PHASETURNRIGHT.......*/
    Uint32 dwRefPhase;
}PACKED TIPC_RedSigEnhance;

/*物理车道*/
typedef struct
{
    /*车道编号*/
    Uint32 dwLaneLabel;
    /*行驶方向: 见DRIVE_DIR_TYPE枚举*/
    Uint32 dwDriveDir;
    /*车道方向: 由南向北、由北向南................*/
    Char   szLaneDir[MAX_SIZE_LEN16+1];
    /*车道属性: bit3--左待转，bit2--左转，bit1--直行，bit0--右转*/
    Uint32 dwLaneAttribute;
    /*白天补光方式*/
    Uint32 dwDaySnapLight;
    /*夜晚补光方式*/
    Uint32 dwNightSnapLight;
    /*关联的爆闪灯: bit0--F1、bit1--F2、bit2--F3、bit3--F4*/
    Uint32 dwSnapF;
    /*关联的LED灯: bit0--内置LED、bit1--LED1、bit2--LED2*/
    Uint32 dwSnapLED;
    /*检测方式: 视频检测、线圈检测、雷达检测、视频+雷达测速*/
    Uint32 dwDetType;

    /*大车限速*/
    Uint32 dwTruckLmSpd;
    /*大车起拍速度*/
    Uint32 dwTruckSnapSpd;
    /*小车限速*/
    Uint32 dwCarLmSpd;
    /*小车起拍速度*/
    Uint32 dwCarSnapSpd;
    /*抓拍类型: bit0--卡口、超速、...见LOGIC_ROAD_TYPE_KK相关定义*/
    Uint32 dwSnapType;
}PACKED TIPC_PhysicLane;

typedef struct
{
    /*设备名称*/
    Char    szDevName[MAX_SIZE_LEN64];
    /*设备编号*/
    Char    szDevLabel[MAX_SIZE_LEN32];
	/*设备型号*/
    Char    szDevType[MAX_SIZE_LEN32];
    /*软件版本号*/
    Char    szSoftWareVer[MAX_SIZE_LEN64];
	/*基础软件版本号*/
	Char    szSoftWareBaseVer[MAX_SIZE_LEN64];
	/*场景名称*/
    Char    szSceneName[MAX_SCENE_NUM][MAX_SIZE_LEN64];
	/*厂家*/
	Uint32	dwVender;
	/*序列号*/
	Char	szSn[MAX_SIZE_LEN16+1];
	/*成员*/
	Uint8	szLicense[MAX_SIZE_LEN16+1];
}PACKED TIPC_DevInfo;

/*场景配置*/
typedef struct
{
    /*预置位id*/
    Uint16  wPrePosId;
    /*道路编码*/
    Char    szRoadCode[MAX_SIZE_LEN32];
    /*车道总数*/
    Uint32  dwLaneNum;
    /*车道宽*/
    Uint32  dwLaneWidth;
    TIPC_PhysicLane tPhyLane[ROAD_NUM_MAX];
    /*车道左边线*/
    TRuleLine_S tRdLineL;
    /*车道右边线*/
    TRuleLine_S tRdLineR;
    /*车道12分割线*/
    TRuleLine_S tRdLine12;
    /*车道23分割线*/
    TRuleLine_S tRdLine23;
    /*车道34分割线*/
    TRuleLine_S tRdLine34;
    /*检测线*/
    TRuleLine_S tScanLine;
    /*左转判定线*/
    TRuleLine_S tTurnLeftLine;
    /*直行判定线*/
    TRuleLine_S tStraightLine;
    /*右转判定线*/
    TRuleLine_S tTurnRightLine;
    /*停车线*/
    TRuleLine_S tStopLine[ROAD_NUM_MAX];
    /*跟踪区域*/
    //TRECTANGLE_S  tTrackArea;
    TRoiArea_S  tTrackArea;
    /*其他规则线*/
    TRuleLine_S   tRuleLine[USER_RULE_LINE_NUM_MAX];
    /*违停检测区域*/
    Uint32        dwRoiAreaNum;
    TRoiArea_S    tRoiArea[VEH_DET_ROI_NUM_MAX];
}PACKED TIPC_SceneConfig;

/*雷达检测器*/
typedef struct
{
    /*编号*/
    Uint32  dwLoopLabel;
    /*使能*/
    Bool    bEnable;
    /*对应串口通道号*/
    Uint32  dwSerialChnId;
    /*雷达类型: 窄波雷达、2D雷达*/
    Uint32  dwRadarType;
    /*雷达模式: 连续模式、触发模式*/
    Uint32  dwRadarMode;
    /*雷达与相机的水平距离: 雷达在左为负，雷达在右为正*/
    float   fH_distance;
    /*速度修正系数*/
    float   fCorrectRate;
    /*对应物理车道号*/
    Uint32  dwPhyLaneNo;
}PACKED TIPC_RadarLoopCfg;

/*线圈检测器*/
typedef struct
{
    /*编号*/
    Uint32  dwCoilLabel;
    /*使能*/
    Bool    bEnable;
    /*对应串口通道号*/
    Uint32  dwSerialChnId;
    /*线圈通道号*/
    Uint32  dwCoilChnId;
    /*方向*/
    Uint32  dwDirection;
    /*线圈间距: 与前一个线圈的距离*/
    Uint32  dwV_distance;
    /*线圈宽度*/
    Uint32  dwCoilWidth;
    /*速度修正系数*/
    float   fCorrectRate;
    /*关联IO输出*/
    Uint32  dwIOLabel;
    /*对应物理车道号*/
    Uint32  dwPhyLaneNo;
    /*线圈类别*/
    Uint32  dwCoilType;
}PACKED TIPC_CoilLoopCfg;

/*Video Detector.....*/
typedef struct
{
    /*编号*/
    Uint32  dwLoopLabel;
    /*使能*/
    Bool    bEnable;
    /*检测器区域*/
    Uint32  dwStartX;
    Uint32  dwStartY;
    Uint32  dwWidth;
    Uint32  dwHeight;
    /*检测方式*/
    Uint32  dwMethod;
    /*方向*/
    Uint32  dwDirection;
    /*目标的宽度、高度
      运动检测器代表目标宽高；
      号牌检测代表号牌宽高；
      头灯检测代表头灯最小尺寸最大尺寸；
      尾灯检测代表尾灯尺寸和尾灯间距；
      红绿灯检测代表最小像素和最大抖动范围;*/
    Uint32  dwTargetWidth;
    Uint32  dwTargetHeight;
    /*号牌角度(1: 龙门架(C杆); 2: A杆，摄像左牌照右; 3: A杆，摄像右牌照左; 4: 角度方向不定)*/
    Uint16  wPlateAngle;
    /*虚拟线圈存在时间，单位为秒，和背景建模速率有关???*/
    Uint32  dwPresentTime;
    /*虚拟线圈相对的灵敏度范围：一般取值20左右*/
    Uint32  dwSensitive;
    /*代表连续多少次检测到以后才正式判断为有或没有*/
    Uint32  dwFilterOnTimes;
    Uint32  dwFilterOffTimes;

    //cst todo: 协议中不包含业务内部参数
}PACKED TIPC_VidLoopCfg;

/*红灯信号检测器*/
typedef struct
{
    /*检测器编号*/
    Uint32  dwLoopLabel;
    /*使能*/
    Bool    bEnable;
    /*极性: 0-负极性，1-正极性*/
    Bool    bPolarity;
    /*串口通道号*/
    Uint32  dwSerialChnId;
    /*通道号*/
    Uint32  dwChnId;
    /*检测相位: REDLAMP_PHASETURNRIGHT.......*/
    Uint32  dwDetPhase;
}PACKED TIPC_RedLampSigLoop;

/*识别区域*/
typedef struct
{
    /*梯形区域*/
    TECHELON_S tArea;
    /*号牌宽度*/
    Uint32  dwPlateWidth;
    /*号牌角度*/
    Uint32  dwPlateAngle;
    /*号牌视角:  1 为抓拍车辆头部，2 为抓拍车辆尾部;*/
    Uint32  dwShootRear;
}PACKED TIPC_RecogArea;

/*特征识别参数*/
typedef struct
{
    /*快速识别*/
    Bool    bFastRecog;
    /*用于控制识别时间(ms)，防止超时; 0 为不控制*/
    Uint32  dwTimeControl;
    /*本地牌照占80%以上*/
    Bool    bLocalPlate;
    /*默认省份*/
    Char    szDefaultProvince[3];
    /*默认地区代码*/
    Char    chDefaultArea;
    /*开启阴影消除*/
    Bool    bShadowRemove;
    /*号牌识别*/
    Bool    bLprRecog;
    /*摩托车牌识别*/
    Bool    bRecogMT;
    /*车身颜色识别*/
    Bool    bVehColorRecog;
    /*车标识别*/
    Bool    bVehLogoRecog;
    /*司乘人员人脸定位*/
    Bool    bFaceLocate;
    /*驾驶员开车打电话*/
    Bool    bPhoneCheck;
    /*驾驶员未寄安全带*/
    Bool    bSeatbeltCheck;
    /*副驾未寄安全带*/
    Bool    bCopilotCheck;
    /*遮阳板遮挡人脸*/
    Bool    bSunvisorCheck;
}TIPC_RecogParam;

/*定时抓拍*/
typedef struct
{
    /*时间段个数: 最多4个时间段*/
    Uint32 num;
	/*开始时间: 精确到分钟*/
    Uint32 startTimeHour[MAX_NUM_DECT_TIME];
    Uint32 startTimeMin[MAX_NUM_DECT_TIME];
    /*结束时间: 精确到分钟*/
    Uint32 stopTimeHour[MAX_NUM_DECT_TIME];
    Uint32 stopTimeMin[MAX_NUM_DECT_TIME];
}TIPC_SnapTiming;

/*逻辑车道*/
typedef struct
{
    /*逻辑车道编号*/
    Uint32 dwLgRdId;
    /*使能*/
    Bool   bEnable;
    /*逻辑车道类型*/
    Uint32 dwLgRdType;
    /*车道ID*/
    Uint32 dwLaneId;
    /*白天补光方式*/
    Uint32 dwDaySnapLight;
    /*夜间补光方式*/
    Uint32 dwNightSnapLight;
    /*人像曝光模式*/
    Bool   bFaceMode;
    /*识别区域*/
    Uint32 dwRecAreaID;
    /*检测器编号: 触发检测器(含规则线)1、2、3;匹配确认检测器4*/
    Uint32 dwDectorLabel[MAX_DECT_LABEL_NUM];
    /*跟踪置信度*/
    Uint32 dwConfidenceLevel;
    /*匹配字符数*/
    Uint32 dwMatchCnt;
    /*号牌过滤: bit0--黄牌、bit1--白牌、bit2--黑牌*/
    Uint32 dwFilterLpr;
	/*方向过滤*/
	Uint32 dwFilterDir;
    /*最小抓拍间隔*/
    Uint32 dwMinInterval;
    /*关联红灯相位: REDLAMP_PHASE_TURNRIGHT*/
    Uint32 dwRedLightPhase;
    /*最大红灯时间*/
    Uint32 dwMaxRedLight;
    /*最小红灯时间*/
    Uint32 dwMinRedLight;
    /*红灯极性: 0-负极性，1-正极性*/
    Bool bPolarity;
    /*抓拍张数*/
    Uint32 dwNeedCnts;
    /*抓拍控制模式: 按时间、按距离*/
    Uint32 dwSnapCtrlMode;
    union
    {
        /*1|2间隔时间*/
        Uint32 dwTime;
        /*1|2间隔距离*/
        Uint32 dwDistance;
    }ShareData1;
    union
    {
        /*2|3间隔时间*/
        Uint32 dwTime;
        /*2|3间隔距离*/
        Uint32 dwDistance;
    }ShareData2;
    /*灵敏度*/
    Uint32  dwSensitivity;
    /*抓拍子图*/
    Bool    bLprSubPic;
    Uint32  dwLprPicW;
    Uint32  dwLprPicH;
    Bool    bVehSubPic;
    Uint32  dwVehPicW;
    Uint32  dwVehPicH;
    Bool    bFaceSubPic;
    Uint32  dwFacePicW;
    Uint32  dwFacePicH;
    /*8位数字: 违章代码*/
    Uint32  dwWzCode;
    /**********************限速***********************/
    /*大车限速*/
    Uint32  dwTruckLmSpd;
    /*大车起拍速度*/
    Uint32  dwTruckSnapSpd;
    /*小车限速*/
    Uint32  dwCarLmSpd;
    /*小车起拍速度*/
    Uint32  dwCarSnapSpd;
    /*启用视频测速校验*/
    Bool    bUseVidSpdChk;
    /*允许偏差*/
    Uint32  dwSpdOffset;
    /********************定时设置**********************/

	TIPC_SnapTiming tSnapTiming[MAX_NUM_SNAPTIMING];

	 /*定时空拍: 单位小时*/
    Uint32 dwTimingSnap;

}PACKED TIPC_LogicLane;

/*场景最大分辨率*/
typedef struct
{
    Uint32  dwMaxWidth;
    Uint32  dwMaxHeight;
}PACKED TIPC_SceneMaxParam;

/*获取场景配置响应*/
typedef struct
{
    Int32           nRetCode;
    Uint32          dwSceneSize;
    /*场景id*/
    Uint32          dwSceneId;
    TIPC_SceneMaxParam  tMaxParam;
    TIPC_SceneConfig    tSceneCfg;
}PACKED TIPC_GetSceneCfgRsp;

typedef struct
{
    /*场景id*/ 
    Uint32          dwSceneId;
    TIPC_SceneConfig    tSceneCfg;
}PACKED TIPC_SetSceneCfgReq;

/*获取ipc设备信息响应*/
typedef struct
{
    TIPC_DevInfo tDev;
    Int32       nRetCode;
}PACKED TIPC_GetDevRsp;


/*PTZ控制命令*/
typedef struct
{
    /*操作命令类别 见PTZ_EVENT_TYPE枚举*/
    Uint16  wEventType;
    Uint16  wP1;
    Uint16  wP2;
    Uint16  wP3;
    Uint16  wP4;
}PACKED TIPC_PTZCtrl;

/*视频测速*/
typedef struct
{
    /*使能*/
    Bool   bEnable;
    /*摄像机的安装高度*/
    float  fCameraHeight;
    /*图像上边沿距离立杆的实际距离*/
    float fTopLineDist;
    /*图像中间线距离立杆的实际距离*/
    float fMidLineDist;
    /*图像下边沿距离立杆的实际距离*/
    float fBottomLineDist;
    /*号牌下边沿距离地面的高度*/
    float  fPlateB2G;
    /*多目标雷达距离摄像机*/
    float  fRadarXShift;
    /*速度修正系数*/
    float  fCorrectRate;
}PACKED TIPC_VidSpdDet;

/*图像控制 范围0~100*/
typedef struct
{
    /*亮度*/
    Uint8 byBright;
    /*对比度*/
    Uint8 byContrast;
    /*GAMA*/
    Uint8 byGama;
    /*饱和度*/
    Uint8 bySaturation;
    /*锐度*/
    Uint8 bySharpness;
    /*2D降噪*/
    Uint8 bydeNoise2D;
}PACKED TIPC_ImageCtrlParam;

/*曝光*/
typedef struct
{
    /*支持的模式列表KISP_2A_AE_INVALID*/
    Uint32 dwSptAEModeList;
    /*曝光模式: KISP_2A_AE_INVALID*/
    Uint32 dwAEmode;
    /*手动曝光快门值: 0~40*1000us*/
    Uint32 dwManuShutter;
    /*手动曝光增益: 0~100db*/
    Uint32 dwManuAgain;
    /*自动曝光快门上限: 0~40*1000us*/
    Uint32 dwMaxShuter;
    /*自动曝光增益上限: 0~100db*/
    Uint32 dwMaxAgain;
    /*自动曝光: 默认目标亮度: 0~100*/
    Uint32 dwTargetY;
    /*使能号牌测光: 0--不使能，1--使能*/
    Uint32 dwLPRAEenable;
    /*期望号牌亮度:0~100*/
    Uint32 dwEptLPRBright;
    /*目标亮度范围: 0~100*/
    Uint32 dwMinTargetY;
    Uint32 dwMaxTargetY;
}PACKED TIPC_ExposureParam;

/*白平衡*/
typedef struct
{
    /*支持的模式列表KISP_2A_AWB_INVALID*/
    Uint32 dwSptAWBModeList;
    /*白平衡模式:E_KISP_AWB_MODE*/
    Uint32 dwAWBMode;
    /*期望R\G\B比例: 0~100*/
    Uint32 dwExpRgain;
    Uint32 dwExpBgain;
    /*固定白平衡R\B: 0~100*/
    Uint32 dwWbRgain;
    Uint32 dwWbBgain;
}PACKED TIPC_WhiteBalParam;

/*抓拍相关*/
typedef struct
{
    /*抓拍快门上限*/
    Uint32 dwSnapMaxShuter;
    /*抓拍增益上限*/
    Uint32 dwSnapMaxAgain;
    /*爆闪灯发光时间*/
    Uint32 dwFLightTime;
    /*夜间爆闪灯发光方式: FLASH_LIGHT_NIGHT_MODE_1正常闪 、交替闪 */
    Uint32 dwFLightMode;
    /*LED灯发光时间*/
    Uint32 dwLedTime;
    /*灰度拉伸1、2、3*/
    Bool   bGStretch;
    Uint32 byGStretchCutOff;
    /*编码控制: 控制图片大小、固定压缩因子*/
    Uint32 dwEncCtrl;
    /*图片大小: 100~1024 KB*/
    Uint32 dwPicSize;
    /*压缩因子0~100*/
    Uint32 dwJpegQp;
}PACKED TIPC_SnapParam;

/*PL偏光镜控制*/
typedef struct
{
    /*PL控制方式: KISP_PL_AUTO相关,自动、常开、常关*/
    Uint32 dwPLCtrlMode;
    /*启用PL偏光镜ES值*/
    Uint32 dwPLenableEs;
    /*停用PL偏光镜ES值*/
    Uint32 dwPLdisableEs;
}PACKED TIPC_PLCtrlParam;

/*LED控制参数*/
typedef struct
{
    /*LED控制方式: KISP_LED_AUTO相关*/
    Uint32 dwLedCtrlMode;
    /*LED灯频率: 50\100*/
    Uint32 dwLedLightHz;
    /*LED灯亮度: 0~100*/
    Uint32 dwLedLightTime;
}PACKED TIPC_LedLightParam;

/*串口参数*/
typedef struct
{
    /*设备名*/
    Char szDevName[MAX_SERIAL_NAME_LEN];
    /*波特率*/
    Uint32 dwBaudRate;
    /*停止位*/
    Uint32 dwStopBit;
    /*数据位*/
    Uint32 dwDataBit;
    /*奇偶校验位*/
    Uint32 dwParity;
    /*串口协议类型ESerialProtocol*/
    Uint32 dwProtocolType;
}PACKED TIPC_SerialParam;

/*串口配置参数*/
typedef struct
{
    /*串口数量*/
    Uint32 dwSerialNum;
    /*串口参数*/
    TIPC_SerialParam atSerialParams[MAX_SERIAL_NUM];
}PACKED TIPC_SerialCfg;

/*图片调节*/
typedef struct
{
    /*图像控制 数组0白天 1夜晚*/
    TIPC_ImageCtrlParam atImageCtrl[2];
    /*曝光 同上*/
    TIPC_ExposureParam  atExposure[2];
    /*白平衡 同上*/
    TIPC_WhiteBalParam  atWhiteBal[2];
    /*抓拍相关*/
    TIPC_SnapParam      atSnap[2];
    /*日夜切换*/
    /*日夜切换ES值*/
    Uint32         dwDayNightSwitchEs;
    /*PL偏光镜控制*/
    TIPC_PLCtrlParam   tPLCtrl;
    /*LED灯控制 0:内置、1:外置1、2:外置2*/
    TIPC_LedLightParam atLedLight[3];
}PACKED TIPC_KdISPCfg;

/*相位表设置*/
typedef struct 
{
    /*等效快门*/
    Uint32 dwEsValue;
    /*相位*/
    Uint32 dwPhase;
}PACKED TIPC_PhaseTableCfg;

/*电源同步及相位*/
typedef struct
{
    /*使能电源同步*/
    Bool   bEnablePwrSync;
    /*使能自动相位同步 False:固定相位 True:自动同步相位表*/
    Bool   bAutoPhaseSync;
    /*固定相位值0~180*/
    Uint32 dwFixPhase;
    /*相位表*/
    TIPC_PhaseTableCfg atPhaseTable[MAX_PHASE_NUM];
}PACKED TIPC_PowerPhaseSync;

/*开启码流请求*/
typedef struct
{
    /*调试模式: 见E_STREAM_MODE*/
    Uint32 dwDebugMode;
    /*接收端ip*/
    Uint32 dwIp;
    /*监听port*/
    Uint16 wPort;
    char   szUsername[MAX_SIZE_USRNAME+1];
    char   szPassword[MAX_SIZE_PASSWORD+1];
    /*标识值:发起者填充*/
    Uint32 dwSSRc;
}PACKED TIPC_BitStreamInfo;

/*开启码流响应*/
typedef struct
{
    /*1:Error 其他:成功的句柄*/
    Uint32 dwRetCode;
    /*调试模式: 见E_STREAM_MODE*/
    Uint32 dwDebugMode;
    /*接收端ip*/
    Uint32 dwIp;
    /*监听port*/
    Uint16 wPort;
    /*标识值,响应者原值返回*/
    Uint32 dwSSRc;
}PACKED TIPC_BitStreamRsp;

/*关闭码流响应*/
typedef struct
{
    /*1:失败 0:成功*/
    Uint32 dwRetCode;
    /*请求句柄值原值返回*/
    Uint32 dwStreamHd;
}PACKED TIPC_StopBitStreamRsp;

typedef struct
{
    /*调试流数据类型E_STREAM_MODE*/
    Uint32 dwBufType;
    /*数据总长度*/
    Uint32 dwLen;
    /*buf偏移量*/
    Uint32 dwOffset;
}PACKED TStreamSockHead;

/*调试模式及调试参数*/
typedef struct 
{
    /*视频检测*/
    Bool bVideoLoop;
    /*视频追踪*/
    Bool bVideotrack;
    /*辅助对焦*/
    Bool bAuxFocus;
    /*曝光参数*/
    Bool bExposure;
    /*外设状态*/
    Bool bExDevice;
    /*2D雷达*/
    Bool b2DRadar;
    /*红灯测量*/
    Bool bRedLampTest;
    /*等效快门 固定值100~51200*/
    Uint32 dwEquShutter; 
    /*图片大小: 0--1/4大小、1--1/2大小、2--原图*/
    Uint32 dwPicSize;
    /*压缩因子*/
    Uint32 dwQpValue;
	/*违停模式*/
	Bool bIllMode;
}PACKED TIPC_DebugParam;

/*Osd背景图片*/
typedef struct
{
    /*图片的大小*/
    Uint32 dwSize;
    /*图片抓拍时间*/
    Int64 qwTime;
    /*图片类型*/
    Uint32 dwType;
    /*图片宽*/
    Uint32 dwWidth;
    /*图片高*/
    Uint32 dwHeight;
    /*Osd内容*/
    char szOsdContent[MAX_SIZE_LEN512];
    /*图片数据*/
    void *szBuf;
}PACKED TIPC_OsdImgData;

/*视频检测器获取与设置*/
typedef struct
{
    Int32       nRetCode;
    Uint32      dwSceneId;
    TIPC_VidLoopCfg tVideoLoop[VIDEO_LOOP_NUM_MAX];
}PACKED TIPC_GetVideoLoopRsp;

typedef struct
{
    Uint32      dwSceneId;
    TIPC_VidLoopCfg tVideoLoop[VIDEO_LOOP_NUM_MAX];
}PACKED TIPC_SetVideoLoopReq;

/*线圈检测器获取与设置*/
typedef struct
{
    Int32       nRetCode;
    Uint32      dwSceneId;
    TIPC_CoilLoopCfg    tCoilLoop[COIL_LOOP_CHANNEL_MAX];
}PACKED TIPC_GetCoilLoopRsp;

typedef struct
{
    Uint32          dwSceneId;
    TIPC_CoilLoopCfg    tCoilLoop[COIL_LOOP_CHANNEL_MAX];
}PACKED TIPC_SetCoilLoopReq;

/*雷达检测器获取与设置*/
typedef struct
{
    Int32       nRetCode;
    Uint32      dwSceneId;
    TIPC_RadarLoopCfg   tRadarLoop[RADAR_NUM_MAX];
}PACKED TIPC_GetRadarLoopRsp;

typedef struct
{
    Uint32          dwSceneId;
    TIPC_RadarLoopCfg   tRadarLoop[RADAR_NUM_MAX];
}PACKED TIPC_SetRadarLoopReq;

/*红灯信号检测器获取与设置*/
typedef struct
{
    Int32           nRetCode;
    Uint32          dwSceneId;
    TIPC_RedLampSigLoop tRedLoop[MAX_REDLAMP_DET_NUM];
}PACKED TIPC_GetRedLampSigRsp;

typedef struct
{
    Uint32          dwSceneId;
    TIPC_RedLampSigLoop tRedLoop[MAX_REDLAMP_DET_NUM];
}PACKED TIPC_SetRedLampSigReq;

/*识别区域获取和设置*/
typedef struct
{
    Int32       nRetCode;
    Uint32      dwSceneId;
    TIPC_RecogArea  tRecogArea[ROAD_NUM_MAX];
}PACKED TIPC_GetRecogAreaRsp;

typedef struct
{
    Uint32      dwSceneId;
    TIPC_RecogArea  tRecogArea[ROAD_NUM_MAX];
}PACKED TIPC_SetRecogAreaReq;

/*IO检测器获取与设置*/
typedef struct
{
    Uint32  IOLabel;
    Int32   nRetCode;
}PACKED TIPC_GetIOLoopRsp;

typedef struct
{
    Uint32  IOLabel;
    Int32   nRetCode;
}PACKED TIPC_SetIOLoopRsp;

/*红灯增强获取与设置*/
typedef struct
{
    Int32       nRetCode;
    Uint32      dwSceneId;
    TIPC_RedSigEnhance tRedEnLoop[MAX_REDLAMP_DET_NUM];
}PACKED TIPC_GetRedEnRsp;

typedef struct
{
    Uint32         dwSceneId;
    TIPC_RedSigEnhance tRedEnLoop[MAX_REDLAMP_DET_NUM];
}PACKED TIPC_SetRedEnReq;

/*逻辑车道获取与设置*/
typedef struct
{
    Int32   nRetCode;
    Uint32  dwSceneId;
    Uint16  wLogicLaneNum;
    TIPC_LogicLane  tLogicLane[LOGIC_ROAD_NUM_MAX];
}PACKED TIPC_GetLogicLaneRsp;

typedef struct
{
    Uint32      dwSceneId;
    Uint16      wLogicLaneNum;
    TIPC_LogicLane  tLogicLane[LOGIC_ROAD_NUM_MAX];
}PACKED TIPC_SetLogicLaneReq;

/*特征识别参数获取*/
typedef struct  
{
    TIPC_RecogParam tParam;
    Int32       nRetCode;
}PACKED TIPC_GetRecogParamRsp;

/*视频测速配置获取*/
typedef struct
{
    Int32 nRetCode;
    TIPC_VidSpdDet tVidSpdDet;
}PACKED TIPC_GetVidSpdDetRsp;

/*图像调节配置获取*/
typedef struct
{
    Int32 nRetCode;
    TIPC_KdISPCfg tKdISPCfg;
}PACKED TIPC_GetKISPCfgRsp;

/*获取电源相位同步*/
typedef struct  
{
    Int32 nRetCode;
    TIPC_PowerPhaseSync tPowerSync;
}PACKED TIPC_GetPowerSnycRsp;

/*获取ipc调试参数*/
typedef struct
{
    Int32 nRetCode;
    TIPC_DebugParam tDebugParam;
}PACKED TIPC_GetDebugParamRsp;

/*获取ipc串口配置*/
typedef struct
{
    Int32 nRetCode;
    TIPC_SerialCfg tSerialCfg;
}PACKED TIPC_GetSerialCfgRsp;

/*----------------------------------------------------------
 *前端结构体（IPC V7）
 *----------------------------------------------------------*/
/*UI功能能力集*/
typedef struct
{
    Int32       nRetCode;   // 
    Bool        bCap;       // 0:不支持; 1：支持
    Uint8       szCaps[EMUI_CAP_TYPE_COUNT]; // EMUI_CAP_TYPE；0:不支持，> 0 ：支持; 
                                             //1,一代版本， 2，二代版本；不同版本的说明在枚举定义位置说明
}PACKED TIPC_GetUICapsRsp;

typedef struct {
    /*违章类型*/
    Uint32 illegalName;
    /*违章代码*/
    Uint32 wzCode;
}PACKED TIllegalDict;

typedef struct {
	/*实际违章类型数*/
	Uint32 dwNum;
	/*违章类型*/
	TIllegalDict atIllegalDict[MAX_NUM_ILLEGAL_TYPE];
}PACKED TIllegalDictLisl;

typedef struct {
	/*文件内容的大小*/
	Uint32 dwSize;
	/*文件内容*/
	Char *szData;
}PACKED TDevAllConfig;


/*违停检测区域*/
typedef struct {
    /*使能*/
    Bool enable;
    /*最小车辆宽度*/
    Uint32 minVehWidth;
    /*最大车辆宽度*/
    Uint32 maxVehWidth;
    /*检测灵敏度: 0--高, 1--低*/
    Uint32 level;
	/*上半部扫描阈值*/
	float halfScanfVal;
	/*扫描步长*/
	float scanfstep;
	/*阈值补偿*/
	float trimVal;
	/*进入滤波*/
	Uint32 filterOn;
	/*离开滤波*/
	Uint32 filterOff;
    /*区域坐标*/
    Uint32 pointNum;
    TPoint_S  tPoint[ROI_AREA_POINT_MAX];
}TIPC_illParkArea_S;

/*违停配置参数: 最大支持4个场景*/
typedef struct {
	/*是否使能*/
	Bool enable;
	/*工作模式:单次抓拍模式、二次确认模式   三次确认模式*/
	Uint32 workMode;
	/*场景ID号*/
	Uint32 parkId;
    /*支持的场景个数: 只读属性*/
    Uint32 sptSceneNum;
	/*预置位编号*/
    Uint32 PrePosLabel;
    /*轮询时间: 精确到秒*/
    Uint32 timeRetention;
    /*违停抓拍时间: 精确到秒*/
    Uint32 illParkTime;
	/*违停发出提醒时间: 精确到秒*/
	Uint32 illParkNoticeTime;
    /*区域个数: 最多4个*/
    Uint32 RoiAreaNum;
    /*检测区域*/
    TIPC_illParkArea_S tRoiArea[VEH_DET_ROI_NUM_MAX];
	/*启用时间*/
    TIPC_SnapTiming tSnapTiming[MAX_NUM_SNAPTIMING];
	/*二次抓拍间隔（分钟）*/
	Uint32 dwSecSnapTime;
	/*快门上限*/
    Uint32 maxShutter;
    /*增益上限*/
    Uint32 maxAgain;
    /*内置灯1*/
    Uint32 internalLight1; 
    /*内置灯2*/
    Uint32 internalLight2;
    /*内置灯3*/
    Uint32 internalLight3;
    /*外置灯*/
    Uint32 externalLight;
	/*图片1--全景 2--中景 3--近景 4--特写*/
	Uint32 picMode[VEH_DET_ROI_NUM_MAX];

}TIPC_illgParkPara;

/*获取违停配置参数请求*/
typedef struct {
	/*场景ID号*/
	Uint32 parkId;
}TIPC_GetIllgParkParaReq;

/*灯亮度参数*/
typedef struct
{
    /*1倍放大灯亮度*/
    Uint32 brightness1x;
    /*2倍放大灯亮度*/
    Uint32 brightness2x;
    /*3倍放大灯亮度*/
    Uint32 brightness3x;
    /*4倍放大灯亮度*/
    Uint32 brightness4x;
    /*5倍放大灯亮度*/
    Uint32 brightness5x;
    /*6倍放大灯亮度*/
    Uint32 brightness6x;
    /*7倍放大灯亮度*/
    Uint32 brightness7x;
    /*8倍放大灯亮度*/
    Uint32 brightness8x;
    /*9倍放大灯亮度*/
    Uint32 brightness9x;
    /*10倍放大灯亮度*/
    Uint32 brightness10x;
    /*11倍放大灯亮度*/
    Uint32 brightness11x;
    /*12倍放大灯亮度*/
    Uint32 brightness12x;
    /*13倍放大灯亮度*/
    Uint32 brightness13x;
    /*14倍放大灯亮度*/
    Uint32 brightness14x;
    /*15倍放大灯亮度*/
    Uint32 brightness15x;
    /*16倍放大灯亮度*/
    Uint32 brightness16x;
    /*17倍放大灯亮度*/
    Uint32 brightness17x;
    /*18倍放大灯亮度*/
    Uint32 brightness18x;
    /*19倍放大灯亮度*/
    Uint32 brightness19x;
    /*20倍放大灯亮度*/
    Uint32 brightness20x;
    /*21倍放大灯亮度*/
    Uint32 brightness21x;
    /*22倍放大灯亮度*/
	Uint32 brightness22x;
    /*23倍放大灯亮度*/
    Uint32 brightness23x;
    /*24倍放大灯亮度*/
    Uint32 brightness24x;
    /*25倍放大灯亮度*/
    Uint32 brightness25x;
    /*26倍放大灯亮度*/
    Uint32 brightness26x;
    /*27倍放大灯亮度*/
    Uint32 brightness27x;
    /*28倍放大灯亮度*/
    Uint32 brightness28x;
    /*29倍放大灯亮度*/
    Uint32 brightness29x;
    /*30倍放大灯亮度*/
    Uint32 brightness30x;
    /*31倍放大灯亮度*/
    Uint32 brightness31x;
    /*32倍放大灯亮度*/
    Uint32 brightness32x;
    /*33倍放大灯亮度*/
    Uint32 brightness33x;
    /*34倍放大灯亮度*/
    Uint32 brightness34x;
    /*35倍放大灯亮度*/
    Uint32 brightness35x;
    /*36倍放大灯亮度*/
    Uint32 brightness36x;
    /*37倍放大灯亮度*/
    Uint32 brightness37x;
}TIPC_LightParam;

typedef struct 
{
	/*是否启用*/
	Bool bEnable;
	/*预置位*/
	Uint32 posId;
	/*停留时间*/
	Uint32 stayTime[MAX_NUM_SNAPTIMING][MAX_NUM_DECT_TIME];
	/*应用类型*/
	Uint32 appType;
	/*快门上限*/
    Uint32 maxShutter;
    /*增益上限*/
    Uint32 maxAgain;
    /*内置灯1*/
    Uint32 internalLight1; 
    /*内置灯2*/
    Uint32 internalLight2;
    /*内置灯3*/
    Uint32 internalLight3;
    /*外置灯1*/
    Uint32 externalLight1;
	/*外置灯2*/
    Uint32 externalLight2;
    /*启用时间*/
    TIPC_SnapTiming tSnapTiming[MAX_NUM_SNAPTIMING];

}PACKED TIPC_ScenePoll;

typedef struct
{
    Uint32 dwFlowCnt;
    Uint32 dwVehSpeed;
    /*车辆平均速度,不区分车道*/
	Uint32 dwVehAveSpd;
    Uint32 dwDeltaTimes;
    Uint32 dwDeltaDistance;
    float  fTimeOccupancy;
    float  fSpaceOccupancy;
    float  fVehQueLen;
    /*路况 ETrafficState*/
    Uint32 dwState;
}TIPC_LaneFlowInfo;

/*车道统计信息*/
typedef struct
{
    /*统计时间*/
    Uint32 dwStatTime[E_UPDATE_PERIOD_TYPE_COUNT];
    /*统计流量值*/
    Uint32 dwStatCnts;
    Uint32 dwVehAveSpd;
}TIPC_LaneFlowStatInfo;

typedef struct
{
    Bool bEnable;
    /*更新方式: 1未来时间点 2周期更新*/
    Uint8 byUpdateType;
    /*更新周期 如每2小时统计更新 dwUpdatePeriod[E_UPDATE_PERIOD_TYPE_HOUR] = 2*/
    Uint32 dwUpdatePeriod[E_UPDATE_PERIOD_TYPE_COUNT];
    /*滞留检测器对应的物理距离 远检测器在前*/
    float fDetPhyLen[RETENTION_NUM_MAX];
    /*IPC可视角度下的道路总长度*/
    //float fPhyTotalLen;
    /*所需要更新的流量值*/
	Uint32 dwUpdateFlowCnts;
}TVidLopFlowCfg;

/*获取车流量检测器配置响应*/
typedef struct
{
    Int32   nRetCode;
    Uint32  dwSceneId;
    TVidLopFlowCfg  atFlowCfg[ROAD_NUM_MAX];
}TIPC_GetVidLoopFlowCfgRsp;

/*设置车流量检测器配置请求*/
typedef struct
{
    Uint32  dwSceneId;
    TVidLopFlowCfg atFlowCfg[ROAD_NUM_MAX];
}TIPC_SetVidLoopFlowCfgReq;

/*车流量信息通知*/
typedef struct
{
    Uint32  dwSceneId;
    TIPC_LaneFlowInfo  atFlowInfo[ROAD_NUM_MAX];
}TIPC_VehFlowInfoNty;

/*车流量统计通知*/
typedef struct
{
    Uint32  dwSceneId;
	Uint32  dwLaneId; /*0: 所有车道更新，其他:对应车道更新,取下结构体对应车道信息*/
    TIPC_LaneFlowStatInfo  atFlowStat[ROAD_NUM_MAX];
}TIPC_VehFlowStatInfoNty;

typedef struct
{
    /*序列号*/
    Uint32 serial;
    /*违法名称*/
    Char illegalName[64];
    /*违法代码*/
    Uint32 code;
	 /*事件代码，与协议的EIllegalType枚举值对应*/
    Uint32 ivhsEventCode;
}TIPC_IllegalCode;

typedef struct
{
    /*字典个数*/
    Uint32 sum;
    /*字典数据*/
    TIPC_IllegalCode tIllegalCode[MAX_ILLEGAL_NUM];
}TIPC_IllegalDict;

typedef struct
{
    /*前端硬件id号*/
	Char achDevId[MAX_SIZE_DEV_ID];
    /*故障类型，参考E_DEV_ERROR_NOTICE*/
    Uint32 dwErrorNotice;
	/*故障当前状态,参考E_DEV_STATE*/
    Uint32 dwState;

}TIPC_DevStateNty;


// #ifdef _MSC_VER
//     #pragma pack(pop)
// #endif //WIN32

#pragma pack(pop)
#endif //_IVHS_STRUCT_H_




