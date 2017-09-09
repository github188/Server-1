#ifndef _IVHS_SDK_H_
#define _IVHS_SDK_H_

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

/*消息回调函数定义
**输入: tHead 消息信息
**      szMsgBody:消息内容参考以上消息定义注释
**      dwMsgLen:消息长度
*/
typedef void (*IvhsMessageEntryCBF)(TItsMessage *ptMsg);

/*协议初始化
**输入:
**      dwMaxNumConnect :最多支持链路总数
**返回值:
**     成功：0 失败：-1
*/
API Int32 CDECL IvhsStart(Uint32 dwMaxNumConnect);

/*退出协议
**返回值:
**     成功：0 失败：-1
*/
API Int32 CDECL IvhsStop();

/*启动监听服务
**输入:
**     dwLocalIp：监听IP，可以默认为0
**     wLocalPort：监听端口
**返回值:
**     成功：0 失败：-1
*/
API Int32 CDECL IvhsStartListen(Uint32 dwLocalIp, Uint16 wLocalPort);

/*停止监听服务 
**返回值:
**     成功：0 失败：-1
*/
API Int32 CDECL IvhsStopListen();

/*主动连接服务端
**输入:
**     dwPeerIp：服务端IP
**     wPeerPort：服务端端口
**返回值:
**     成功：链路句柄 失败：0
*/
API Uint32 CDECL IvhsFwdConnect(Uint32 dwPeerIp, Uint16 wPeerPort);

/*主动断开服务端连接
**输入:
**     dwHandle：链路句柄
**返回值:
**     成功：0 失败：-1
*/
API Int32 CDECL IvhsDisconnect(KOSA_HANDLE dwHandle);

/*主动发送消息
**输入:
**     ptMsg：消息信息
**返回值:
**     成功：0 失败：-1
*/
API Int32 CDECL IvhsPostMsg(TItsMessage *ptMsg);

/*设置接收消息回调函数
**输入:
**     cbFunMsgEntry：接收消息回调函数
*/
API void CDECL IvhsSetMsgCBFun(IvhsMessageEntryCBF cbFunMsgEntry);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_IVHS_SDK_H_


