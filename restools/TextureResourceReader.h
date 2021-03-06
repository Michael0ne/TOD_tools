#pragma once
#include "GenericResourceReader.h"

class TextureResourceReader : public GenericResourceReader
{
private:
	struct TextureResourceHeader
	{
		unsigned int	m_Timestamp = NULL;
		unsigned int	m_ResInfoSize = NULL;
		unsigned int	m_ResDataSize = NULL;
		unsigned int	m_CheckRegion = NULL;
		unsigned int	m_RegionStrLen = NULL;
	};
	TextureResourceHeader	m_ResHeader;
	char					m_Region[4] = {};

	struct TextureResourceInfo
	{
		unsigned int	field_0[12] = {};
		char* m_ResourceOriginalPath = nullptr;
		char			field_8[4] = {};
		unsigned int	field_C[34] = {};
	};
	TextureResourceInfo		m_ResInfo;

	struct TextureResourceChunk
	{
		char			Chunk[2048] = {};
	};

	unsigned int			m_TotalChunks = NULL;
	TextureResourceChunk* m_Chunks = nullptr;

	static const std::string	PlatformExtension[];
public:
	TextureResourceReader(const char* const filename, PlatformDefinition platform = PLATFORM_PC);
	~TextureResourceReader();

	virtual void	ReadInfo() override;
	virtual void	PrintInfo() const override;
};