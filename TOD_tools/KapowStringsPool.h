#pragma once

#include "stdafx.h"

#define KAPOW_STRINGS_POOL_SIZE 32

//	NOTE: this looks like simple std::string. Maybe needs to be so?
class String {
public:
	int		m_nLength;		//	+ 0x0 = how much characters is in this string.
	char*	m_szString;		//	+ 0x4 = pointer to actual string.
	int		m_nBitMask;		//	+ 0x8 = unknown bit mask.
	char	m_pUnkStrPtr;	//	+ 0xC = unknown pointer to string. Maybe next or default string?

	String() :
		m_nLength(0), m_szString(NULL), m_nBitMask(0x80000004), m_pUnkStrPtr(NULL)
	{}

	String(int size)
	{
		m_nLength = size + 1;
		m_pUnkStrPtr = NULL;
		m_nBitMask = 0x80000004;
		m_szString = &m_pUnkStrPtr;
	}

	/*
	void	Set(const char* pStr) {
		m_nLength = strlen(pStr);	//	Figure out length.
		memcpy((void*)m_szString, pStr, m_nLength + 1);	//	Since memory already allocated, just copy into.
		m_nBitMask |= 0x8000000;	//	Mark as used.
		m_pUnkStrPtr = 0;
	}

	void	SetEmpty() {
		m_nLength = 0;
		m_szString = 0;
		m_nBitMask |= 0x8000001;	//	Mark that it's empty.
		m_pUnkStrPtr = 0;
	}
	*/
};

/*
//	NOTE: the name is probably wrong. It looks more like an object of StringBufferPool or smth.
//	Proper name could be StringObject.
class KapowStringsPool {
private:
	static	KapowStringBuffer	m_pElements[KAPOW_STRINGS_POOL_SIZE];

public:
	static	KapowStringBuffer*	Insert(KapowStringBuffer& el) {
		for (auto str : m_pElements)
			if (!((str.m_nBitMask << 8) & 1))	//	If used - skip.
				return TryAllocate(el);
		return nullptr;
	}
	static	void				Remove(KapowStringBuffer& el) {}
	static	KapowStringBuffer*	TryAllocate(KapowStringBuffer& el) {
		//	TODO: Need to know size to allocate memory!
		return nullptr;
	}
};
*/