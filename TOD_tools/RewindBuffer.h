#pragma once

#include "stdafx.h"

struct RewindBuffer_UnkStr
{
	int field_0;
	void* m_pUnkBuffer_Size32;
	int field_8;
};


class RewindBuffer
{
private:
		void* m_pBuffer;
		int m_nChunks;
		int m_nSize;
		int field_C;
		RewindBuffer_UnkStr m_pUnkStr;
		void* m_pUnkPtrToUnkStr;
		int field_20;
};

