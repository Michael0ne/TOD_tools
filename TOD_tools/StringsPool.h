#pragma once

#include "stdafx.h"
#include "MemoryAllocators.h"

class String {
public:
#define STRING_BITMASK_DEFAULT	0x80000000
#define STRING_BITMASK_SHORT	0x80000004

	int		m_nLength;		//	String length, including null terminator.
	char*	m_szString;		//	Actual string ptr.
	unsigned int	m_nBitMask;		//	Bit mask, also contains string's length.
	char	m_pEmpty;		//	Default empty string.

	inline String() :
		m_nLength(0), m_szString(&m_pEmpty), m_nBitMask(0x80000000), m_pEmpty(NULL)
	{}

	String(const char* str)
	{
		m_nLength = strlen(str);
		m_nBitMask = (m_nBitMask ^ (m_nLength + (m_nLength >> 2))) & 0x7FFFFFFF ^ m_nBitMask;

		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate(m_nBitMask & 0x7FFFFFFF, NULL, NULL);

		m_pEmpty = NULL;

		strcpy_s(m_szString, m_nLength, str);
	}

	String(const String* rhs)
	{
		m_nLength = rhs->m_nLength;
		m_nBitMask = rhs->m_nBitMask;
		m_pEmpty = NULL;
		m_szString = &m_pEmpty;

		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate(m_nBitMask & 0x7FFFFFFF, NULL, NULL);

		strcpy_s(m_szString, m_nLength, rhs->m_szString);
	}

	//	Override assignment operator, when rvalue is String object.
	void operator=(const String& _r)
	{
		if (m_szString != &m_pEmpty && m_nBitMask < 0)
			Allocators::ReleaseMemory(m_szString, 0);

		m_nLength = _r.m_nLength;
		m_nBitMask = _r.m_nBitMask;
		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate(m_nBitMask & 0x7FFFFFFF, NULL, NULL);
		m_pEmpty = NULL;

		strcpy(m_szString, _r.m_szString);
	}

	inline ~String()
	{
		if (m_szString != &m_pEmpty && m_nBitMask & 0x80000000)
			Allocators::ReleaseMemory(m_szString, 0);

		m_szString = &m_pEmpty;
		m_nLength = 0;
		m_nBitMask &= 0x80000000;
		m_pEmpty = NULL;
	}

	//	NOTE: used when String object passed by reference.
	void Set(const char* str);

	void Append(const char* str);

	inline void ToLowerCase()
	{
		char* s_ = m_szString;
		while (*(s_++))
			if (*s_ >= 65 && *s_ <= 90)
				*s_ += 32;
	}

	bool Equal(const char* _str);	//	@40FE30

	static bool EqualIgnoreCase(const char* str1, const char* str2, unsigned int len);	//	@4177C0

	String* Substring(String* outStr, int posStart, int length);	//	@409E90

	inline void	ConvertBackslashes()	//	Always used as inlined function.
	{
		char* strPtr = m_szString;
		while (*(strPtr++))
			if (*strPtr == '\\')
				*strPtr = '/';
	}

	void Format(const char* format, ...);	//	@415300	NOTE: adapted to be String's class method.

private:

	void _AllocateSpaceForString()
	{
		void(__thiscall * _AllocString)(String * _this) = (void(__thiscall*)(String*))0x4056E0;

		_AllocString(this);
	}

	void _AdjustBufferSize()
	{
		void(__thiscall * _AdjBufSize)(String * _this) = (void(__thiscall*)(String*))0x405610;

		_AdjBufSize(this);
	}
};