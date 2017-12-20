#pragma once
#ifndef LOG_MACROS_H
#define LOG_MACROS_H

#define WWFILE strrchr(__FILE__,'\\')?strrchr(__FILE__,'\\')+1:__FILE__
#include "glog.h"
#define UPDATESERVER_ERROR(fmt, ...)         glog_format(GLOG_LEVEL_ERROR, "UpdateService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__)
#define UPDATESERVER_WARN(fmt, ...)          glog_format(GLOG_LEVEL_WARN,  "UpdateService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__) 
#define UPDATESERVER_INFO(fmt, ...)          glog_format(GLOG_LEVEL_INFO,  "UpdateService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__)  
#define UPDATESERVER_DEBUG(fmt, ...)         glog_format(GLOG_LEVEL_DEBUG, "UpdateService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__)  
#define UPDATESERVER_TRACE(fmt, ...)         glog_format(GLOG_LEVEL_TRACE, "UpdateService","[%s:%d] " fmt, WWFILE , __LINE__,##__VA_ARGS__)  

#endif