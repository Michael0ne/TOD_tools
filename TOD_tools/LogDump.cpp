#include "LogDump.h"
#include "Scene.h"
#include "Globals.h"

File* LogDumpFile = nullptr;
int LogDump::OldFrameNumber = -1;

void LogDump::OpenLogDump(const char* path)
{
	if (LogDumpFile)
		delete LogDumpFile;

	LogDumpFile = new File(path, 66, true);

	if (LogDumpFile->IsFileOpen())
		return;

	if (LogDumpFile)
		delete LogDumpFile;
	LogDumpFile = nullptr;

	debug("WARNING: Failed to open log dump file %s\n", path);
}

void LogDump::PrintDebugString(const char* str)
{
	OutputDebugString(str);

	//	NOTE: 'debug' call was added to output debug info into our file.
	debug((char*)str);

	if (LogDumpFile)
	{
		LogDumpFile->WriteBuffer(str);
		LogDumpFile->WriteBuffers();
	}
}

void LogDump::PrintNewFrameInfo(const char* format, va_list args)
{
	char str[LOGDUMP_BUFFER_SIZE];

	if (Script::FrameConsoleMarker && OldFrameNumber != Scene::NewFrameNumber)
	{
		OldFrameNumber = Scene::NewFrameNumber;
		int buffersize = snprintf(str, LOGDUMP_BUFFER_SIZE, " -- NEW FRAME %d (real time is %d, game time is %d)--\n", Scene::NewFrameNumber, Scene::RealTimeMs, Scene::GameTimeMs);

		if (buffersize < NULL || buffersize >= LOGDUMP_BUFFER_SIZE)
		{
			LogA("log: buffer overflow");

			return;
		}
	}

	int buffersize = vsnprintf(str, LOGDUMP_BUFFER_SIZE, format, args);

	if (buffersize >= NULL && buffersize < LOGDUMP_BUFFER_SIZE)
	{
		PrintDebugString(str);

		return;
	}

	LogA("log: buffer overflow");
}

void LogDump::LogA(char* msg, ...)
{
	va_list args;

	va_start(args, msg);

	if (ShouldPrintNewFrameInfo)
		PrintNewFrameInfo(msg, args);
}

void LogDump::Print_Impl(char* msg, ...)
{
	va_list args;

	va_start(args, msg);

	if (ShouldPrintNewFrameInfo)
		PrintNewFrameInfo(msg, args);
}