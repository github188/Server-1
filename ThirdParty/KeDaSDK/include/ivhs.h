#ifndef _IVHS_H_
#define _IVHS_H_

#include "ivhs_struct.h"

#ifdef __cplusplus
extern "C" {

#ifndef API
#ifdef WIN32
#define API   __declspec(dllexport)
#else
#define API
#endif
#endif

#ifndef CDECL
#ifdef WIN32
#define CDECL _cdecl
#else
#define CDECL
#endif
#endif

#else

#ifndef API
#define API
#endif
#ifndef CDECL
#define CDECL
#endif

#endif /* __cplusplus */

/*上传实时图片失败回调
 *输入:
 *      dwSeqNumber:    车辆过车编号
 *      dwIp:           平台对应的ip
 *      wPort:          平台对应的port
 */
typedef void (*IvhsUploadPicCBF)(Uint32 dwSeqNumber, TIvhsSendPicCb tSendPicCb, Uint32 dwMaxPlatNum);

/*消息回调函数定义
 *输入: 
 *      tHead:          消息头
 *      szMsgBody:      消息内容参考以上消息定义注释
 *      dwMsgLen:       消息长度
 */
typedef void (*IvhsMessageEntryCBF)(TItsMessage *ptMsg);

/*设置接收消息回调函数
 *输入:
 *     cbFunMsgEntry:接收消息回调函数
 */
API void CDECL IvhsSetMsgCBFun(IvhsMessageEntryCBF cbFunMsgEntry);

/*设置上传图片失败回调
 *输入:
 *     IvhsUploadPicCBF:接收消息回调函数
 */
API void CDECL IvhsSetUploadPicCBFun(IvhsUploadPicCBF cbFunUploadPicEntry);

/*协议初始化(服务器或ipc调用)
 * 输入: 
 *     TPlatCfgParam        平台配置参数
 *     dwFunctionSupport    协议支持功能
 *     TPlatInfo            平台信息(同IvhsAddPlat)
 * 输出: 0:成功   其他: 失败
 */
API Int32 CDECL  IvhsInit(TIvhsInitParam* ptInitCfg);


/*协议逆初始化(服务器或ipc调用)
 */
API Int32 CDECL IvhsUnInit();


/*主动发送消息
 *输入:
 *     ptMsg:    消息体
 *返回值:
 *     成功:0	   失败:-1
 */
API Int32 CDECL IvhsPostMsg(TItsMessage *ptMsg);

/*转发图片
 *输入:
 *      TVechileData    过车数据(过车信息+图片)
 *      byReal          是否实时过车 1实时; 0历史;
 *      dwIp  wport     0 全转发   其他 特定ip、port对应的转发
 *返回值:	成功:0  失败:-1(平台发送标记)
 */

API Int32 CDECL IvhsPostPic(TVechileData *ptData, Uint8 byReal, Uint32 dwIp/*=0*/, Uint16 wPort/*=0*/);


/*设置平台信息
 *输入: 
 *    TPlatInfo*  平台信息指针
 *    byPlatSize  平台个数
 *返回值: 成功:0  失败:-1
 *说明: 内部会完成平台的添加、删除和更新
 */
API Int32 CDECL IvhsSetPlat(TIvhsPlatInfo* ptInfo, Uint8 byPlatSize);

/*链路信息通知
 *输入: 
 *      szLinkInfo  链路信息 IP:Port
 *      szEState    链路状态
 *返回: LinkSize    链路个数
 *说明: 链路信息上报给业务
 */
API Int32 CDECL IvhsLinkInfoNotify(TIvhsNotifyInfo *pNotifyInfo);

/*设置注册信息
 *输入: 
 *      ptInfo  注册信息
 *返回: null
 *说明: 
 */
API Void CDECL IvhsSetDevInfo(TIvhsDevInfo *ptInfo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_IVHS_H_


