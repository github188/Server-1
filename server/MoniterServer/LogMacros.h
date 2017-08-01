#ifndef LOG_MACROS_H
#define LOG_MACROS_H

#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib") 

static LONG WINAPI unhandledExceptionFilter(EXCEPTION_POINTERS* pExp)
{
	char name[1024 + 1];
	{
		auto nameEnd = name + GetModuleFileNameA(GetModuleHandleA(0), name, 1024);
		SYSTEMTIME t;
		GetLocalTime(&t);
		wsprintfA(nameEnd - strlen(".exe"),
			"_%4d%02d%02d_%02d%02d%02d.dmp",
			t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	}

	auto hFile = CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return EXCEPTION_EXECUTE_HANDLER;

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = pExp;
	exceptionInfo.ClientPointers = FALSE;

	MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		MINIDUMP_TYPE(MiniDumpNormal),
		&exceptionInfo,
		nullptr,
		nullptr);

	CloseHandle(hFile);
	return EXCEPTION_EXECUTE_HANDLER;
}

static void crashDump()
{
	SetUnhandledExceptionFilter(unhandledExceptionFilter);
}
#endif // _WIN32

#include "glog.h"
#define MONITERSERVER_ERROR(fmt, ...)         glog_format(GLOG_LEVEL_ERROR, "MoniterServer", fmt, ##__VA_ARGS__)
#define MONITERSERVER_WARN(fmt, ...)          glog_format(GLOG_LEVEL_WARN, "MoniterServer", fmt,  ##__VA_ARGS__)    
#define MONITERSERVER_INFO(fmt, ...)          glog_format(GLOG_LEVEL_INFO, "MoniterServer", fmt,  ##__VA_ARGS__)    
#define MONITERSERVER_DEBUG(fmt, ...)         glog_format(GLOG_LEVEL_DEBUG, "MoniterServer", fmt, ##__VA_ARGS__)    
#define MONITERSERVER_TRACE(fmt, ...)         glog_format(GLOG_LEVEL_TRACE, "MoniterServer", fmt, ##__VA_ARGS__)    

#endif