
#ifndef __GLOG_H__
#define __GLOG_H__

#if (defined _WIN32 || defined _WIN64)
#   ifdef GLOG_EXPORTS
#       define GLOG_DECLARE extern "C" __declspec(dllexport)
#   else
#		pragma comment(lib, "log.lib")
#       define GLOG_DECLARE extern "C" __declspec(dllimport)
#   endif
#   define GLOG_API __stdcall
#else
#   ifdef __linux__
#       define GLOG_DECLARE extern "C"
#   else
#       define GLOG_DECLARE
#   endif
#   define GLOG_API
#endif

/** @enum GLOG_LEVEL
 *  @brief 日志级别
 */
typedef enum {
    GLOG_LEVEL_OFF    = 7,
    GLOG_LEVEL_FATAL  = 6,
    GLOG_LEVEL_ERROR  = 5,
    GLOG_LEVEL_WARN   = 4,
    GLOG_LEVEL_INFO   = 3,
    GLOG_LEVEL_DEBUG  = 2,
    GLOG_LEVEL_TRACE  = 1,
    GLOG_LEVEL_ALL    = 0
} GLOG_LEVEL;

#ifdef __GNUC__
#define LOG_CHECK_FMT(a, b) __attribute__((format(printf, a, b)))
#define CALLBACK
#else
#define LOG_CHECK_FMT(a, b)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (GLOG_API *GLOG_CB)(GLOG_LEVEL level, const char* message);

    /** @fn void glog_init(const char* name, bool async = false)
    *   @brief 日志信息初始化
    *   @param name 名称，日志文件的前缀或配置文件xxx.log4cxx的xxx部分，也可以是log4cxx.properties和log4cxx.xml
    *   @param async 是否启用异步写日志任务
    */
    GLOG_DECLARE void GLOG_API glog_init(const char* name, bool async = false);

    /** @fn void glog_ocx_init(const char* name, const char* path, bool async = false)
    *   @brief 日志信息初始化
    *   @param name 名称，日志文件的前缀或配置文件xxx.log4cxx的xxx部分，也可以是log4cxx.properties和log4cxx.xml
    *   @param path 配置文件路径,path最后不必加"/"
    *   @param async 是否启用异步写日志任务
    */
    GLOG_DECLARE void GLOG_API glog_ocx_init(const char* name, const char* path, bool async = false);

    /** @fn void glog_fini()
    *   @brief 日志系统反初始化
    */
    GLOG_DECLARE void GLOG_API glog_fini();

    /** @fn void glog_set_callback(GLOG_CB callback)
    *   @brief 设置日志的回调函数
    *   @param callback 用于处理日志信息的回调函数
    */
    GLOG_DECLARE void GLOG_API glog_set_callback(GLOG_CB callback);

    /** @fn void glog_level(GLOG_LEVEL level)
    *   @brief 设置日志的输出级别
    *   @param level 日志级别
    */
    GLOG_DECLARE void GLOG_API glog_level(GLOG_LEVEL level);

    /** @fn void glog_format(GLOG_LEVEL level, const char* module, const char* format, ...)
    *   @brief 写日志格式化数据
    *   @param level 日志级别
    *   @param module 日志模块
    *   @param format 日志格式化字符串，后接可变参数
    *   @note glog_format会调用glog_set_callback传入的回调函数
    */
    GLOG_DECLARE void GLOG_API glog_format(GLOG_LEVEL level, const char* module, const char* format, ...) LOG_CHECK_FMT(3, 4);

    /** @fn void glog_message(GLOG_LEVEL level, const char* module, const char* message)
    *   @brief 写日志字符串
    *   @param level 日志级别
    *   @param module 日志模块
    *   @param message 日志字符串
    *   @note glog_message不会调用glog_set_callback传入的回调函数
    */
    GLOG_DECLARE void GLOG_API glog_message(GLOG_LEVEL level, const char* module, const char* message);

#ifdef __cplusplus
}
#endif

// 考虑兼容性，特增加log_printf_def和log_printf_dst
#if (defined _WIN32 || defined _WIN64)
#define log_printf_def(level, format, ...)      glog_format(level, "", format, ##__VA_ARGS__)
#else
#define log_printf_def(level, format, args...)  glog_format(level, "", format, ##args)
#endif
#define log_printf_dst glog_format

// for defualt module
#if (defined _WIN32 || defined _WIN64)
    #define LOG_ERROR(fmt, ...)         glog_format(GLOG_LEVEL_ERROR, "", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define LOG_WARN(fmt, ...)          glog_format(GLOG_LEVEL_WARN, "", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define LOG_INFO(fmt, ...)          glog_format(GLOG_LEVEL_INFO, "", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define LOG_DEBUG(fmt, ...)         glog_format(GLOG_LEVEL_DEBUG, "", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define LOG_TRACE(fmt, ...)         glog_format(GLOG_LEVEL_TRACE, "", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
    #define LOG_ERROR(fmt, args...)     glog_format(GLOG_LEVEL_ERROR, "", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define LOG_WARN(fmt, args...)      glog_format(GLOG_LEVEL_WARN, "", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define LOG_INFO(fmt, args...)      glog_format(GLOG_LEVEL_INFO, "", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define LOG_DEBUG(fmt, args...)     glog_format(GLOG_LEVEL_DEBUG, "", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define LOG_TRACE(fmt, args...)     glog_format(GLOG_LEVEL_TRACE, "", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

// for HPP module
#if (defined _WIN32 || defined _WIN64)
    #define HPP_ERROR(fmt, ...)         glog_format(GLOG_LEVEL_ERROR, "HPP", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define HPP_WARN(fmt, ...)          glog_format(GLOG_LEVEL_WARN, "HPP", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define HPP_INFO(fmt, ...)          glog_format(GLOG_LEVEL_INFO, "HPP", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define HPP_DEBUG(fmt, ...)         glog_format(GLOG_LEVEL_DEBUG, "HPP", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define HPP_TRACE(fmt, ...)         glog_format(GLOG_LEVEL_TRACE, "HPP", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
    #define HPP_ERROR(fmt, args...)     glog_format(GLOG_LEVEL_ERROR, "HPP", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define HPP_WARN(fmt, args...)      glog_format(GLOG_LEVEL_WARN, "HPP", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define HPP_INFO(fmt, args...)      glog_format(GLOG_LEVEL_INFO, "HPP", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define HPP_DEBUG(fmt, args...)     glog_format(GLOG_LEVEL_DEBUG, "HPP", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define HPP_TRACE(fmt, args...)     glog_format(GLOG_LEVEL_TRACE, "HPP", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

// for MQ module
#if (defined _WIN32 || defined _WIN64)
    #define MQ_ERROR(fmt, ...)          glog_format(GLOG_LEVEL_ERROR, "MQ", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define MQ_WARN(fmt, ...)           glog_format(GLOG_LEVEL_WARN, "MQ", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define MQ_INFO(fmt, ...)           glog_format(GLOG_LEVEL_INFO, "MQ", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define MQ_DEBUG(fmt, ...)          glog_format(GLOG_LEVEL_DEBUG, "MQ", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define MQ_TRACE(fmt, ...)          glog_format(GLOG_LEVEL_TRACE, "MQ", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
    #define MQ_ERROR(fmt, args...)      glog_format(GLOG_LEVEL_ERROR, "MQ", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define MQ_WARN(fmt, args...)       glog_format(GLOG_LEVEL_WARN, "MQ", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define MQ_INFO(fmt, args...)       glog_format(GLOG_LEVEL_INFO, "MQ", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define MQ_DEBUG(fmt, args...)      glog_format(GLOG_LEVEL_DEBUG, "MQ", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define MQ_TRACE(fmt, args...)      glog_format(GLOG_LEVEL_TRACE, "MQ", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

#endif // __GLOG_H__
