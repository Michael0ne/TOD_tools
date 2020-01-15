#pragma once

#include "stdafx.h"

//	NOTE: the name is probably wrong. It looks more like an object of StringBufferPool or smth.
//	Proper name could be StringObject.
class KapowStringBuffer {
private:
	//	Size is probably 10 (0xC) bytes.
	int		m_nLength;		//	+ 0x0 = how much characters is in this string.
	char*	m_pszString;	//	+ 0x4 = pointer to actual string.
	int		m_nUnkBitMask;	//	+ 0x8 = unknown bit mask.
	char*	m_pUnkStrPtr;	//	+ 0xC = unknown pointer to string. Maybe next or default string?

public:
	KapowStringBuffer(char* pString);	//	>> 405A80

	void	TryAllocate();	//	>> 405520
	void	AllocateSpaceForString();	//	>> 4056E0
	void	Append(KapowStringBuffer* baseString, const char* affixString);		//	>> 4058C0
	void	Append(const char* baseString, KapowStringBuffer* affixString);		//	>> 405940
	void	CopyFrom(KapowStringBuffer* pStringBuffer);	//	>> 405AE0
};
