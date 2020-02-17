#pragma once

#include "stdafx.h"

#define KAPOW_STRINGS_POOL_SIZE 32

class String {
public:
	int		m_nLength;		//	String length, including null terminator.
	char*	m_szString;		//	Actual string ptr.
	int		m_nBitMask;		//	Bit mask, also contains string's length.
	char	m_pEmpty;		//	Default empty string.

	String() :
		m_nLength(0), m_szString(NULL), m_nBitMask(0x80000000), m_pEmpty(NULL)
	{}

	String(int size)
	{
		m_nLength = size + 1;
		m_pEmpty = NULL;
		m_nBitMask = 0x80000004;
		m_szString = &m_pEmpty;
	}

	String(const char* str)
	{
		if (!str) {
			String();

			return;
		}

		m_nLength = strlen(str) + 1;
		m_szString = (char*)malloc(m_nLength);

		if (m_szString == NULL) {
			String();

			return;
		}

		strcpy(m_szString, str);
		m_nBitMask = 0x80000004;
		m_pEmpty = NULL;
	}
};