#pragma once

#include "stdafx.h"
#include "MemoryAllocators.h"

class String {
public:
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

		m_szString = (char*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(m_nBitMask & 0x7FFFFFFF);

		m_pEmpty = NULL;

		strcpy(m_szString, str);
	}

	//	Override assignment operator, when rvalue is String object.
	void operator=(const String& _r)
	{
		if (m_szString != &m_pEmpty && m_nBitMask < 0)
			Allocators::MemoryAllocators::ReleaseMemory(m_szString, 0);

		m_nLength = _r.m_nLength;
		m_nBitMask = _r.m_nBitMask;
		m_szString = (char*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(m_nBitMask & 0x7FFFFFFF);
		m_pEmpty = NULL;

		strcpy(m_szString, _r.m_szString);
	}

	inline ~String()
	{
		if (m_szString != &m_pEmpty && m_nBitMask & 0x80000000)
			Allocators::MemoryAllocators::ReleaseMemory(m_szString, 0);

		m_szString = &m_pEmpty;
		m_nLength = 0;
		m_nBitMask &= 0x80000000;
		m_pEmpty = NULL;
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

	bool Equal(const char* _str)	//	@40FE30
	{
		//	If base string is empty.
		if (m_szString == &m_pEmpty && m_nBitMask < 0)
			return false;

		//	If strings have same mem address.
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