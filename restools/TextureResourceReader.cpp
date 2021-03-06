#include "TextureResourceReader.h"

const std::string TextureResourceReader::PlatformExtension[] =
{
	"",
	".texture_pc",
	".texture_ps2",
	".texture_xbox"
};

void TextureResourceReader::ReadInfo()
{
	if (!m_FilePtr ||
		m_LastErrorCode)
		return;

	fread(&m_ResHeader, sizeof(TextureResourceHeader), 1, m_FilePtr);

	for (unsigned int i = 0; i < sizeof(m_Region); i++)
	{
		fread(&m_Region[i], 1, 1, m_FilePtr);
		if (m_Region[i] == NULL)
			break;
	}

	long headerInfoEnd = ftell(m_FilePtr) + m_ResHeader.m_ResInfoSize;

	fread(m_ResInfo.field_0, sizeof(m_ResInfo.field_0), 1, m_FilePtr);

	char buf_[512] = {};

	for (unsigned int i = 0; i < sizeof(buf_); i++)
	{
		fread_s(&buf_[i], 1, 1, 1, m_FilePtr);
		if (buf_[i] == NULL)
		{
			m_ResInfo.m_ResourceOriginalPath = new char[i + 1];
			strcpy_s(m_ResInfo.m_ResourceOriginalPath, i + 1, buf_);
			break;
		}
	}

	for (unsigned int i = 0; i < sizeof(m_ResInfo.field_8); i++)
	{
		fread(&m_ResInfo.field_8[i], 1, 1, m_FilePtr);
		if (m_ResInfo.field_8[i] != 0xFFFFFFAB)
		{
			break;
		}
	}

	fseek(m_FilePtr, -1, SEEK_CUR);

	while (ftell(m_FilePtr) < headerInfoEnd)
		fread(m_ResInfo.field_C, sizeof(m_ResInfo.field_C), 1, m_FilePtr);

	m_TotalChunks = m_ResHeader.m_ResDataSize / sizeof(TextureResourceChunk);

	m_Chunks = new TextureResourceChunk[m_TotalChunks];
	void* _chnks = (void*)m_Chunks;

	fread(m_Chunks++, sizeof(TextureResourceChunk), m_TotalChunks, m_FilePtr);

	m_Chunks = (TextureResourceChunk*)_chnks;
}

void TextureResourceReader::PrintInfo() const
{
	printf("\tEngine timestamp:\t%d\n", m_ResHeader.m_Timestamp);
	printf("\tInfo header size:\t%d\n", m_ResHeader.m_ResInfoSize);
	printf("\tRes data size:\t%d (%d Kb)\n", m_ResHeader.m_ResDataSize, m_ResHeader.m_ResDataSize / 1024);
	printf("\tRes total chunks:\t%d\n", m_TotalChunks);
	printf("\tCheck region:\t%s\n", m_ResHeader.m_CheckRegion ? "true" : "false");
	printf("\tRegion:\t\"%s\"\n", m_Region);
	printf("\tResource info:\t\n");

	for (unsigned int i = 0; i < (sizeof(m_ResInfo.field_0) / sizeof(unsigned int)); i++)
		if (m_ResInfo.field_0[i] == 0xBAADF00D)
			continue;
		else
			printf("\t\t%x %x %x %x | %u\n",
				0x000000FF & (m_ResInfo.field_0[i] >> 24),
				0x000000FF & (m_ResInfo.field_0[i] >> 16),
				0x000000FF & (m_ResInfo.field_0[i] >> 8),
				0x000000FF & m_ResInfo.field_0[i],
				m_ResInfo.field_0[i]);

	printf("\tResource original path:\t\"%s\"\n", m_ResInfo.m_ResourceOriginalPath);

	for (unsigned int i = 0; i < sizeof(m_ResInfo.field_C) / sizeof(unsigned int); i++)
		if (m_ResInfo.field_C[i] == 0xBAADF00D)
			continue;
		else
			printf("\t\t%x %x %x %x | %u\n",
				0x000000FF & (m_ResInfo.field_C[i] >> 24),
				0x000000FF & (m_ResInfo.field_C[i] >> 16),
				0x000000FF & (m_ResInfo.field_C[i] >> 8),
				0x000000FF & m_ResInfo.field_C[i],
				m_ResInfo.field_C[i]);

	printf("\tNon-empty chunks dump:\n");

	TextureResourceChunk* chnks_ = m_Chunks;
	std::vector<unsigned int> emptychunksnum;

	printf("\t\tID | HEX | INT\n");
	for (unsigned int i = 0; i < m_TotalChunks; i++, chnks_++)
	{
		if ((int)(*chnks_->Chunk) == NULL ||
			(int)(*chnks_->Chunk) == 0xFFFFFFFF)
		{
			emptychunksnum.push_back(i);
			continue;
		}

		printf("\t\t%u | %x %x %x %x ... | %u ...\n", i, chnks_->Chunk[0], chnks_->Chunk[1], chnks_->Chunk[2], chnks_->Chunk[3], (int)(*chnks_->Chunk));
	}

	printf("\tChunks that are empty:\n\t\t");
	for (std::vector<unsigned int>::const_iterator it = emptychunksnum.cbegin(); it != emptychunksnum.cend(); ++it)
		printf("%u ", *it);
	printf("\n\t%u out of %u empty chunks.\n", emptychunksnum.size(), m_TotalChunks);
}

TextureResourceReader::TextureResourceReader(const char* const filename, PlatformDefinition platform)
{
	m_ResourceExtension = PlatformExtension[platform];
	m_ResourceName = filename;
	m_ResourceName += m_ResourceExtension;
}

TextureResourceReader::~TextureResourceReader()
{
	delete[] m_ResInfo.m_ResourceOriginalPath;
	delete[] m_Chunks;
}