#include "StringsPool.h"

inline void PATCH_STRING_BUFFER()
{}

String* String::Substring(String* outStr, int posStart, int length)
{
	if (posStart > m_nLength) {
		outStr->m_nLength = 0;
		outStr->m_szString = &outStr->m_pEmpty;
		outStr->m_nBitMask = 0x80000004;
		outStr->m_pEmpty = NULL;

		return outStr;
	}

	if (posStart + length <= m_nLength) {
		outStr->m_nLength = length;
		outStr->m_nBitMask = (0x80000000 ^ (length + (length >> 2))) & 0x7FFFFFFF ^ 0x80000000;
		outStr->m_pEmpty = NULL;
		outStr->m_szString = (char*)Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(outStr->m_nBitMask & 0x7FFFFFFF);
		strncpy(outStr->m_szString, &m_szString[posStart], length);
	}else{
		outStr->m_nLength = m_nLength - posStart;
		outStr->m_nBitMask = (0x80000000 ^ ((m_nLength - posStart) + ((m_nLength - posStart) >> 2))) & 0x7FFFFFFF ^ 0x80000000;
		outStr->m_pEmpty = NULL;
		outStr->m_szString = (char*)Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(outStr->m_nBitMask & 0x7FFFFFFF);
		strncpy(outStr->m_szString, &m_szString[posStart], m_nLength - posStart);
	}

	return outStr;
}

void String::Set(const char* str)
{
	if (m_szString != &m_pEmpty && m_nBitMask < 0)
		Allocators::MemoryAllocators::ReleaseMemory(m_szString, 0);

	m_nLength = strlen(str);
	m_nBitMask = 0x80000000 ^ (0x80000000 ^ (m_nLength + (m_nLength >> 1))) & 0x7FFFFFFF;

	m_szString = (char*)Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(m_nBitMask & 0x7FFFFFFF);
	m_nBitMask |= 0x80000000;

	m_pEmpty = '\0';

	strcpy(m_szString, str);
}

void String::Append(const char* str)
{
	int _len = m_nLength;
	int _len_str = strlen(str);

	m_nLength = _len + _len_str;

	_AdjustBufferSize();

	memcpy(&m_szString[_len], str, _len_str + 1);
}

bool String::Equal(const char* _str)
{
	//	If base string is empty.
	if (m_szString == &m_pEmpty && m_nBitMask < 0)
		return false;

	//	If strings have same memory address.
	if (m_szString == _str)
		return true;

	//	If first character is null.
	if (!*m_szString)
		if (!*_str)
			return true;
		else
			return false;

	char* _base = m_szString;
	char _char = *m_szString;
	while (_char == *_str)
	{
		_char = (_base++)[1];
		_str++;

		if (!_char)
			if (!*_str)
				return true;
			else
				return false;
	}

	if (*m_szString)
		return false;

	if (!*_str)
		return true;

	return false;
}

bool String::EqualIgnoreCase(const char* str1, const char* str2, unsigned int len)
{
	if (!len)
		return true;

	char* str1_ = (char*)str1;
	char* str2_ = (char*)str2;
	unsigned int ind = 0;

	while (true)
	{
		char str1_char = *str1_;
		char str2_char = *str2_;

		++str1_;
		++str2_;
		++ind;

		if (!str1_char)
			return str2_char == 0;

		if (!str2_char || (str2_char & str1_char) & 0xDF)
			break;

		if (ind == len)
			return true;
	}

	return false;
}

void String::Format(const char* format, ...)
{
	char buffer[MAX_PATH];

	va_list va;

	va_start(va, format);
	vsnprintf(buffer, sizeof(buffer), format, va);

	//	Allocate enough space and strcpy.
	Set(buffer);
}
