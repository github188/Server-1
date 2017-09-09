#ifndef __NET_SDK_ERROR_H__
#define __NET_SDK_ERROR_H__ 

/*
**sdk error code
*/	
//0~39 通用错误
#define NET_ERROR_NO_ERROR                      0                   //没有错误                                                  
#define NET_ERROR_VERSION_MISMATCH              1                   //版本不匹配                                                
#define NET_ERROR_CHAN_NOT_SUPPORT              2                   //指定通道不支持该操作                                      
#define NET_ERROR_UNKNOW_ENCRYPT                3                   //未知的加密方式                                            
#define NET_ERROR_SDK_INTER_ERROR               4                   //SDK内部错误                                               
#define NET_ERROR_ALLOC_RESOURCE_ERROR          5                   //资源分配错误                                              
#define NET_ERROR_NOT_SUPPORT                   6                   //SDK不支持该操作                                           
#define NET_ERROR_NO_INIT                       7                   //没有初始化                                                
#define NET_ERROR_PARAM_ERROR                   8                   //输入的参数错误                                            
#define NET_ERROR_INPUT_TIME_ERROR              9                   //时间输入不正确                                            
#define NET_ERROR_INVALID_HANDLE                10                  //无效的句柄                                                
#define NET_ERROR_ORDER_ERROR                   11                  //接口调用次序错误      
#define NET_ERROR_OS_NO_SUPPORT                 12                  //当前系统不支持此功能      
#define NET_ERROR_UNSUPPORT_CONFIG              13                  //不支持的配置命令
#define NET_ERROR_SERVER_DATA_ERROR             14                  //服务端传输的数据有误
#define NET_ERROR_TIME_PARA_ERROR               15                  //时间参数错误
#define NET_ERROR_BUFFER_SIZE_ERROR             16                  //输入的buffer大小有误
#define NET_ERROR_ID_ERROR                      17                  //ID错误, 如网卡ID, 硬盘ID
#define NET_ERROR_PLAY_END                      18                  //回放录像文件已结束
#define NET_ERROR_MAX_SESSION                   19                  //session分配达到最大数目                  

//40~59 网络错误
#define NET_ERROR_BIND_SOCKET_ERROR             40                  //绑定套接字失败                                            
#define NET_ERROR_CLOSE_SOCKET_ERROR            41                  //socket连接中断，此错误通常是由于连接中断或目的地不可达    
#define NET_ERROR_LISTEN_ERROR                  42                  //socket监听失败                                            
#define NET_ERROR_CONNECT_FAIL                  43                  //连接服务器失败                                            
#define NET_ERROR_SEND_FAIL                     44                  //向服务器发送失败                                          
#define NET_ERROR_RECV_FAIL                     45                  //从服务器接收数据失败                                      
#define NET_ERROR_RECV_TIMEOUT                  46                  //从服务器接收数据超时                                      
#define NET_ERROR_NO_ENOUGH_SENDBUF             47                  //发送缓冲区太小                                            
#define NET_ERROR_NO_ENOUGH_RECVBUF             48                  //接收缓冲区太小                                            
#define NET_ERROR_CREATE_SOCKET_ERROR           49                  //创建SOCKET出错                                            
#define NET_ERROR_SET_SOCKET_ERROR              50                  //设置SOCKET参数出错        
#define NET_ERROR_LOCAL_PORT_IN_USE             51                  //本地端口已被占用
#define NET_ERROR_FILE_END                      52                  //文件结束
#define NET_ERROR_RECONNECTING                  53                  //正在重连             

//60~79 文件错误
#define NET_ERROR_CREATE_FILE_FAIL              60                  //创建文件出错                                              
#define NET_ERROR_OPEN_FILE_FAIL                61                  //打开文件出错                                              
#define NET_ERROR_WRITE_FILE_FAIL               62                  //写文件失败                                                
#define NET_ERROR_UNKNOW_FORMAT                 63                  //文件格式不正确                                            
#define NET_ERROR_DIR_ERROR                     64                  //路径错误     
#define NET_ERROR_READ_FILE_FAIL                65                  //读文件失败                                                                                           

//80~99 系统环境相关
#define NET_ERROR_PC_OPEN_SOUND_FAIL            80                  //打开PC声音失败                                            
#define NET_ERROR_AUDIO_MODE_ERROR              81                  //声卡模式错误                                              
#define NET_ERROR_VOICEMONOPOLIZE               82                  //声卡被独占                                                
#define NET_ERROR_LOAD_PLAYSDK_API_FAIL         83                  //调用播放库某个接口失败                                
#define NET_ERROR_LOAD_PLAYSDK_FAIL             84                  //加载播放SDK失败                                           
#define NET_ERROR_PLAYER_FAIL                   85                  //播放库调用错误                                            
#define NET_ERROR_GET_LOCAL_IP_FAIL             86                  //获得客户端的IP地址或物理地址失败                          
#define NET_ERROR_GET_LOCAL_MAC_FAIL            87                  //获得客户端的IP地址或物理地址失败                          
#define NET_ERROR_IP_MISMATCH                   88                  //IP地址不匹配                                              
#define NET_ERROR_MAC_MISMATCH                  89                  //MAC地址不匹配           

//100~130 推模式相关错误
#define NET_ERROR_NO_IPC_REGISTER               101                 //没有IPC主动建立的链接

//500~999   设备端反馈的错误
#define NET_ERROR_OPERATION_ERROR               500                 //设备返回调用失败                                              
#define NET_ERROR_USER_NOT_EXIST                501                 //用户不存在                                                
#define NET_ERROR_PASSWORD_ERROR                502                 //用户名密码错误                                            
#define NET_ERROR_INVALID_SESSION_ID            503                 //无效的会话ID                                              
#define NET_ERROR_MAX_USER_NUM                  504                 //设备端用户数达到最大                                      
#define NET_ERROR_NOSERIALPORT                  505                 //串口号错误                                                
#define NET_ERROR_ALARMIN_ID_ERROR              506                 //无效的报警输入号                                          
#define NET_ERROR_ALARMOUT_ID_ERROR             507                 //无效的报警输出号                                          
#define NET_ERROR_CHANNEL_ID_ERROR              508                 //通道号错误                                                
#define NET_ERROR_NET_ERROR_DATA                509                 //设备反应传送的数据有误                                    
#define NET_ERROR_VOICE_IN_USE                  510                 //服务器语音对讲被占用                                      
#define NET_ERROR_NO_DISK                       511                 //没有硬盘                                                  
#define NET_ERROR_DISK_ID_ERROR                 512                 //硬盘号错误                                                
#define NET_ERROR_NO_RIGHT                      513                 //设备端反应权限不足  
#define NET_ERROR_DISK_IS_FORMATING             514                 //硬盘正在格式化硬盘
#define NET_ERROR_UNKONW_DEVICE_ERROR           515                 //设备返回的未知错误码

//1000~1255 RTSP错误                  
#define NET_ERROR_LOAD_RTSP_SDK_ERROR           1000                //加载RTSP SDK 失败
#define NET_ERROR_RTSP_INVALID_PARAM            1001                //参数错误                                                  
#define NET_ERROR_RTSP_NOT_SUPPORT              1002                //不支持的方法                                              
#define NET_ERROR_RTSP_NO_MEMORY                1003                //内存不足                                                  
#define NET_ERROR_RTSP_TIMEDOUT                 1004                //访问超时                                                  
#define NET_ERROR_RTSP_AGAIN                    1005                //请重试                                                    
#define NET_ERROR_RTSP_UNIMPLEMENTED            1006                //未实现                                                    
#define NET_ERROR_RTSP_WRONG_STATUS             1007                //错误的状态                                                
#define NET_ERROR_RTSP_PROTOCOL                 1008                //不支持的协议   
#define NET_ERROR_RTSP_NOTFOUND                 1009                //record file not found
#define NET_ERROR_RTSP_AUTHFAILED               1010                //Auth Failed
#define NET_ERROR_RTSP_FAULT                    1255                //General fault

//1400~1500 播放库错误


/*
*exception callback
*/
#define EXCP_HEARTBEAT_FAIL			        0x0001	            //用户交互时异常
#define EXCP_HEARTBEAT_RESUME			    0x0002	            //用户交互恢复
#define EXCP_VOICE_TALK			            0x0003	            //语音对讲异常
#define EXCP_ALARMCHAN_FAIL					0x0004	            //报警异常
#define EXCP_ALARMCHAN_RECONNECT		    0x0007	            //报警时重连
#define EXCP_ALARMCHAN_RESUME			    0x0010              //报警时重连成功
#define EXCP_REALPLAY_FAIL				    0x0005	            //预览异常
#define EXCP_REALPLAY_RECONNECT				0x0006	            //预览时重连
#define EXCP_REALPLAY_RESUME		        0x0009              //预览时重连成功
#define EXCP_PLAYBACK_FAIL				    0x0008	            //回放异常

#endif
