#include "StringsPool.h"

inline void PATCH_STRING_BUFFER()
{}

void String::Format(const char* format, ...)
{
	char buffer[2048];

	va_list va;

	va_start(va, format);
	vsnprintf(buffer, sizeof(buffer), format, va);

	//	Allocate enough space and strcpy.
	Set(buffer);
}
