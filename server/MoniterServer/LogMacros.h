#ifndef LOG_MACROS_H
#define LOG_MACROS_H

#include "glog.h"
#define MONITERSERVER_ERROR(fmt, ...)         glog_format(GLOG_LEVEL_ERROR, "MoniterServer", fmt, ##__VA_ARGS__)
#define MONITERSERVER_WARN(fmt, ...)          glog_format(GLOG_LEVEL_WARN, "MoniterServer", fmt,  ##__VA_ARGS__)    
#define MONITERSERVER_INFO(fmt, ...)          glog_format(GLOG_LEVEL_INFO, "MoniterServer", fmt,  ##__VA_ARGS__)    
#define MONITERSERVER_DEBUG(fmt, ...)         glog_format(GLOG_LEVEL_DEBUG, "MoniterServer", fmt, ##__VA_ARGS__)    
#define MONITERSERVER_TRACE(fmt, ...)         glog_format(GLOG_LEVEL_TRACE, "MoniterServer", fmt, ##__VA_ARGS__)    

#endif