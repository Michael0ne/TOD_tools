#pragma once

#include "stdafx.h"
#include "MemoryAllocators.h"

class String {
public:
	int		m_nLength;		//	String length, including null terminator.
	char*	m_szString;		//	Actual string ptr.
	int		m_nBitMask;		//	Bit mask, also contains string's length.
	char	m_pEmpty;		//	Default empty string.

	inline String() :
		m_nLength(0), m_szString(&m_pEmpty), m_nBitMask(0x80000000), m_pEmpty(NULL)
	{}

	String(const char* str)
	{
		m_nLength = strlen(str);
		m_nBitMask = 0x80000000 ^ (0x80000000 ^ (m_nLength + (m_nLength >> 1))) & 0x7FFFFFFF;	//	Bitmask contains string length + 1.

		m_szString = (char*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(m_nBitMask & 0x7FFFFFFF);
		m_nBitMask |= 0x80000000;

		m_pEmpty = '\0';

		strcpy(m_szString, str);
	}

	//	Override copy constructor to allow full copy of an object.
	String(const String& _copy)
		: m_nLength(_copy.m_nLength), m_szString(nullptr), m_nBitMask(_copy.m_nBitMask), m_pEmpty(NULL)
	{
		m_szString = (char*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(m_nBitMask & 0x7FFFFFFF);

		strcpy(m_szString, _copy.m_szString);
	}

	//	Override assignment operator, when rvalue is String object.
	String& operator=(const String& _r)
	{
		if (this == &_r)
			return *this;

		if (m_szString != &m_pEmpty && m_nBitMask < 0)
			Allocators::MemoryAllocators::ReleaseMemory(m_szString, 0);

		m_nLength = _r.m_nLength;
		m_nBitMask = _r.m_nBitMask;

		m_szString = (char*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(m_nBitMask & 0x7FFFFFFF);

		m_pEmpty = '\0';

		strcpy(m_szString, _r.m_szString);

		return *this;
	}

	inline ~String()
	{
		if (m_szString != &m_pEmpty && m_nBitMask < 0)
			Allocators::MemoryAllocators::ReleaseMemory(m_szString, 0);
	}

	//	NOTE: used when String object passed by reference.
	void Set(const char* str)
	{
		if (m_szString != &m_pEmpty && m_nBitMask < 0)
			Allocators::MemoryAllocators::ReleaseMemory(m_szString, 0);

		m_nLength = strlen(str);
		m_nBitMask = 0x80000000 ^ (0x80000000 ^ (m_nLength + (m_nLength >> 1))) & 0x7FFFFFFF;

		m_szString = (char*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(m_nBitMask & 0x7FFFFFFF);
		m_nBitMask |= 0x80000000;

		m_pEmpty = '\0';

		strcpy(m_szString, str);
	}

	void Append(const char* str)
	{
		int _len = m_nLength;
		int _len_str = strlen(str);
		
		m_nLength = _len + _len_str;

		_AdjustBufferSize();

		memcpy(&m_szString[_len], str, _len_str + 1);
	}

	void ToLowerCase()
	{
		if (!m_szString || m_szString == &m_pEmpty)
			return;

		int _ind = 0;
		do {
			if (m_szString[_ind] >= 65 && m_szString[_ind] <= 90)
				m_szString[_ind] += 32;
		} while (_ind < m_nLength);
	}

	String* Equal(String* str)
	{
		if (m_nLength == 0 || m_szString == &m_pEmpty)
			return nullptr;

		String* (__stdcall* _AreEqual)(String* _this, String* _str) = (String* (__stdcall*)(String*, String*))0x412A30;

		return _AreEqual(this, str);
	}

	String* Equal(const char* str)
	{
		if (m_nLength == 0 || m_szString == &m_pEmpty)
			return nullptr;

		String _stemp = String(str);
		String* (__stdcall * _AreEqual)(String * _this, String * _str) = (String * (__stdcall*)(String*, String*))0x412A30;

		return _AreEqual(this, &_stemp);
	}

	static String* Equal(String* base, String* str)	//	@412A30
	{
		String* (__stdcall * _AreEqual)(String* _this, String* _str) = (String* (__stdcall*)(String*, String*))0x412A30;

		return _AreEqual(base, str);
	}

	static String* Equal(const String& base, String* str)
	{
		String* (__stdcall * _AreEqual)(const String * _this, String * _str) = (String * (__stdcall*)(const String*, String*))0x412A30;

		return _AreEqual(&base, str);
	}

	String* Substring(String* outStr, int posStart, int length)
	{
		String* (__thiscall * _Substr)(String * _this, String * _out, int _start, int _len) = (String * (__thiscall*)(String*, String*, int, int))0x409E90;

		return _Substr(this, outStr, posStart, length);
	}

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