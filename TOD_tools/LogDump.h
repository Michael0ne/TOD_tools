#pragma once

#include "FileInternal.h"

#define LOGDUMP_BUFFER_SIZE 1024

class LogDump
{
public:
	static void		OpenLogDump(const char* path);	//	@40CA80
	static void		PrintDebugString(const char* str);	//	@40C850
	static void		PrintNewFrameInfo(const char* format, va_list args);	//	@40C8C0
	static void		LogA(char* msg, ...);	//	@40C9D0
	static void		Print_Impl(char* msg, ...);	//	@40C9F0

	static const bool	ShouldPrintNewFrameInfo = true;	//	@A082FC
	static int		OldFrameNumber;	//	@A0833C
};

extern FileInternal*	LogDumpFile;	//	@A35B84