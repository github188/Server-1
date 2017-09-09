#ifndef _IVHS_DEFINE_H_
#define _IVHS_DEFINE_H_

#pragma pack(push, 1)
#ifdef _MSC_VER
#define PACKED
#else
#define PACKED
#endif //LINUX

#include "kosa.h"
//#include "kdvtype.h"

/*----------------------------------------------------------
*消息类型定义(科达消息    +    统一接口层消息)
*----------------------------------------------------------*/

/*******************科达消息类型定义:相机********************/
/*心跳类型
参数：
*/
#define EV_KEDA_HEARTBEAT_CMD                       102
/*发送过车信息数据
参数：TVechileData
*/
#define EV_VEHICLE_INFO_DATA_CMD                    1724
/*发送历史数据命令
参数：
*/
#define EV_START_SEND_HISTORY_DATA_CMD              1725
/*停止发送历史数据命令
参数：
*/
#define EV_STOP_SEND_HISTORY_DATA_CMD               1726
/*历史数据上传完毕通知
参数：
*/
#define EV_UPLOAD_FINISHED_NTY                      1727
/*接收过车数据的响应
参数：TVechileDataAck
*/
#define EV_VEHICLE_INFO_DATA_ACK                    1728
/*发送停止采集视频数据命令
参数：TVideoCapture
*/
#define EV_STOP_CAPTURE_VIDEO_CMD                   1729
/*实时视频数据
参数：
*/
#define EV_REAL_TIME_VIDEO_CMD                      1730
/*搜索局域网内设备请求
参数：
*/
#define EV_DEV_DISCOVERY_REQ                        1731
/*搜索局域网内设备响应
参数：
*/
#define EV_DEV_DISCOVERY_ACK                        1732
/*前端入网注册请求
参数：TLoginReq
*/
#define EV_REGISTER_REQ                             1733
/*前端入网注册响应
参数：TLoginRsp
*/
#define EV_REGISTER_ACK                             1734
/*前端重启命令
参数：
*/
#define EV_DEV_REBOOT_CMD                           1735
/*前端时间同步请求
参数：
*/
#define EV_TIME_SYNC_REQ                            1736
/*前端时间同步响应
参数：
*/
#define EV_TIME_SYNC_ACK                            1737
/*前端故障上报
参数：TIPC_DevStateNty
*/
#define EV_DEV_BREAKDOWN_NTY                        1738
/*交通参数信息上报
参数：
*/
#define EV_TRAFFIC_CFG_NTY                          1739
/*前端下线请求
参数：
*/
#define EV_DEV_OFFLINE_REQ                          1740
/*前端下线响应
参数：
*/
#define EV_DEV_OFFLINE_ACK                          1741
/*前端断链通知
参数：设备ID
*/
#define EV_DEV_DISCONNECT_NTY                      1742
/*获取前端设备参数请求
参数：achDevId
*/
#define EV_GET_DEV_CFG_REQ                          1743
/*获取前端设备参数应答
参数：TCfgInfo
*/
#define EV_GET_DEV_CFG_ACK                          1744
/*设置前端设备参数请求
参数：TCfgInfo
*/
#define EV_SET_DEV_CFG_REQ                          1745
/*设置前端设备参数应答
参数：achDevId(64)+retcode(u16)
*/
#define EV_SET_DEV_CFG_ACK                          1746
/*布控请求
参数：
*/
#define EV_ARRAGEMENT_REQ                           1747
/*布控响应
参数：
*/
#define EV_ARRAGEMENT_ACK                           1748
/*撤控请求
参数：
*/
#define EV_REMOVE_ARRAGEMENT_REQ                    1749
/*撤控响应
参数：
*/
#define EV_REMOVE_ARRAGEMENT_ACK                    1750
/*获取卡口列表请求
参数：
*/
#define EV_GET_BAY0NET_LIST_REQ                     1751
/*获取卡口列表响应
参数：
*/
#define EV_GET_BAY0NET_LIST_ACK                     1752
/*查询设备工作状态请求
参数：
*/
#define EV_GET_DEV_STATE_REQ                        1753
/*查询设备工作状态响应
参数：
*/
#define EV_GET_DEV_STATE_ACK                        1754
/*报警数据上报
参数：
*/
#define EV_ALARM_DATA_NTY                           1755
/*订阅卡口命令
参数：
*/
#define EV_SUBSCRIBE_CMD                            1756
/*取消订阅卡口命令
参数：
*/
#define EV_SUBSCRIBE_CANCLE_CMD                     1757
/*发送通用消息请求*/
#define EV_SEND_COMMON_REQ                          1224
/*发送通用消息响应*/
#define EV_SEND_COMMON_ACK                          1225
/*新建连接通知*/
#define EV_NEW_CONNECT_NTY                          1226

/*闸机控制请求（平台-->设备）*/
#define EV_OP_DOOR_REQ                              5000
/*闸机控制响应（设备-->平台）*/
#define EV_OP_DOOR_ACK                              5001
/*上报串口状态信息 （设备-->平台）*/
#define EV_UPLOAD_SERIAL_INFO_NTY                   5002
/*开始发送时实视频码流请求*/
#define EV_START_SEND_VIDEO_STREAM_REQ              5003
/*开始发送时实视频码流响应*/
#define EV_START_SEND_VIDEO_STREAM_ACK              5004
/*停止发送时实视频码流请求*/
#define EV_STOP_SEND_VIDEO_STREAM_REQ               5005
/*停止发送时实视频码流响应*/
#define EV_STOP_SEND_VIDEO_STREAM_ACK               5006
/*获取相机时间请求*/
#define EV_GET_CAM_TIME_REQ                         5007
/*获取相机时间响应*/
#define EV_GET_CAM_TIME_ACK                         5008
/*系统升级请求*/
#define EV_SYSTEM_UPGRADE_REQ                       5009
/*系统升级响应*/
#define EV_SYSTEM_UPGRADE_ACK                       5010

#define EV_SYN_SNP_REQ                              5011
#define EV_SYN_SNP_ACK                              5012

/*****************科达消息类型定义:电警业务******************/
/*获取图片合成*/
#define EV_GET_IMAGE_MERGE_CFG_REQ                  5200    //NULL
#define EV_GET_IMAGE_MERGE_CFG_ACK                  5201    //TImageMergeRsp
/*设置图片合成*/
#define EV_SET_IMAGE_MERGE_CFG_REQ                  5202    //TImageMergeCfg
#define EV_SET_IMAGE_MERGE_CFG_ACK                  5203    //NULL
/*获取osd叠加*/
#define EV_GET_OSD_CONFIG_REQ                       5204    //带参u32 osd type 见ENUM_ITS_OSD_IMAGE_TYPE
#define EV_GET_OSD_CONFIG_ACK                       5205    //TOsdCfgRsp
/*设置osd叠加*/
#define EV_SET_OSD_CONFIG_REQ                       5206    //TOsdCfgInfo
#define EV_SET_OSD_CONFIG_ACK                       5207    //NULL
/*获取osd字体*/
#define EV_GET_OSD_FONT_CFG_REQ                     5208    //带参u32 osd type 同上
#define EV_GET_OSD_FONT_CFG_ACK                     5209    //TOsdFontCfgRsp
/*设置osd字体*/
#define EV_SET_OSD_FONT_CFG_REQ                     5210    //TOsdFontCfg
#define EV_SET_OSD_FONT_CFG_ACK                     5211    //NULL
/*图片存储*/
#define EV_GET_STORE_CONFIG_REQ                     5212    //NULL
#define EV_GET_STORE_CONFIG_ACK                     5213    //TImageStoreRsp
/*图片存储*/
#define EV_SET_STORE_CONFIG_REQ                     5214    //TImageStore
#define EV_SET_STORE_CONFIG_ACK                     5215    //NULL
/*获取中心服务器*/
#define EV_GET_SERVER_CONFIG_REQ                    5216    //NULL
#define EV_GET_SERVER_CONFIG_ACK                    5217    //TITS_GetServerRsp
/*设置中心服务器*/
#define EV_SET_SERVER_CONFIG_REQ                    5218    //TITS_SetServerReq
#define EV_SET_SERVER_CONFIG_ACK                    5219    //NULL
/*添加前端设备*/
#define EV_ADD_DEVICE_REQ                           5220    //TITS_DevList
#define EV_ADD_DEVICE_ACK                           5221    //暂不用 NULL
/*删除前端设备*/
#define EV_DEL_DEVICE_REQ                           5222    //TITS_DevList
#define EV_DEL_DEVICE_ACK                           5223    //NULL
/*获取设备信息*/
#define EV_GET_DEVINFO_CONFIG_REQ                   5224    //TITS_GetDevInfoReq
#define EV_GET_DEVINFO_CONFIG_ACK                   5225    //TITS_GetDevInfoRsp
/*设置设备信息*/
#define EV_SET_DEVINFO_CONFIG_REQ                   5226    //TITS_SetDevInfoReq
#define EV_SET_DEVINFO_CONFIG_ACK                   5227    //NULL
/*获取辅助相机设备参数*/
#define EV_GET_AUX_DEVINFO_REQ                      5228    //TITS_GetDevInfoReq
#define EV_GET_AUX_DEVINFO_ACK                      5229    //TITS_GetAuxCameraRsp
/*设置辅助相机设备参数*/
#define EV_SET_AUX_DEVINFO_REQ                      5230    //TITS_SetAuxCameraReq
#define EV_SET_AUX_DEVINFO_ACK                      5231    //NULL
/*获取设备列表*/
#define EV_GET_DEVLIST_CONFIG_REQ                   5232    //NULL
#define EV_GET_DEVLIST_CONFIG_ACK                   5233    //TITS_GetDevListRsp
/*获取录像设置*/
#define EV_GET_VIDEO_CONFIG_REQ                     5234    //TITS_GetVideoCfgReq
#define EV_GET_VIDEO_CONFIG_ACK                     5235    //TITS_GetVideoCfgRsp
/*设置录像设置*/
#define EV_SET_VIDEO_CONFIG_REQ                     5236    //TITS_VideoSectCfg
#define EV_SET_VIDEO_CONFIG_ACK                     5237    //NULL

/*实时过车信息接收(用于客户端)*/
#define EV_RT_VEHICLE_INFO_REC_REQ                  5238    //TITS_RtVehicleReq
/*数据查询*/
#define EV_QUERY_DATA_REQ                           5239    //TDataQuery
#define EV_QUERY_DATA_ACK                           5240    //TITS_QueryResultRsp

/*图片推送挂起和恢复(目前用于客户端)*/
#define EV_POST_PIC_SUSPEND                         5241    //NULL
#define EV_POST_PIC_UNSUSPEND                       5242    //NULL

/***************科达消息类型定义:IPC V7业务*****************/

/*IPCtrl抓拍图片*/
#define EV_CAPTURE_PIC_REQ                          6000    //u32 capture_type
#define EV_CAPTURE_PIC_ACK                          6001    //NULL
/*获取场景配置*/
#define EV_GET_SCENE_CFG_REQ                        6002    //u32 dwSceneId
#define EV_GET_SCENE_CFG_ACK                        6003    //TIPC_GetSceneCfgRsp
/*设置场景配置*/
#define EV_SET_SCENE_CFG_REQ                        6004    //TIPC_SetSceneCfgReq
#define EV_SET_SCENE_CFG_ACK                        6005    //NULL
/*自动生成视频检测器*/
#define EV_AUTO_CREATE_VIDEO_LOOP_REQ               6006    //u32 dwSceneId
#define EV_AUTO_CREATE_VIDEO_LOOP_ACK               6007    //NULL
/*获取视频检测器配置*/
#define EV_GET_VIDEO_LOOP_CFG_REQ                   6008    //u32 dwSceneId
#define EV_GET_VIDEO_LOOP_CFG_ACK                   6009    //TIPC_GetVideoLoopRsp
/*设置视频检测器配置*/
#define EV_SET_VIDEO_LOOP_CFG_REQ                   6010    //TIPC_SetVideoLoopReq
#define EV_SET_VIDEO_LOOP_CFG_ACK                   6011    //NULL
/*获取线圈检测器配置*/
#define EV_GET_COIL_LOOP_CFG_REQ                    6012    //u32 dwSceneId
#define EV_GET_COIL_LOOP_CFG_ACK                    6013    //TIPC_GetCoilLoopRsp
/*设置线圈检测器配置*/
#define EV_SET_COIL_LOOP_CFG_REQ                    6014    //TIPC_SetCoilLoopReq
#define EV_SET_COIL_LOOP_CFG_ACK                    6015    //NULL
/*获取雷达检测器配置*/
#define EV_GET_RADAR_LOOP_CFG_REQ                   6016    //u32 dwSceneId
#define EV_GET_RADAR_LOOP_CFG_ACK                   6017    //TIPC_GetRadarLoopRsp
/*设置雷达检测器配置*/
#define EV_SET_RADAR_LOOP_CFG_REQ                   6018    //TIPC_SetRadarLoopReq
#define EV_SET_RADAR_LOOP_CFG_ACK                   6019    //NULL
/*获取IO检测器配置*/
#define EV_GET_IO_LOOP_CFG_REQ                      6020    //未实现
#define EV_GET_IO_LOOP_CFG_ACK                      6021    //未实现
/*设置IO检测器配置*/
#define EV_SET_IO_LOOP_CFG_REQ                      6022    //未实现
#define EV_SET_IO_LOOP_CFG_ACK                      6023    //未实现
/*获取红灯检测器配置*/
#define EV_GET_RED_LAMP_CFG_REQ                     6024    //u32 dwSceneId
#define EV_GET_RED_LAMP_CFG_ACK                     6025    //TIPC_GetRedLampSigRsp
/*设置红灯检测器配置*/
#define EV_SET_RED_LAMP_CFG_REQ                     6026    //TIPC_SetRedLampSigReq
#define EV_SET_RED_LAMP_CFG_ACK                     6027    //NULL
/*获取识别区域配置*/
#define EV_GET_RECOG_AREA_CFG_REQ                   6028    //u32 dwSceneId
#define EV_GET_RECOG_AREA_CFG_ACK                   6029    //TIPC_GetRecogAreaRsp
/*设置识别区域配置*/
#define EV_SET_RECOG_AREA_CFG_REQ                   6030    //TIPC_SetRecogAreaReq
#define EV_SET_RECOG_AREA_CFG_ACK                   6031    //NULL
/*获取红灯增强配置*/
#define EV_GET_ENHANCE_RED_CFG_REQ                  6032    //u32 dwSceneId
#define EV_GET_ENHANCE_RED_CFG_ACK                  6033    //TIPC_GetRedEnRsp
/*设置红灯增强配置*/
#define EV_SET_ENHANCE_RED_CFG_REQ                  6034    //TIPC_SetRedEnReq
#define EV_SET_ENHANCE_RED_CFG_ACK                  6035    //NULL
/*自动生成逻辑车道*/
#define EV_AUTO_CREATE_LOGIC_LANE_REQ               6036    //u32 dwSceneId
#define EV_AUTO_CREATE_LOGIC_LANE_ACK               6037    //NULL
/*获取逻辑车道*/
#define EV_GET_LOGIC_LANE_CFG_REQ                   6038    //u32 dwSceneId
#define EV_GET_LOGIC_LANE_CFG_ACK                   6039    //TIPC_GetLogicLaneRsp
/*设置逻辑车道*/
#define EV_SET_LOGIC_LANE_CFG_REQ                   6040    //TIPC_SetLogicLaneReq
#define EV_SET_LOGIC_LANE_CFG_ACK                   6041    //NULL
/*获取识别参数*/
#define EV_GET_RECOG_PARAM_CFG_REQ                  6042    //NULL
#define EV_GET_RECOG_PARAM_CFG_ACK                  6043    //TIPC_GetRecogParamRsp
/*设置识别参数*/
#define EV_SET_RECOG_PARAM_CFG_REQ                  6044    //TIPC_RecogParam
#define EV_SET_RECOG_PARAM_CFG_ACK                  6045    //NULL
/*获取IPC设备参数*/
#define EV_GET_IPC_DEV_INFO_REQ                     6046    //NULL
#define EV_GET_IPC_DEV_INFO_ACK                     6047    //TIPC_GetDevRsp
/*设置IPC设备参数*/
#define EV_SET_IPC_DEV_INFO_REQ                     6048    //TIPC_DevInfo
#define EV_SET_IPC_DEV_INFO_ACK                     6049    //NULL
/*PTZ控制*/
#define EV_PTZ_CTRL_REQ                             6050    //TIPC_PTZCtrl
#define EV_PTZ_CTRL_ACK                             6051    //NULL
/*获取IPC视频测速*/
#define EV_GET_VIDEO_SPD_DET_CFG_REQ                6052    //NULL
#define EV_GET_VIDEO_SPD_DET_CFG_ACK                6053    //TIPC_GetVidSpdDetRsp
/*设置IPC视频测速*/
#define EV_SET_VIDEO_SPD_DET_CFG_REQ                6054    //TIPC_VidSpdDet
#define EV_SET_VIDEO_SPD_DET_CFG_ACK                6055    //NULL
/*获取图像调节参数*/
#define EV_GET_IPC_ISP_CFG_REQ                      6056    //NULL
#define EV_GET_IPC_ISP_CFG_ACK                      6057    //TIPC_GetKISPCfgRsp
/*设置图像调节参数*/
#define EV_SET_IPC_ISP_CFG_REQ                      6058    //TIPC_KdISPCfg
#define EV_SET_IPC_ISP_CFG_ACK                      6059    //NULL
/*获取电源相位同步*/
#define EV_GET_IPC_POWER_SYNC_REQ                   6060    //NULL
#define EV_GET_IPC_POWER_SYNC_ACK                   6061    //TIPC_GetPowerSnycRsp
/*设置电源相位同步*/
#define EV_SET_IPC_POWER_SYNC_REQ                   6062    //TIPC_PowerPhaseSync
#define EV_SET_IPC_POWER_SYNC_ACK                   6063    //NULL
/*开码流 JPEG调试*/
#define EV_IPC_BIT_STREAM_START_REQ                 6064    //TIPC_BitStreamInfo
#define EV_IPC_BIT_STREAM_START_ACK                 6065    //TIPC_BitStreamRsp
/*关码流 JPEG调试*/
#define EV_IPC_BIT_STREAM_STOP_REQ                  6066    //u32 (start_ack返回的句柄)
#define EV_IPC_BIT_STREAM_STOP_ACK                  6067    //TIPC_StopBitStreamRsp
/*获取调试参数*/
#define EV_GET_IPC_DEBUG_PARAM_REQ                  6068    //NULL
#define EV_GET_IPC_DEBUG_PARAM_ACK                  6069    //TIPC_GetDebugParamRsp
/*设置调试参数*/
#define EV_SET_IPC_DEBUG_PARAM_REQ                  6070    //TIPC_DebugParam
#define EV_SET_IPC_DEBUG_PARAM_ACK                  6071    //NULL
/*UI能力集获取*/
#define EV_GET_IPC_UI_CAP_REQ                       6072    //NULL
#define EV_GET_IPC_UI_CAP_ACK                       6073    //TIPC_GetUICapsRsp
/*获取串口配置*/
#define EV_GET_IPC_SERIAL_CFG_REQ                   6074    //NULL
#define EV_GET_IPC_SERIAL_CFG_ACK                   6075    //TIPC_GetSerialCfgRsp
/*设置串口配置*/
#define EV_SET_IPC_SERIAL_CFG_REQ                   6076    //TIPC_SerialCfg
#define EV_SET_IPC_SERIAL_CFG_ACK                   6077    //NULL
/*获取智能IPC版本号*/
#define EV_GET_IPC_VERSION_REQ                      6078    //NULL
#define EV_GET_IPC_VERSION_ACK                      6079    //char szVersion[64]

#define EV_UPLOAD_IPC_OSD_BACK_PIC                  6080    //TIPC_OsdImgData

/*获取违章类型的所有描述请求,消息内容为空*/
#define EV_GET_ILLEGAL_DICT_REQ						6081
/*获取违章类型的所有描述相应，消息内容TIllegalDictLisl*/
#define EV_GET_ILLEGAL_DICT_ACK						6082


/*获取违章类型的所有描述请求,消息内容为TIllegalDictLisl*/
#define EV_SET_ILLEGAL_DICT_REQ						6083
/*获取违章类型的所有描述相应，消息内容为空*/
#define EV_SET_ILLEGAL_DICT_ACK						6084


/*恢复出厂设置请求，消息内容为puid*/
#define EV_RESTORE_SETTINE_REQ						6085
/*恢复出厂设置响应，消息内容为puid*/
#define EV_RESTORE_SETTINE_ACK						6086

/*导出配置文件请求，消息内容为puid*/
#define EV_READ_CONFIG_REQ							6087
/*导出配置文件响应，消息内容为TDevAllConfig*/
#define EV_READ_CONFIG_ACK							6088


/*获取违停配置参数，消息内容为TIPC_GetIllgParkParaReq*/
#define EV_GET_ILLG_PARK_REQ						6089
/*获取违停配置参数，消息内容为TIPC_illgParkPara*/
#define EV_GET_ILLG_PARK_ACK						6090

/*设置违停配置参数，消息内容为TIPC_illgParkPara*/
#define EV_SET_ILLG_PARK_REQ						6091
/*设置违停配置参数，消息内容为puid*/
#define EV_SET_ILLG_PARK_ACK						6092

/*查询过车记录请求*/
#define EV_QUERY_RECORD_REQ                         6093    //TIPC_PicQuery
/*查询过车记录响应*/
#define EV_QUERY_RECORD_ACK                         6094    //TIPC_PicQueryResult
/*获取过车记录请求*/
#define EV_GET_RECORD_REQ                           6095    //Uint32 RecordId 实际对应db中的KeyId
/*获取过车记录响应*/
#define EV_GET_RECORD_ACK                           6096    //TVechileData

/*获取灯亮度参数请求，消息内容为puid*/
#define EV_GET_LIGHT_PARAM_REQ                      6097    //
/*获取过灯亮度参数响应，消息内容为TIPC_LightParam*/
#define EV_GET_LIGHT_PARAM_ACK                      6098    //TIPC_LightParam
/*设置灯亮度参数请求，消息内容为TIPC_LightParam*/
#define EV_SET_LIGHT_PARAM_REQ                      6099    //TIPC_LightParam 
/*设置灯亮度参数响应，消息内容为puid*/
#define EV_SET_LIGHT_PARAM_ACK                      6100    //

/*获取ScenePoll参数请求，消息内容为puid*/
#define EV_GET_SCENEPOLL_REQ                        6101    //
/*获取过ScenePoll参数响应，消息内容为TIPC_ScenePoll[MAX_NUM_SCENEPOLL]*/
#define EV_GET_SCENEPOLL_ACK                        6102    //
/*设置ScenePoll参数请求，消息内容为TIPC_ScenePoll[MAX_NUM_SCENEPOLL]*/
#define EV_SET_SCENEPOLL_REQ                        6103    //
/*设置ScenePoll参数响应，消息内容为puid*/
#define EV_SET_SCENEPOLL_ACK                        6104    //

/*获取IPC车流量检测器配置*/
#define EV_GET_IPC_FLOW_CFG_REQ                     6201    //u32 dwSceneId
#define EV_GET_IPC_FLOW_CFG_ACK                     6202    //TIPC_GetVidLoopFlowCfgRsp
/*设置IPC车流量检测器配置*/
#define EV_SET_IPC_FLOW_CFG_REQ                     6203    //TIPC_SetVidLoopFlowCfgReq
#define EV_SET_IPC_FLOW_CFG_ACK                     6204    //NULL
/*车流量信息通知 相机->客户端*/
#define EV_IPC_FLOWINFO_NTY                         6205    //TIPC_VehFlowInfoNty

/*设置违法字典请求,消息内容为TIPC_IllegalDict*/
#define EV_SET_ILLEGAL_CODE_LIST_REQ				6206
/*设置违法字典响应，消息内容为空*/
#define EV_SET_ILLEGAL_CODE_LIST_ACK				6207

/*获取违法字典请求,消息内容为空*/
#define EV_GET_ILLEGAL_CODE_LIST_REQ				6208
/*获取违法字典响应，消息内容TIPC_IllegalDict*/
#define EV_GET_ILLEGAL_CODE_LIST_ACK				6209

/*车流量信息请求通知 相机->客户端*/
#define EV_IPC_FLOWINFO_REQ                         6210    //u32 dwSceneId (ask nty only client)
/*车流量统计信息通知 相机->客户端/流量平台*/
#define EV_IPC_FLOWSTAT_REQ                         6211    //u32 dwSceneId(ask nty only client)
#define EV_IPC_FLOWSTAT_NTY                         6212    //TIPC_VehFlowStatInfoNty

/************************通用消息类型***********************/
/*心跳*/
#define EV_COMM_HEARTBEAT                           10000
/*实时过车消息*/
#define EV_COMM_RT_VEHICLE_INFO                     10001
/*历史过车消息*/
#define EV_COMM_HISTORY_VEHICLE_INFO                10002
/*获取状态信息*/
#define EV_COMM_STATUS_INFO                         10003
/*发送历史数据命令*/
#define EV_COMM_START_SEND_HISTORY_DATA             10004
/*停止发送历史数据命令*/
#define EV_COMM_STOP_SEND_HISTORY_DATA              10005
/*历史数据上传完毕通知*/
#define EV_COMM_UPLOAD_FINISHED_NTY                 10006
/*发送开始采集视频数据命令
参数：TVideoCapture
*/
#define EV_COMM_START_CAPTURE_VIDEO                 10007
/*发送停止采集视频数据命令
参数：TVideoCapture
*/
#define EV_COMM_STOP_CAPTURE_VIDEO                  10008
/*实时视频数据
参数：
*/
#define EV_COMM_REAL_TIME_VIDEO                     10009
/*搜索局域网内设备请求
参数：
*/
#define EV_COMM_DEV_DISCOVERY_REQ                   10010
/*搜索局域网内设备响应
参数：
*/
#define EV_COMM_DEV_DISCOVERY_ACK                   10011
/*前端入网注册请求
参数：TLoginReq
*/
#define EV_COMM_REGISTER_REQ                        10012
/*前端入网注册响应
参数：TLoginRsp
*/
#define EV_COMM_REGISTER_ACK                        10013
/*前端重启命令
参数：
*/
#define EV_COMM_DEV_REBOOT_CMD                      10014
/*前端时间同步请求
参数：
*/
#define EV_COMM_TIME_SYNC_REQ                       10015
/*前端时间同步响应
参数：
*/
#define EV_COMM_TIME_SYNC_ACK                       10016
/*前端故障上报
参数：
*/
#define EV_COMM_DEV_BREAKDOWN_NTY                   10017
/*交通参数信息上报
参数：
*/
#define EV_COMM_TRAFFIC_CFG_NTY                     10018
/*前端下线请求
参数：
*/
#define EV_COMM_DEV_OFFLINE_REQ                     10019
/*前端下线响应
参数：
*/
#define EV_COMM_DEV_OFFLINE_ACK                     10020
/*前端断链通知
参数：u32 dwPeerIp对端IP地址+ u16 wPort
*/
#define EV_COMM_PEER_DISCONNECT_NTY                 10021
/*获取前端设备参数请求
参数：achDevId
*/
#define EV_COMM_GET_DEV_CFG_REQ                     10022
/*获取前端设备参数应答
参数：TCfgInfo
*/
#define EV_COMM_GET_DEV_CFG_ACK                     10023
/*设置前端设备参数请求
参数：TCfgInfo
*/
#define EV_COMM_SET_DEV_CFG_REQ                     10024
/*设置前端设备参数应答
参数：achDevId(64)+retcode(u16)
*/
#define EV_COMM_SET_DEV_CFG_ACK                     10025
/*布控请求
参数：
*/
#define EV_COMM_ARRAGEMENT_REQ                      10026
/*布控响应
参数：
*/
#define EV_COMM_ARRAGEMENT_ACK                      10027
/*撤控请求
参数：
*/
#define EV_COMM_REMOVE_ARRAGEMENT_REQ               10028
/*撤控响应
参数：
*/
#define EV_COMM_REMOVE_ARRAGEMENT_ACK               10029
/*获取卡口列表请求
参数：
*/
#define EV_COMM_GET_BAY0NET_LIST_REQ                10030
/*获取卡口列表响应
参数：
*/
#define EV_COMM_GET_BAY0NET_LIST_ACK                10031
/*查询设备工作状态请求
参数：
*/
#define EV_COMM_GET_DEV_STATE_REQ                   10032
/*查询设备工作状态响应
参数：
*/
#define EV_COMM_GET_DEV_STATE_ACK                   10033
/*报警数据上报
参数：
*/
#define EV_COMM_ALARM_DATA_NTY                      10034
/*订阅卡口命令
参数：
*/
#define EV_COMM_SUBSCRIBE_CMD                       10035
/*取消订阅卡口命令
参数：
*/
#define EV_COMM_SUBSCRIBE_CANCLE_CMD                10036
/*发送通用消息请求*/
#define EV_COMM_SEND_COMMON_REQ                     10037
/*发送通用消息响应*/
#define EV_COMM_SEND_COMMON_ACK                     10038
/*新建连接通知*/
#define EV_COMM_NEW_CONNECT_NTY                     10039
/*车位状态信息*/
#define EV_COMM_PARKING_STATUS                      10040
/*应答控制指令*/
#define EV_COMM_REC_ACK_CTRL                        10041
/*控制指令*/
#define EV_COMM_CONTROL_CMD                         10042
/*应答接收消息*/
#define EV_COMM_REC_RECD_INFO_ACK                   10043
/*客户端设置接收的转发数据类型(同平台接收数据类型)*/
#define EV_COMM_SET_POST_DATA_TYPE_CMD              10044
/*前端断链通知
参数：u32 dwPeerIp对端IP地址+ u16 wPort
*/
#define EV_PEER_DISCONNECT_NTY                      10045
/*接收过车数据反馈结果
参数：TIPC_VechileDataAck
*/
#define EV_COMM_VEHICLE_INFO_DATA_ACK               10046

/*----------------------------------------------------------
 *通用长度定义
 *----------------------------------------------------------*/
 /*设备字符数*/
#define MAX_SIZE_DEV_ID          64
/*用户名字符数*/
#define MAX_SIZE_USRNAME         32
/*密码字符数*/
#define MAX_SIZE_PASSWORD        32
/*IP地址字符数*/
#define MAX_SIZE_STR_IPADDR      32
/*产品型号字符数*/
#define MAX_SIZE_PRODUCT         32
/*厂家字符数*/
#define MAX_SIZE_VENDER          32
/*卡口编号字符数*/
#define MAX_SIZE_POINT_NUM       64
/*车辆行驶方向字符数*/
#define MAX_SIZE_DIRECTION       32
/*车牌号码字符数*/
#define MAX_SIZE_PLATE_NUM       16
/*违法或事件类型字符数*/
#define MAX_SIZE_ILLEGAL_TYPE    64
/*图片张数*/
#define MAX_NUM_IMAGE            8
/*用户名字符数*/
#define MAX_SIZE_LEN8            8

#define MAX_SIZE_LEN16           16

#define MAX_SIZE_LEN32           32

#define MAX_SIZE_LEN64           64

#define MAX_SIZE_LEN512          512

#define MAX_SIZE_PATH_LEN        256

#define MAX_NUM_AUX_CAMERA       3

#define MAX_NUM_SERVER           4

#define MAX_NUM_DEVICE           8

#define MAX_NUM_PER_PAGE         50

#define MAX_CLIENT_LINK_NUM      4

#define MAX_PLAT_LINK_NUM        4
/*图片合成最大张数*/
#define MAX_MERGE_PIC_NUM        8
/*图片合成布局最大布局数*/
#define MAX_MERGE_LAYOUT_NUM     4
/*最大图片合成布局数*/
#define MAX_PIC_MERGE_NUM        5
/*最大osd类型数*/
#define MAX_OSD_TYPE_COUNT       22
/*最大OSD叠加项(行数)*/
#define MAX_OSD_OVERLAY_COUNT    10

/*最大支持8路相机接入*/
#define MAX_CAMERA_LINK_NUM      8
/*最大透明消息长度*/
#define MAX_TRAN_DATA_NUM        1024
/*注册平台的puid*/
#define  MAX_SIZE_PUID           64
/*串口控制协议最大长度*/
#define MAX_CON_PROT_DATA        256
/*最多支持的违法视频个数*/
#define MAX_NUM_VIDEOR_INFO      1
/*违法视频时间*/
#define MAX_LEN_VIDEOR_TIME      24
/*违法视频全路径*/
#define MAX_LEN_VIDEOR_URL       1024
/*字典个数*/
#define MAX_ILLEGAL_NUM          36
/*场景个数*/
#define MAX_SCENE_NUM            4

/************************IPC V7定义***********************/
/*最大逻辑车道数*/
#define LOGIC_ROAD_NUM_MAX                  32
/*单个逻辑车道关联检测器或规则线最大个数*/
#define MAX_DECT_LABEL_NUM                  4
/*车道数*/
#define ROAD_NUM_MAX                        4
/*场景*/
#define SCENE_NUM_SUPPORT_MAX               4
/*检测区域: 指静态场景下的违停检测*/
#define VEH_DET_ROI_NUM_MAX                 4
/*单个区域多边形最大定点个数*/
#define ROI_AREA_POINT_MAX                  8
#define VEH_DET_NUM_MAX                     32

/*线圈检测器开始编号、最大个数*/
#define COIL_LOOP_START_LABEL               1
#define COIL_LOOP_CHANNEL_MAX               16
/*雷达检测器开始编号、最大个数*/
#define RADAR_LOOP_START_LABEL              (COIL_LOOP_START_LABEL+COIL_LOOP_CHANNEL_MAX)
#define RADAR_NUM_MAX                       4
/*视频检测器开始编号、最大个数*/
#define VIDEO_LOOP_START_LABEL              32
#define VIDEO_LOOP_NUM_MAX                  24
/*最大红灯检测器通道数量*/
#define MAX_REDLAMP_DET_NUM                 8

/*最大串口数*/
#define MAX_SERIAL_NUM                      3
/*串口名*/
#define MAX_SERIAL_NAME_LEN                 32

/**/
#define MAX_NUM_SNAPTIMING			        7
/**/
#define MAX_NUM_DECT_TIME					4
/*单车道最大滞留检测器数*/
#define RETENTION_NUM_MAX                   2
/**/
#define MAX_NUM_SCENEPOLL					8


/*固定含义规则线*/
#define FIX_RULE_LINE_START_LABEL           64
#define FIX_RULE_LINE_NUM_MAX               16
/*车道左边线标号*/
#define ROAD_LINE_LEFT_LABEL                (FIX_RULE_LINE_START_LABEL+0)
/*车道右边线标号*/
#define ROAD_LINE_RIGHT_LABEL               (FIX_RULE_LINE_START_LABEL+1)
/*车道12分割线标号*/
#define ROAD_LINE_12_LABEL                  (FIX_RULE_LINE_START_LABEL+2)
/*车道23分割线标号*/
#define ROAD_LINE_23_LABEL                  (FIX_RULE_LINE_START_LABEL+3)
/*车道34分割线标号*/
#define ROAD_LINE_34_LABEL                  (FIX_RULE_LINE_START_LABEL+4)
/*检测线标号*/
#define SCAN_LINE_LABEL                     (FIX_RULE_LINE_START_LABEL+5)
/*左转判定线标号*/
#define TURN_LEFT_LINE_LABEL                (FIX_RULE_LINE_START_LABEL+6)
/*直行判定线标号*/
#define STRAIGHT_LINE_LABEL                 (FIX_RULE_LINE_START_LABEL+7)
/*右转判定线标号*/
#define TURN_RIGHT_LINE_LABEL               (FIX_RULE_LINE_START_LABEL+8)
/*车道1停车线标号*/
#define STOP_LINE_1_LABEL                   (FIX_RULE_LINE_START_LABEL+9)
/*车道2停车线标号*/
#define STOP_LINE_2_LABEL                   (FIX_RULE_LINE_START_LABEL+10)
/*车道3停车线标号*/
#define STOP_LINE_3_LABEL                   (FIX_RULE_LINE_START_LABEL+11)
/*车道4停车线标号*/
#define STOP_LINE_4_LABEL                   (FIX_RULE_LINE_START_LABEL+12)

/*自定义规则线编号、最大个数*/
#define USER_RULE_LINE_START_LABEL          (FIX_RULE_LINE_START_LABEL+FIX_RULE_LINE_NUM_MAX)
#define USER_RULE_LINE_NUM_MAX               16

/*视频检测、线圈检测、雷达检测、视频检测+雷达测速*/
#define DETECT_MODE_VIDEO                    (0x0001<<1)
#define DETECT_MODE_COIL                     (0x0001<<2)
#define DETECT_MODE_RADAR                    (0x0001<<3)
#define DETECT_MODE_VIDEO_RADAR              ((0x0001<<4)|(0x0001<<1))

////////////////////////////具体检测方法/////////////////////////////////////
/*运动检测*/
#define DETECT_METHOD_GRAY                   0
/*号牌检测*/
#define DETECT_METHOD_PLATE                  1
/*尾灯检测*/
#define DETECT_METHOD_TAILLAMP               2
/*头灯检测*/
#define DETECT_METHOD_HEADLAMP               3
/*简单运动检测*/
#define DETECT_METHOD_GRAY_SIMPLE            4
/*红灯检测*/
#define DETECT_METHOD_REDLAMP                5
/*车检器检测*/
#define DETECT_METHOD_COIL                   6
/*雷达检测器*/
#define DETECT_METHOD_RADAR                  7
/*外部触发*/
#define DETECT_METHOD_EXTERNAL               8
/*IO信号检测*/
#define DETECT_METHOD_IO                     9
/*滞留检测*/
#define DETECT_METHOD_RETENTION              10
/*无效检测*/
#define DETECT_METHOD_INVALID                255
/*方向: 1--车头、2--车尾、4--右转、8--直行、16--左转*/
#define DETECT_HEAD_VIEW                     (0x1<<0)
#define DETECT_TAIL_VIEW                     (0x1<<1)
/*红灯相位: 右转*/
#define REDLAMP_PHASE_TURNRIGHT               (0x1<<2)
/*红灯相位: 直行*/
#define REDLAMP_PHASE_STRA_AHEAD              (0x1<<3)
/*红灯相位: 左转*/
#define REDLAMP_PHASE_TURNLEFT               (0x1<<4)

/*行驶方向: 1--来向行驶、2--去向行驶、3--双向混行*/
#define MOVE_DIR_LX                          0x1
#define MOVE_DIR_QX                          0x2
#define MOVE_DIR_SX                          0x3

/*车道属性定义: */
/*公交车专用车道*/
#define BUS_LANE                             (1<<4)
/*左待转车道*/
#define TURN_LEFT_WAIT                       (1<<3)
/*允许左转*/
#define TURN_LEFT                            (1<<2)
/*允许直行*/
#define GO_STRAGIHT                          (1<<1)
/*允许右转*/
#define TURN_RIGHT                           (1<<0)

/*号牌角度(1: 龙门架(C杆); 2: A杆，摄像左牌照右; 3: A杆，摄像右牌照左; 4: 角度方向不定)*/
#define PLATE_ANGLE_C                        0x01
#define PLATE_ANGLE_A_R                      0x02
#define PLATE_ANGLE_A_L                      0X03
#define PLATE_ANGLE_UN                       0x04
/*5: A杆，摄像左牌照右严重畸形*/
#define PLATE_ANGLE_A_R_R                    0x05
#define PLATE_ANGLE_A_L_L                    0x06
#define PLATE_ANGLE_A_UN_UN                  0x07

////////////////逻辑车道类型 &&  抓拍类型////////////////////////////////////////////
/*卡口*/
#define LOGIC_ROAD_TYPE_KK                  (0x0001<<0)
/*超速*/
#define LOGIC_ROAD_TYPE_CS                  (0x0001<<1)
/*逆行: 延时抓拍逻辑(车头卡口场景)*/
#define LOGIC_ROAD_TYPE_NX                  (0x0001<<2)
/*大车占道*/
#define LOGIC_ROAD_TYPE_ZD                  (0x0001<<3)
/*机占非&占用应急车道*/
#define LOGIC_ROAD_TYPE_YJ                  (0x0001<<4)
/*压线: 延时抓拍逻辑(车头卡口场景)*/
#define LOGIC_ROAD_TYPE_YX                  (0x0001<<5)
/*实线变道*/
#define LOGIC_ROAD_TYPE_BD                  (0x0001<<6)
/*占用公交车道*/
#define LOGIC_ROAD_TYPE_GJ                  (0x0001<<7)
/*闯红灯*/
#define LOGIC_ROAD_TYPE_HD                  (0x0001<<8)
/*不按导向*/
#define LOGIC_ROAD_TYPE_DX                  (0x0001<<9)
/*禁止左转*/
#define LOGIC_ROAD_TYPE_JZ                  (0x0001<<10)
/*违章停车*/
#define LOGIC_ROAD_TYPE_WT                  (0x0001<<11)
/*逆行: 视频跟踪逻辑(车尾电警场景)*/
#define LOGIC_ROAD_TYPE_NX_2                (0x0001<<12)
/*压线: 视频跟踪逻辑(车尾电警场景)*/
#define LOGIC_ROAD_TYPE_YX_2                (0x0001<<13)

////////////////////////补光方式////////////////////////////////////////
/*闪光灯补光模式*/
#define FLASH_LIGHT_MODE                    (0x0001<<0)
/*LED灯补光模式*/
#define LED_LIGHT_MODE                      (0x0001<<1)
/*无补光，视频流截图模式*/
#define NONE_LIGHT_MODE                     (0x0000)
/*正常闪*/
#define FLASH_LIGHT_NIGHT_MODE_1            (0x0000)
/*交替闪*/
#define FLASH_LIGHT_NIGHT_MODE_2            (0x0001)

/*电源同步 相位表最大个数*/
#define MAX_PHASE_NUM                       8

////////////////////////图片质量////////////////////////////////////////
/*手动曝光*/
#define KISP_2A_AE_MANU                     (0x1<<0)
/*自动曝光*/
#define KISP_2A_AE_AUTO_1                   (0x1<<1)
/*ISP内置AE*/
#define KISP_2A_AE_AUTO_2                   (0x1<<2)
/*锁定*/
#define KISP_2A_AE_HOLD                     (0x1<<3)
/*Invalid*/
#define KISP_2A_AE_INVALID                  (0x1<<4)

/*手动*/
#define KISP_2A_AWB_MANU                    (0x1<<0)
/*自动白平衡*/
#define KISP_2A_AWB_AUTO_1                  (0x1<<1)
/*ISP内置AWB*/
#define KISP_2A_AWB_AUTO_2                  (0x1<<2)
/*锁定: */
#define KISP_2A_AWB_HOLD                    (0x1<<3)
/*Invalid*/
#define KISP_2A_AWB_INVALID                 (0x1<<4)

/*LED灯自动控制: 自动*/
#define KISP_LED_AUTO                       (0x1<<0)
/*LED灯开启*/
#define KISP_LED_ON                         (0x1<<1)
/*LED灯关闭*/
#define KISP_LED_OFF                        (0x1<<2)
/*UnKnown*/
#define KISP_LED_UNKNOWN                    (0x1<<3)

/*PL偏光镜控制: 自动*/
#define KISP_PL_AUTO                        (0x1<<0)
/*PL开启*/
#define KISP_PL_ON                          (0x1<<1)
/*PL关闭*/
#define KISP_PL_OFF                         (0x1<<2)
/*Unknown*/
#define KISP_PL_UNKNOWN                     (0x1<<3)

/*违法最多数*/
#define MAX_NUM_ILLEGAL_TYPE				32


////////////////////////////公安视图库/////////////////////////////////////
/*字符串最长的长度*/
#define STRING_MAX_LENGTH_128  				129
/*字符串最长的长度*/
#define STRING_MAX_LENGTH_256  				257
/*字符串最长的长度*/
#define STRING_MAX_LENGTH_100  				101
/*字符串最长的长度*/
#define STRING_MAX_LENGTH_1024  			1024
/*设备ID的编码长度*/
#define STRING_LENGTH_DEVID  				20

////////////////////////////银江平台/////////////////////////////////////
/*卡口编号最长的长度*/
#define YJ_MAX_LEN_BAYNOTID					12
/*卡口编号最长的长度*/
#define YJ_MAX_LEN_DIRECTION				2

/*---------------------------------------------------------
 *枚举      (通用枚举   +   协议枚举   +  业务枚举)
 *---------------------------------------------------------*/
/**************************通用枚举************************/

/*车辆类型编码*/
typedef enum
{
    /*未识别*/
    VEHICLE_TYPE_UNKNOWN    = 50000,
    /*小型车*/
    VEHICLE_TYPE_SMALL_SIZE = 50001,
    /*大型车*/
    VEHICLE_TYPE_BIG_SIZE   = 50002,
    /*中型车*/
    VEHICLE_TYPE_MID_SIZE   = 50003,
    /*摩托车*/
    VEHICLE_TYPE_MOTOR      = 50004,
    /*行人*/
    VEHICLE_TYPE_PEOPLE     = 50005,
    /*非机动车*/
    VEHICLE_TYPE_NON_MOTOR  = 50006,
    /*轿车*/
    VEHICLE_TYPE_JC         = 50007,
    /*面包车*/
    VEHICLE_TYPE_MBC        = 50008,
    /*客车*/
    VEHICLE_TYPE_KC         = 50009,
    /*小货车*/
    VEHICLE_TYPE_XHC        = 50010,
    /*大货车*/
    VEHICLE_TYPE_DHC        = 50011,
}EVehicleType;

/*车牌颜色*/
typedef enum
{
    /*未识别*/
    PLATE_COLOR_UNKNOWN = 40000,
    /*黑*/
    PLATE_COLOR_BLACK   = 40001,
    /*蓝*/
    PLATE_COLOR_BLUE    = 40002,
    /*白*/
    PLATE_COLOR_WHITE   = 40003,
    /*黄*/
    PLATE_COLOR_YELLOW  = 40004,
    /*绿*/
    PLATE_COLOR_GREEN   = 40005,
	/*黄绿*/
    PLATE_COLOR_YELLOW_GREEN   = 40006
}EPlateColor;

/*车身颜色编码*/
typedef enum
{
    /*A白*/
    VEHICLE_COLOR_WHITE     = 0x41,
    /*灰*/
    VEHICLE_COLOR_GRAY      = 0x42,
    /*C黄*/
    VEHICLE_COLOR_YELLOW    = 0x43,
    /*D粉*/
    VEHICLE_COLOR_PINK      = 0x44,
    /*E红*/
    VEHICLE_COLOR_RED       = 0x45,
    /*F紫*/
    VEHICLE_COLOR_purple    = 0x46,
    /*G绿*/
    VEHICLE_COLOR_GREEN     = 0x47,
    /*H蓝*/
    VEHICLE_COLOR_BLUE      = 0x48,
    /*I棕*/
    VEHICLE_COLOR_BROWN     = 0x49,
    /*J黑*/
    VEHICLE_COLOR_BLACK     = 0x4A,
	/*K橙*/
	VEHICLE_COLOR_ORANGE         = 0x4B,
	/*L青*/
	VEHICLE_COLOR_INDIGO         = 0x4C,
	/*M银*/
	VEHICLE_COLOR_SILVERY        = 0x4D,
	/*N银白*/
	VEHICLE_COLOR_SILVERY_WHITE  = 0x4E,
    /*Z其他*/
    VEHICLE_COLOR_OTHER     = 0x5A
}EVehicleColor;

/*违法类型编码定义如下：*/
typedef enum
{
    /*超速*/
    ILLEGAL_TYPE_OVER_SPEED         = 10001,
    /*逆行*/
    ILLEGAL_TYPE_RETROGRADE         = 10002,
    /*闯红灯*/
    ILLEGAL_TYPE_THROUGHT_RED_RIGHT = 10003,
    /*压黄线*/
    ILLEGAL_TYPE_OVER_YELLOW_LINE   = 10004,
    /*不按车道行驶*/
    ILLEGAL_TYPE_NOT_CORRECT_LANE   = 10005,
    /*区间超速*/
    ILLEGAL_TYPE_OVER_LINE          = 10006,
    /*压实线 */
    ILLEGAL_TYPE_SOLIDLINE          = 10007,
    /*违章停车*/
    ILLEGAL_TYPE_STOP               = 10008,
    /*无车牌，用于高速卡口项目*/
    ILLEGAL_TYPE_NOPLATE            = 10009,
    /*闯禁令 */
    ILLEGAL_TYPE_BAN_VIOLATION      = 10010,
	/* 违反交通禁令标志*/
    ILLEGAL_TYPE_TRAFFIC_SIGNS      = 10011,
	/* 越线停车*/
    ILLEGAL_TYPE_LINE_STOP          = 10012,
	/* 占用专用车道*/
    ILLEGAL_TYPE_OCCU_LANE          = 10013,
	/*违法掉头 */
    ILLEGAL_TYPE_ILLEGA_TURN        = 10014,
	/* 机动车占道*/
    ILLEGAL_TYPE_MOTOR_LANE         = 10015,
	/*实线变道 */
    ILLEGAL_TYPE_SOLID_LINE         = 10016,
	/*压线行驶 */
    ILLEGAL_TYPE_LINE_DRIVE         = 10017,
	/*主驾未系安全带 */
    ILLEGAL_TYPE_SAFETY_BELT        = 10018,
	/*副驾未系安全带 */
    ILLEGAL_TYPE_SEC_SAFETY_BELT    = 10019,
	/*主驾开车打电话 */
    ILLEGAL_TYPE_MAKE_PHONE         = 10020,
	/*渣土车不按规定行驶 */
    ILLEGAL_TYPE_MUCK_CARS          = 10021,
	/*违章停车短信通知 */
    ILLEGAL_TYPE_STOP_MESSAGE       = 10022,

}EIllegalType;

/*车牌类型*/
typedef enum
{
	/*大型汽车号牌*/      
	PLATE_TYPE_BIG_CAR				=1,
	/*小型汽车号牌*/      
	PLATE_TYPE_SMALL_CAR			=2,
	/*使馆汽车号牌*/      
	PLATE_TYPE_EMBASSY_CAR			=3,
	/*领馆汽车号牌*/      
	PLATE_TYPE_CONSULATE_CAR		=4,
	/*境外汽车号牌*/      
	PLATE_TYPE_ABOARD				=5,
	/*外籍汽车号牌*/      
	PLATE_TYPE_FOREIGN				=6,
	/*两、三轮摩托车号牌*/    
	PLATE_TYPE_MOTORCYCLE			=7,
	/*轻便摩托车号牌*/    
	PLATE_TYPE_SMALL_MOTORCYCLE		=8,
	/*使馆摩托车号牌*/    
	PLATE_TYPE_EMBASSY_MOTOR		=9,
	/*领馆摩托车号牌*/    
	PLATE_TYPE_CONSULATE_MOTOR		=10,
	/*境外摩托车号牌*/    
	PLATE_TYPE_ABOARD_MOTOR			=11,
	/*外籍摩托车号牌*/    
	PLATE_TYPE_FOREIGN_MOTOR		=12,
	/*农用运输车号牌*/    
	PLATE_TYPE_FARMING				=13,
	/*拖拉机号牌*/        
	PLATE_TYPE_TRACTOR				=14,
	/*挂车号牌*/          
	PLATE_TYPE_TRAILER				=15,
	/*教练汽车号牌*/      
	PLATE_TYPE_COACH				=16,
	/*教练摩托车号牌*/    
	PLATE_TYPE_COACH_MOTOR			=17,
	/*实验汽车号牌*/      
	PLATE_TYPE_EXPER				=18,
	/*实验摩托车号牌*/    
	PLATE_TYPE_EXPER_MOTOR			=19,
	/*临时入境汽车号牌*/  
	PLATE_TYPE_ENTRY				=20,
	/*临时入境摩托车号牌*/
	PLATE_TYPE_ENTRY_MOTOR			=21,
	/*临时行驶车号牌*/    
	PLATE_TYPE_ENTRY_DRIVING		=22,
	/*警用汽车号牌*/      
	PLATE_TYPE_POLICE				=23,
	/*警用摩托车号牌*/    
	PLATE_TYPE_POLICE_MOTOR			=24,
	/*其他号牌*/          
	PLATE_TYPE_OTHER				=99,

}EPlateType;

typedef enum
{
	/*前端设备*/
    DEVICE_TYPE_FRONT_DEV   = 0,
	/*接入服务器*/
    DEVICE_TYPE_CLIENT      = 1,
	/*客户端界面*/
	DEVICE_TYPE_UI			= 2,
	/*其他*/
    DEVICE_TYPE_NUKOWN
}EDeviceType;

typedef enum
{
	ENUM_LINK_TYPE_SERVER         = 1,
	ENUM_LINK_TYPE_CLIENT         = 2,
}EIVHSLINKTYPE;

/*图片类型*/
typedef enum
{
    /*车头清晰全景图*/
    VEH_IMAGE_TYPE_FRONT_PANORAMA = 2,
    /*车头车牌号码小图片*/
    VEH_IMAGE_TYPE_FRONT_PLANTE   = 3,
    /*车尾清晰全景图*/
    VEH_IMAGE_TYPE_BACK_PANORAMA  = 5,
    /*车尾车牌号码小图片*/
    VEH_IMAGE_TYPE_BACK_PLANTE    = 6,
    /*人脸小图片*/
    VEH_IMAGE_TYPE_FACE           = 7,
    /*违法图片*/
    VEH_IMAGE_TYPE_ILLEGAL        = 11,
    /*事件图片*/
    VEH_IMAGE_TYPE_EVENT          = 12,
    /*车辆特写图片*/
    VEH_IMAGE_TYPE_VEHICLE        = 13,
    /*合成图片*/
    VEH_IMAGE_TYPE_MONTAGE        = 128,
    /*调试图片*/
    VEH_IMAGE_TYPE_MJPEG_DEBUG    = 255
}EImageType;

/*上传数据类型(平台上传)*/
typedef enum
{
    EM_DATA_TYPE_KAKOU_ORIGINAL_PIC     = 0x0001,   //卡口原图
    EM_DATA_TYPE_KAKOU_MERGE_PIC        = 0x0002,   //卡口合成图片
    EM_DATA_TYPE_ILLEGAL_ORIGINAL_PIC   = 0x0004,   //违法原图
    EM_DATA_TYPE_ILLEGAL_MERGE_PIC      = 0x0008,   //违法合成图片
    EM_DATA_TYPE_FACE_FEATURE           = 0x0010,   //人脸特写
    EM_DATA_TYPE_VEHICLE_FEATURE        = 0x0020,   //车辆特写
    EM_DATA_TYPE_PLATE_FEATURE          = 0x0040,   //号牌特写
    EM_DATA_TYPE_ILLEGAL_VIDEO          = 0x0080,   //违法录像
    EM_DATA_TYPE_VEHICLE_INFO           = 0x0100,   //过车信息
    EM_DATA_TYPE_NUKOWN
}EITS_DATA_TYPE;


/*车辆种类*/
typedef enum
{
	/*未识别*/
	EM_VEHICLE_CLASS_UNKWON			= 150000,
	/*汽车*/
	EM_VEHICLE_CLASS_CAR			= 150001,
	/*挂车*/
	EM_VEHICLE_CLASS_TRAILER		= 150002,
	/*摩托车*/
	EM_VEHICLE_CLASS_MOTOR			= 150003,

}EVehicleClass;


/*车辆子类型定义*/
typedef enum
{
	/*未识别*/
	EM_VEHICLE_SUB_CLASS_UNKWON			    = 160000,
	/*轿车*/
	EM_VEHICLE_SUB_CLASS_CAR		        = 160001,
	/*客车*/
	EM_VEHICLE_SUB_CLASS_BUS		        = 150002,
	/*面包车*/
	EM_VEHICLE_SUB_CLASS_VAN			    = 150003,
	/*重型货车*/
	EM_VEHICLE_SUB_CLASS_HEAVY_TRUCK		= 160004,
	/*轻微型货车*/
	EM_VEHICLE_SUB_CLASS_LIGHT_TRUCK		= 150005,
	/*中型客车*/
	EM_VEHICLE_SUB_CLASS_MEDIUM_BUS			= 150006,
	/*SUV_MPV*/
	EM_VEHICLE_SUB_CLASS_SUV_MPV			= 160007,

}EVehicleSubClass;

/**************************协议枚举************************/
typedef enum
{
    FUNCTION_SUPPORT_TYPE_CLIENT        = 0x0001,   //客户端
    FUNCTION_SUPPORT_TYPE_WEBSERVICE    = 0x0002,   //客户端webserivce
    FUNCTION_SUPPORT_TYPE_HTTP          = 0x0004,   //客户端HTTP
    FUNCTION_SUPPORT_TYPE_FTP           = 0x0008,   //客户端FTP
    FUNCTION_SUPPORT_TYPE_IPC_ACCESS    = 0x0010,   //ipc接入
    FUNCTION_SUPPORT_TYPE_PIC           = 0x0020,   //图片转发
    FUNCTION_SUPPORT_TYPE_NULL
}EFUNCTION_SUPPORT;

typedef enum
{
    /*空闲状态*/
    LINK_STATE_NONE             = 0,
    /*正在建立状态 */
    LINK_STATE_CONNECTING       = 1,
    /*已经建立状态*/
    LINK_STATE_CONNECTED        = 2,
    /*正在注册状态*/
    LINK_STATE_REGISTRERING     = 3,
    /*会话状态*/
    LINK_STATE_SSN              = 4,
    /*暂停推送*/
    LINK_STATE_SUSPEND          = 5,
    /*关闭状态*/
    LINK_STATE_CLOSE            = 6,
}ELinkState;

enum PROTOCOL_COMM_TYPE
{
    PROTOCOL_TYPE_KEDA      = 1,            //苏州科达模式
    PROTOCOL_TYPE_YUSHI     = 2,            //宇视模式
    PROTOCOL_TYPE_FTP       = 3,            //FTP
	PROTOCOL_TYPE_HAIKANG   = 4,            //海康模式
	PROTOCOL_TYPE_FENGHUO   = 5,            //烽火模式
	PROTOCOL_TYPE_VID_IMG	= 6,            //公安视图库,视频图像信息模式
	PROTOCOL_TYPE_YINJIANG	= 7,            //银江平台
	PROTOCOL_TYPE_BENNENG	= 8,            //本能平台
	PROTOCOL_TYPE_TIANWANG	= 9,            //重庆天网平台
	PROTOCOL_TYPE_HISENSER	= 10,           //海信平台
    PROTOCOL_TYPE_NONE      = 55            //定义为无	
};

enum HISDATA_TRANS_MODE
{
	HISDATA_TRANS_ALWAYS = 0,
	HISDATA_TRANS_ASK	= 1,
	HISDATA_TRANS_NO = 2,
};

/**************************业务枚举：ITS图片配置************************/
// osd 类型
enum emOSDType
{
    EM_OSD_TYPE_ALL,        //全部事件
    EM_OSD_TYPE_PLATE,      //车牌
    EM_OSD_TYPE_PLATECOLOR, //车牌颜色
    EM_OSD_TYPE_BODYCOLOR,  //车身颜色
    EM_OSD_TYPE_BRAND,      //车标
    EM_OSD_TYPE_DATETIME,   //时间
    EM_OSD_TYPE_MS,         //毫秒
    EM_OSD_TYPE_SCENE,      //地点
};

enum ENUM_MERGE_TYPE
{
    enum_merge_type_none        =0,
    enum_merge_type_twoinone,
    enum_merge_type_threeinone,
    enum_merge_type_fourinone,
    enum_merge_type_fiveinone,
    enum_merge_type_sixinone,
    enum_merge_type_sixintwo,
};

enum ENUM_IMAGE_SOURCE_TYPE
{
    enum_image_source_none = 0,
    enum_image_source_panorama,    //全景图片
    enum_image_source_feature,     //特写图片
};

enum ENUM_FEATURE_POSITION
{
    enum_feature_pos_unkown = 0,
    enum_feature_pos_ul     = 1,
    enum_feature_pos_ur     = 2,
    enum_feature_pos_ll     = 3,
    enum_feature_pos_lr     = 4,
};

enum ENUM_IMAGE_SIZE
{
    enum_image_size_unkown   = 0,
    enum_image_size_original = 1,
    enum_image_size_half     = 2,
    enum_image_size_quarter  = 4,
};

enum ENUM_IMAGE_STORE_TYPE
{
    enum_image_store_type_rt    = 1,
    enum_image_store_type_local = 2,
};

/*************************业务枚举:ITS平台、相机配置**********************/
enum PLATFORM_SERVER_TYPE
{
    enum_platform_type_kedacom = 1,
    enum_platform_type_shiyue  = 2,
    enum_platform_type_ftp     = 3,
};

enum CAMERA_TYPE
{
    enum_camera_kedacom = 1,
    enum_camera_ruishi  = 2,
};

enum CAMERA_MATCH_MODE_TYPE
{
    enum_match_mode_type_capture        = 1,    //前后抓拍
    enum_match_mode_type_epolice        = 2,    //人像电警
    enum_match_mode_type_illegal_uturn  = 3,    //违法掉头
    enum_match_mode_type_speed_measure  = 4,    //区间测速
};

/*************************业务枚举:ITS录像配置***********************/
enum VIDEO_SECTION_TYPE
{
    enum_video_section_type_illegal     = 1,    //违法截取
    enum_video_section_type_unkown
};

/*************************业务枚举:ITS查询***************************/
enum VEHICLE_QUERY_TYPE
{
    VEHICLE_QUERY_TYPE_NULL         = 0x0,
    VEHICLE_QUERY_TYPE_START_TIME   = 0x0001,   //开始时间
    VEHICLE_QUERY_TYPE_STOP_TIME    = 0x0002,   //结束时间
    VEHICLE_QUERY_TYPE_TYPE         = 0x0004,   //数据类型
    VEHICLE_QUERY_TYPE_IPCNO        = 0x0008,   //相机编号
    VEHICLE_QUERY_TYPE_PLATE        = 0x0010,   //车牌
    VEHICLE_QUERY_TYPE_STATE        = 0x0020,   //上传状态
    VEHICLE_QUERY_TYPE_MIN_SPEED    = 0x0040,   //最小车速
    VEHICLE_QUERY_TYPE_MAX_SPEED    = 0x0080,   //最大车速
    VEHICLE_QUERY_TYPE_ASC          = 0x0100,   //升降序
};

enum QUERY_DATA_TYPE
{
    enum_data_type_all_kakou = 1,
    enum_data_type_all_illegal,
    enum_data_type_run_red_light,
    enum_data_type_overspeed,
    enum_data_type_retrograde,
    enum_data_type_linedriving,
    enum_data_type_illegal_parking,
    enum_data_type_change_lanes,
    enum_data_type_illegal_prohibit,
    enum_data_type_occupancy_speciallane,
    enum_data_type_inconsistent_plate,
    enum_data_type_congestion,
    enum_data_type_vehicle_occupancy_lane,
    enum_data_type_run_green_light,
    enum_data_type_vehicle_uturn,
    enum_data_type_speed_detection,
};

enum QUERY_UPLOAD_STATE
{
    enum_query_upload_state_finish   = 1,
    enum_query_upload_state_unfinish = 2,
};

enum QUERY_SORT_TYPE
{
    enum_query_sort_time_asc    = 1,
    enum_query_sort_time_des    = 2,
    enum_query_sort_no_asc      = 3,
    enum_query_sort_no_des      = 4,
};
/*************************业务枚举:ipc v7***********************/
/*逻辑车道 抓拍模式*/
enum SNAP_CTRL_MODE
{
    enum_snap_ctrl_mode_by_time     = 1,
    enum_snap_ctrl_mode_by_distance = 2,
};

enum PTZ_EVENT_TYPE
{
    enum_ptz_event_type_preset      = 1,    //->p1
    enum_ptz_event_type_patrol      = 2,    //p1<-->p2
    enum_ptz_evnet_type_unkown
};

enum DRIVE_DIR_TYPE
{
    DRIVE_DIR_TYPE_LX                = 0x1,    //来向行驶
    DRIVE_DIR_TYPE_QX                = 0x2,    //去向行驶
    DRIVE_DIR_TYPE_SX                = 0x3,    //双向混行
    DRIVE_DIR_TYPE_UNKOWN
};

/*码流模式*/
enum E_STREAM_MODE
{
    ENUM_STREAM_MODE_JPEG   = 1,
    ENUM_STREAM_MODE_UNKOWN
};

/*串口协议类型*/
typedef enum
{
    /*雷达CSR-IN协议*/
    E_RADAR_CSR_IN = 1,
    /*泓鎏红灯车检模块V2协议*/
    E_HL_DET_MODULE_V2,
    /*雷达802-2D协议*/
    E_RADAR_802_2D,
    /*泓鎏红灯车检模块V1协议*/
    E_HL_DET_MODULE_V1,
    /*宇视车检器协议*/
    E_UNIVIEW_DET_MODULE,
    /*无*/
    E_NONE = 255,
}ESerialProtocol;

typedef enum
{
    E_TRAFFIC_STATE_CLEAR   = 0,
    E_TRAFFIC_STATE_SLOW    = 1,
    E_TRAFFIC_STATE_BUSY    = 2,
}ETrafficState;

typedef enum
{
    /*不更新*/
    E_VEHFLOW_UPDATE_TYPE_NEVER     = 0,
    /*未来时间点, 单位MM:HH:MM 如12:00:00*/
    E_VEHFLOW_UPDATE_TYPE_BY_POINT  = 1,
    /*时间周期 如当前时间2:30后*/
    E_VEHFLOW_UPDATE_TYPE_BY_PERIOD = 2,
    /*计数更新 达到规定流量值 如1000*/
    E_VEHFLOW_UPDATE_TYPE_BY_FLOWCNT= 3,
}EFlowUpdateType;

typedef enum
{
    E_UPDATE_PERIOD_TYPE_SEC   = 0,
    E_UPDATE_PERIOD_TYPE_MIN   = 1, 
    E_UPDATE_PERIOD_TYPE_HOUR  = 2,
    E_UPDATE_PERIOD_TYPE_DAY   = 3,
    E_UPDATE_PERIOD_TYPE_MONTH = 4,
    E_UPDATE_PERIOD_TYPE_YEAR  = 5,
    E_UPDATE_PERIOD_TYPE_COUNT
}EFlowPeriodType;

//UI能力集定义
enum EMUI_CAP_TYPE
{
    EMUI_CAP_TYPE_SCENE = 0x0,      // 场景配置 0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_DETECTION,        // 检测配置 0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_LOGICLANE,        // 逻辑车道 0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_IDENTPARAM,       // 识别参数 0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_VELOMETER,        // 视频测速 0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_IMAGEQUALITY,     // 图像品质 0:不支持, 1:全部支持;2，二代版本，仅显示曝光的号牌测光 
    EMUI_CAP_TYPE_SYSPICMERGE,      // 图片合成 0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_OSD,              // OSD      0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_REDLIGHT,         // 红灯测量 0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_POWERSYN,         // 电源同步 0:不支持, 1:全部支持; 
    EMUI_CAP_TYPE_SERIAL,           // 串口信息 0:不支持, 1:全部支持
    EMUI_CAP_TYPE_REGISTERPLAT,     // 注册平台 0:不支持，1:全部支持; 
	EMUI_CAP_TYPE_ILLGPARK,		    // 违停配置 0:不支持，1:全部支持; 
	EMUI_CAP_TYPE_FLOWDET,			// 视频流量检测 0:不支持，1:全部支持; 
    EMUI_CAP_TYPE_COUNT
};

typedef enum
{
    /*东向西*/
    E_LANE_DIR_TYPE_EAST2WEST           = 1,
    /*西向东*/
    E_LANE_DIR_TYPE_WEST2EAST           = 2,
    /*南向北*/
    E_LANE_DIR_TYPE_SOUTH2NORTH         = 3,
    /*北向南*/
    E_LANE_DIR_TYPE_NORTH2SOUTH         = 4,
    /*东南向西北*/
    E_LANE_DIR_TYPE_SOUTHEAST2NORTHWEST = 5,
    /*西北向东南*/
    E_LANE_DIR_TYPE_NORTHWEST2SOUTHEAST = 6,
    /*东北向西南*/
    E_LANE_DIR_TYPE_NORTHEAST2SOUTHWEST = 7,
    /*西南向东北*/
    E_LANE_DIR_TYPE_SOUTHWEST2NORTHEAST = 8,
    /*其他*/
    E_LANE_DIR_TYPE_OTHER,
}E_LANE_DIR_TYPE;


typedef enum
{
    /*正常*/
    E_DEV_ERROR_NOTICE_NORMAL          = 1,
    /*异常*/
    E_DEV_ERROR_NOTICE_EXCEPT          = 2,
    /*未知*/
    E_DEV_ERROR_NOTICE_UNKNOW          = 3,
  
}E_DEV_ERROR_NOTICE;

typedef enum
{
    /*检测线圈状态*/
    E_DEV_STATE_LOOP				= 80001,
    /*闪光灯工作状态*/
    E_DEV_STATE_LAMP				= 80002,
    /*机箱温度状态*/
    E_DEV_STATE_TEMPERATURE         = 80003,
	/*机箱门状态*/
    E_DEV_STATE_CASE				= 80004,
    /*主控机工作状态*/
    E_DEV_STATE_ITS_WORKING         = 80005,
  
}E_DEV_STATE;

/*************************公安视图库枚举***********************/

//图片采集方式
enum EInfoKind
{
	EN_INFO_KIND_OTHER		=0,  //其他
	EN_INFO_KIND_AUTOMATIC	=1,  //自动采集
	EN_INFO_KIND_MANUAL	    =2,  //人工采集
};

//图片的格式
enum EFileFormat
{
	EN_FILE_FORMAT_BMP			=1,  //Bmp
	EN_FILE_FORMAT_GIF			=2,  //Gif
	EN_FILE_FORMAT_JPEG			=3,  //Jpeg
	EN_FILE_FORMAT_JFIF			=4,  //Jfif
	EN_FILE_FORMAT_KDC			=5,  //Kdc
	EN_FILE_FORMAT_PCD			=6,  //Pcd
	EN_FILE_FORMAT_PCX			=7,  //Pcx
	EN_FILE_FORMAT_PIC			=8,  //Pic
	EN_FILE_FORMAT_PIX			=9,  //Pix
	EN_FILE_FORMAT_PNG			=10,  //Png
	EN_FILE_FORMAT_PSD			=11,  //Psd
	EN_FILE_FORMAT_TAPGA		=12,  //Tapga
	EN_FILE_FORMAT_TIFF			=13,  //Tiff
	EN_FILE_FORMAT_WMF			=14,  //Wmf
	EN_FILE_FORMAT_JPEG_2000	=15,  //Jp2
	EN_FILE_FORMAT_OTHER		=16,  //Other
};

//图片信息来源
enum EDataSourceType
{
	EN_DATA_SOURCE_TYPE_GOVERNMENT		=1,		//政府机关监控
	EN_DATA_SOURCE_TYPE_SOCIOLOGY		=2,		//社会面治安监控
	EN_DATA_SOURCE_TYPE_TRAFFIC			=3,		//交通监控（含轨道交通监控）
	EN_DATA_SOURCE_TYPE_IMMIGRATION		=4,		//出入境监控
	EN_DATA_SOURCE_TYPE_PORT			=5,		//港口监控
	EN_DATA_SOURCE_TYPE_FINANCIAL		=6,		//金融系统监控
	EN_DATA_SOURCE_TYPE_HOTEL			=7,		//旅馆监控
	EN_DATA_SOURCE_TYPE_WWW				=8,		//互联网营业场所监控
	EN_DATA_SOURCE_TYPE_SERVICE			=9,		//娱乐服务场所监控
	EN_DATA_SOURCE_TYPE_OTHER_COMPANY	=10,	//其它企业/事业单位监控
	EN_DATA_SOURCE_TYPE_RESIDENT		=11,	//居民自建监控
	EN_DATA_SOURCE_TYPE_GA				=12,	//公安内部
	EN_DATA_SOURCE_TYPE_PRISONS			=13,	//监所
	EN_DATA_SOURCE_TYPE_INERROGATION	=14,	//讯问室
	EN_DATA_SOURCE_TYPE_TERMINAL		=15,	//车（船、直升机等）载终端拍摄
	EN_DATA_SOURCE_TYPE_MOBILE			=16,	//移动执法
	EN_DATA_SOURCE_TYPE_PHONE			=17,	//手机、平板电脑拍摄
	EN_DATA_SOURCE_TYPE_DV				=18,	//DV拍摄
	EN_DATA_SOURCE_TYPE_CAMERA			=19,	//相机拍摄
	EN_DATA_SOURCE_TYPE_NETWORK			=20,	//网络获取
	EN_DATA_SOURCE_TYPE_AUDIO_VISUAL	=21,	//声像资料片
	EN_DATA_SOURCE_TYPE_OTHER			=99,	//其他
};

//图片信息来源
enum EEventSort
{
	EN_EVENT_TYPE_OTHER					= 0,	//其它
	EN_EVENT_TYPE_BAYONET_CAR			= 1,   	//卡口过车
	EN_EVENT_TYPE_BAYONET_PEOPLE		= 2,   	//卡口过人
	EN_EVENT_TYPE_BAYONET_FIGHT			= 3,  	//卡口打架
	EN_EVENT_TYPE_BAYONET_RUN			= 4,  	//卡口快速奔跑
	EN_EVENT_TYPE_MOVING_DETECT			= 5,  	//目标检测与特征提取运动目标检测
	EN_EVENT_TYPE_TARGET_CLASS			= 6,  	//目标检测与特征提取,目标分类
	EN_EVENT_TYPE_COLOR_DETECT			= 7,  	//目标检测与特征提取,目标颜色检测
	EN_EVENT_TYPE_PEOPLE_DETECT			= 8,  	//目标检测与特征提取,行人检测
	EN_EVENT_TYPE_PEOPLE_INFO			= 9,  	//目标检测与特征提取,人员属性分析
	EN_EVENT_TYPE_FACE_DETECT			= 10,  	//目标检测与特征提取,人脸检测
	EN_EVENT_TYPE_FACE_COMP				= 11,  	//目标检测与特征提取,人脸比对
	EN_EVENT_TYPE_VEHC_DETECT			= 12,  	//目标检测与特征提取,车辆检测
	EN_EVENT_TYPE_TRAFFIC_STATISTICS	= 14,  	//目标数量分析,流量统计
	EN_EVENT_TYPE_DENSISTY_DETECT		= 15,  	//目标数量分析,密度检测
	EN_EVENT_TYPE_PLATE_INFO			= 16,  	//目标识别,车牌识别
	EN_EVENT_TYPE_VEHC_INFO				= 17,  	//目标识别,车辆基本特征识别
	EN_EVENT_TYPE_VEHC_FEATURE			= 18,  	//目标识别,车辆个体特征识别
	EN_EVENT_TYPE_TRIPWIRE				= 19,  	//行为分析,绊线检测
	EN_EVENT_TYPE_INTRUSION				= 20,  	//行为分析,入侵检测
	EN_EVENT_TYPE_RETROGRADE			= 21,  	//行为分析,逆行检测
	EN_EVENT_TYPE_LINGER				= 22,  	//行为分析,徘徊检测
	EN_EVENT_TYPE_LEGACE				= 23,  	//行为分析,遗留物检测
	EN_EVENT_TYPE_TARGET_REMOVAL		= 24,  	//行为分析,目标移除检测
	EN_EVENT_TYPE_VIDEO_SUMMAR			= 25,  	//视频摘要,视频摘要
	EN_EVENT_TYPE_FOG					= 26,  	//视频增强与复原,去雾
	EN_EVENT_TYPE_VAGUE					= 27,  	//视频增强与复原,去模糊
	EN_EVENT_TYPE_CONTRAST_RATIO		= 28,  	//视频增强与复原,对比度增强
	EN_EVENT_TYPE_LOW_ILLUMINATION		= 29,  	//视频增强与复原,低照度视频图像增强
	EN_EVENT_TYPE_PARTIAL_COLOR			= 30,  	//视频增强与复原,偏色校正
	EN_EVENT_TYPE_WIDE_DYNAMIC			= 31,  	//视频增强与复原,宽动态增强
	EN_EVENT_TYPE_SUPER_RESOLUTION		= 32,  	//视频增强与复原,超分辨率重建
	EN_EVENT_TYPE_GEOMETRY				= 33,  	//视频增强与复原,几何畸变校正
	EN_EVENT_TYPE_PARITY				= 34,  	//视频增强与复原,奇偶场校正
	EN_EVENT_TYPE_COLOR_ROOM			= 35,  	//视频增强与复原,颜色空间分量分离
	EN_EVENT_TYPE_NOISE					= 36,  	//视频增强与复原,去噪声
};

//视频图像信息基本对象的类型
enum EBasicObjectsInfoType
{
	EN_BASIC_OBJECT_INFO_TYPE_VIDEO		=1,		//视频片段
	EN_BASIC_OBJECT_INFO_TYPE_IMAGE		=2,		//图像
	EN_BASIC_OBJECT_INFO_TYPE_FILE		=3,		//文件
	EN_BASIC_OBJECT_INFO_TYPE_OTHER		=99,	//其他
};

//视频图像信息语义属性对象的类型
enum EAttributeInfoType
{
	EN_ATTRIBUTE_INFO_TYPE_PEOPLE		=1,		//人员
	EN_ATTRIBUTE_INFO_TYPE_CAR			=2,		//机动车
	EN_ATTRIBUTE_INFO_TYPE_BICYCLE		=3,		//非机动车
	EN_ATTRIBUTE_INFO_TYPE_GOODS		=4,		//物品
	EN_ATTRIBUTE_INFO_TYPE_SCENE		=5,		//场景
	EN_ATTRIBUTE_INFO_TYPE_FACE			=6,		//人脸
	EN_ATTRIBUTE_INFO_TYPE_TAG			=7,		//视频图像标签
	EN_ATTRIBUTE_INFO_TYPE_OTHER		=99,	//其他
};

enum EColorType
{
	EN_COLOR_TYPE_BALCK			= 1 ,//黑
	EN_COLOR_TYPE_WHITE			= 2 ,//白
	EN_COLOR_TYPE_GRAY			= 3 ,//灰
	EN_COLOR_TYPE_RED			= 4 ,//红
	EN_COLOR_TYPE_BLUE			= 5 ,//蓝
	EN_COLOR_TYPE_YELLOW		= 6 ,//黄
	EN_COLOR_TYPE_ORANGE		= 7 ,//橙
	EN_COLOR_TYPE_BROWN			= 8 ,//棕
	EN_COLOR_TYPE_GREEN			= 9 ,//绿
	EN_COLOR_TYPE_VIOLET		= 10 ,//紫
	EN_COLOR_TYPE_CYAN			= 11 ,//青
	EN_COLOR_TYPE_PINK			= 12 ,//粉
	EN_COLOR_TYPE_TRANSPARENT	= 13 ,//透明
	EN_COLOR_TYPE_OTHER			= 99 ,//其他
};

// #ifdef _MSC_VER
// #pragma pack(pop)
// #endif //WIN32

#pragma pack(pop)
#endif //end of _IVHS_DEFINE_H_
