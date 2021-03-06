#include "SaveResourceReader.h"

SaveResourceReader::SaveResourceReader(LPCSTR filename)
{
	m_ResourceExtension;	//	NOTE: no extension is applied.
	m_ResourceName = filename;
}

SaveResourceReader::~SaveResourceReader()
{
	delete[] m_Summary;
	delete[] m_DeflateBuffer;
	delete[] m_DeflateUncompressed;
}

void SaveResourceReader::ReadInfo()
{
	if (!m_FilePtr ||
		m_LastErrorCode)
		return;

	fread(&m_Version, sizeof(m_Version), 1, m_FilePtr);
	fread(&m_PropListCRC, sizeof(m_PropListCRC), 1, m_FilePtr);
	fread(&m_EngineBuildVer, sizeof(m_EngineBuildVer), 1, m_FilePtr);
	fread(&m_HeaderChecksum, sizeof(m_HeaderChecksum), 1, m_FilePtr);
	fread(&field_10, sizeof(field_10), 1, m_FilePtr);
	fread(&m_BuffersCount, sizeof(m_BuffersCount), 1, m_FilePtr);
	fread(&field_18, sizeof(field_18), 1, m_FilePtr);
	fread(&m_SummaryDataSize, sizeof(m_SummaryDataSize), 1, m_FilePtr);

	m_Summary = new CHAR[m_SummaryDataSize];
	fread(m_Summary, 1, m_SummaryDataSize, m_FilePtr);

	fread(m_BuffersOffsets, sizeof(UINT), m_BuffersCount, m_FilePtr);
	fread(&m_SavedGameTimeMs, sizeof(m_SavedGameTimeMs), 1, m_FilePtr);
	fread(&m_SavedFrameNumber, sizeof(m_SavedFrameNumber), 1, m_FilePtr);

	fread(&field_44, sizeof(field_44), 1, m_FilePtr);
	fread(&m_DeflateBufferSize, sizeof(m_DeflateBufferSize), 1, m_FilePtr);

	m_DeflateBuffer = new CHAR[m_DeflateBufferSize * 4];
	m_DeflateUncompressed = new CHAR[m_DeflateBufferSize * 4];
	fread(m_DeflateBuffer, m_DeflateBufferSize * 4, 1, m_FilePtr);

	m_ActualDeflateBuffer.zalloc = (alloc_func)0;
	m_ActualDeflateBuffer.zfree = (free_func)0;
	m_ActualDeflateBuffer.opaque = 0;
	m_ActualDeflateBuffer.next_in = (Bytef*)0;
	m_ActualDeflateBuffer.avail_in = 0;
	m_ActualDeflateBuffer.next_out = (Bytef*)0;

	inflateInit_(&m_ActualDeflateBuffer, ZLIB_VERSION, 56);

	m_ActualDeflateBuffer.next_in = (Bytef*)m_DeflateBuffer;
	m_ActualDeflateBuffer.avail_in = 4 * m_DeflateBufferSize;
	m_ActualDeflateBuffer.next_out = (Bytef*)m_DeflateUncompressed;
	m_ActualDeflateBuffer.avail_out = 4 * m_DeflateBufferSize;

	while (m_ActualDeflateBuffer.avail_out)
	{
		if (!m_ActualDeflateBuffer.avail_in)
			break;
		if (inflate(&m_ActualDeflateBuffer, Z_NO_FLUSH) == Z_STREAM_END)
			break;
	};

	inflateEnd(&m_ActualDeflateBuffer);
}

void SaveResourceReader::PrintInfo() const
{
	printf("\tSave version:\t%d\n", m_Version);
	printf("\tProperties List CRC:\t");
	PrintHex(m_PropListCRC);
	printf("\n");
	printf("\tEngine build version:\t%d\n", m_EngineBuildVer);
	printf("\tHeader checksum:\t");
	PrintHex(m_HeaderChecksum);
	printf("\n");
	printf("\tfield_10:\t%u\n", field_10);
	printf("\tBuffers in file:\t%d\n", m_BuffersCount);
	printf("\tfield_18:\t%X\n", field_18);
	printf("\tSummary data size:\t%u\n", m_SummaryDataSize);

	printf("\tSummary:\n\t\t");
	for (UINT i = NULL; i != m_SummaryDataSize; i++)
		printf("%u ", m_Summary[i]);
	printf("\n");

	printf("\tBuffers offsets (?):\n\t\t");
	for (UINT i = NULL; i != m_BuffersCount; i++)
		printf("%X ", m_BuffersOffsets[i]);
	printf("\n");

	printf("\tGame time:\t%u ms\n", m_SavedGameTimeMs);
	printf("\tSaved frame number:\t%u\n", m_SavedFrameNumber);

	printf("\tfield_44:\t%u\n", field_44);
	printf("\tDeflate buffer size:\t%u\n", m_DeflateBufferSize * 4);
	printf("\tUncompressed buffer first 16 bytes:\n");

	for (UINT i = NULL; i < 16; i += 4)
	{
		PrintHex(*(UINT*)(&m_DeflateUncompressed[i]));
		printf("\n");
	}
}

void SaveResourceReader::DumpData() const
{
	std::string outfilename = m_ResourceName + ".dump";
	FILE* ofile = fopen(outfilename.c_str(), "wb");
	fwrite(m_DeflateUncompressed, m_DeflateBufferSize * 4, 1, ofile);
	fclose(ofile);

	printf("Saved dump: %s -> %s\n", m_ResourceName.c_str(), outfilename.c_str());
}