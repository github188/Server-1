#ifndef LOG_MACROS_H
#define LOG_MACROS_H

#define WWFILE strrchr(__FILE__,'\\')?strrchr(__FILE__,'\\')+1:__FILE__
#include "glog.h"
#define MONITERSERVER_ERROR(fmt, ...)         glog_format(GLOG_LEVEL_ERROR, "MonitorService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__)
#define MONITERSERVER_WARN(fmt, ...)          glog_format(GLOG_LEVEL_WARN,  "MonitorService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__) 
#define MONITERSERVER_INFO(fmt, ...)          glog_format(GLOG_LEVEL_INFO,  "MonitorService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__)  
#define MONITERSERVER_DEBUG(fmt, ...)         glog_format(GLOG_LEVEL_DEBUG, "MonitorService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__)  
#define MONITERSERVER_TRACE(fmt, ...)         glog_format(GLOG_LEVEL_TRACE, "MonitorService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__)  

#endif