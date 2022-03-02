#pragma once
#include "GenericResourceReader.h"
#include <deflate.h>

class SaveResourceReader : public GenericResourceReader
{
private:
    /* + 0 */UINT	m_Version = NULL;			//	NOTE: for TOD1 PC version it is 9.
    /* + 4 */UINT	m_PropListCRC = NULL;
    /* + 8 */UINT	m_EngineBuildVer = NULL;	//	NOTE: for TOD1 PC version it is 1925.
    /* + C */UINT	m_HeaderChecksum = NULL;
    /* +10 */UINT	m_SaveDataSize = NULL;            //  NOTE: actual size for save data, excluding header.
    /* +14 */UINT	m_BuffersCount = NULL;		//	NOTE: for TOD1 PC version it is 6.

    /* +18 */UINT	field_18 = NULL;			//	NOTE: this is supposedly read 4 times (?).
    /* +1C */UINT	m_SummaryDataSize = NULL;
    /* +20 */PCHAR	m_Summary = nullptr;		//	NOTE: if '_18' is not 0, then read to this field '_18' times.
    /* +24 */UINT	m_BuffersOffsets[7] = {};
    /* +3C */UINT	m_SavedGameTimeMs = NULL;
    /* +40 */UINT	m_SavedFrameNumber = NULL;

    /* +44 */UINT	field_44 = NULL;
    /* +48 */UINT	m_DeflateBufferSize = NULL;	//	NOTE: this is later multiplied by 4.
    /* +4C */PCHAR	m_DeflateBuffer = nullptr;

    z_stream		m_ActualDeflateBuffer;
    PCHAR			m_DeflateUncompressed = nullptr;

public:
    SaveResourceReader(LPCSTR filename);
    ~SaveResourceReader();

    virtual void	ReadInfo() override;
    virtual void	PrintInfo() const override;
    virtual void	DumpData() const override;
};